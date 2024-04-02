#include "server_grpc.h"

ServerShutdown::ServerShutdown(grpc::Server& server, agrpc::GrpcContext& grpc_context)
    : server(server), signals(grpc_context, SIGINT, SIGTERM)
{
    signals.async_wait(
        [&](auto&&, auto&&)
        {
            shutdown();
        });
}
ServerShutdown::~ServerShutdown()
{
    if (shutdown_thread)
    {
        shutdown_thread->join();
    }
}
void ServerShutdown::shutdown()
{
    if (!shutdown_thread)
    {
        // This will cause all coroutines to run to completion normally
        // while returning `false` from RPC related steps, cancelling the signal
        // so that the GrpcContext will eventually run out of work and return
        // from `run()`.
        shutdown_thread.emplace(
            [&]
            {
                signals.cancel();
                server.Shutdown();
            });
        // Alternatively call `grpc_context.stop()` here instead which causes all coroutines
        // to end at their next suspension point.
        // Then call `server->Shutdown()` after the call to `grpc_context.run()` returns
        // or `.reset()` the grpc_context and go into another `grpc_context.run()`
    }
}
void abort_if_not(bool condition)
{
    if (!condition)
    {
        std::abort();
    }
}
template <class... Args>
void silence_unused(Args&&... args)
{
    ((void)args, ...);
}

asio::awaitable<void> GrpcServer::handle_client_streaming_request(grpc::ServerContext&, grpc::ServerAsyncReader<ProtoAPI::Response, ProtoAPI::Request>& reader)
{
    // Optionally send initial metadata first.
// Since the default completion token in asio-grpc is asio::use_awaitable, this line is equivalent to:
// co_await agrpc::send_initial_metadata(reader, asio::use_awaitable);
    bool send_ok = co_await agrpc::send_initial_metadata(reader);

    bool read_ok;
    do
    {
        ProtoAPI::Request request;
        // Read from the client stream until the client has signaled `writes_done`.
        read_ok = co_await agrpc::read(reader, request);
    } while (read_ok);

    ProtoAPI::Response response;
    bool finish_ok = co_await agrpc::finish(reader, response, grpc::Status::OK);

    // Or finish with an error
    // bool finish_with_error_ok = co_await agrpc::finish_with_error(reader, grpc::Status::CANCELLED);

    // See documentation for the meaning of the bool values

    silence_unused(send_ok, finish_ok);
}
asio::awaitable<void> GrpcServer::handle_bidirectional_streaming_request(ProtoAPI::Example::AsyncService& service,
    asio::thread_pool& thread_pool)
{
    grpc::ServerContext server_context;
    grpc::ServerAsyncReaderWriter<ProtoAPI::Response, ProtoAPI::Request> reader_writer{ &server_context };
    bool request_ok = co_await agrpc::request(&ProtoAPI::Example::AsyncService::RequestBidirectionalStreaming,
        service, server_context, reader_writer);
    if (!request_ok)
    {
        // Server is shutting down.
        co_return;
    }
    Channel channel{ co_await asio::this_coro::executor };

    using namespace asio::experimental::awaitable_operators;
    const auto ok = co_await(reader(reader_writer, channel) && writer(reader_writer, channel, thread_pool));

    if (!ok)
    {
        // Client has disconnected or server is shutting down.
        co_return;
    }

    bool finish_ok = co_await agrpc::finish(reader_writer, grpc::Status::OK);

    silence_unused(finish_ok);
}
asio::awaitable<void>  GrpcServer::reader(grpc::ServerAsyncReaderWriter<ProtoAPI::Response, ProtoAPI::Request>& reader_writer,
    Channel& channel)
{
    while (true)
    {
        ProtoAPI::Request request;
        if (!co_await agrpc::read(reader_writer, request))
        {
            break;
        }
        // Send request to writer. Using detached as completion token since we do not want to wait until the writer
        // has picked up the request.
        channel.async_send(boost::system::error_code{}, std::move(request), asio::detached);
    }
    // Signal the writer to complete.
    channel.close();
}
asio::awaitable<bool> GrpcServer::writer(grpc::ServerAsyncReaderWriter<ProtoAPI::Response, ProtoAPI::Request>& reader_writer,
    Channel& channel, asio::thread_pool& thread_pool)
{
    bool ok{ true };
    while (ok)
    {
        boost::system::error_code ec;
        const auto request = co_await channel.async_receive(asio::redirect_error(asio::use_awaitable, ec));
        if (ec)
        {
            // Channel got closed by the reader.
            break;
        }
        
        // Switch to the thread_pool.
        co_await asio::post(asio::bind_executor(thread_pool, asio::use_awaitable));
        // Compute the response.
        ProtoAPI::Response response= this->worker.push(request);
      //  response.set_pdata(request.pdata());
        // reader_writer is thread-safe so we can just interact with it from the thread_pool.
        ok = co_await agrpc::write(reader_writer, response);
        // Now we are back on the main thread.
    }
    co_return ok;
}

void GrpcServer::register_client_streaming_handler(ProtoAPI::Example::AsyncService& service, agrpc::GrpcContext& grpc_context)
{
    // Register a handler for all incoming RPCs of this method (Example::ClientStreaming) until the server is being
    // shut down. An API for requesting to handle a single RPC is also available:
    // `agrpc::request(&ProtoAPI::Example::AsyncService::RequestClientStreaming, service, server_context, reader)`

    //agrpc::repeatedly_request(&ProtoAPI::Example::AsyncService::RequestClientStreaming, service,
    //    asio::bind_executor(grpc_context, &handle_client_streaming_request));
}

GrpcServer::GrpcServer(MainWorker& worker_):worker(worker_)
{
   
   
    grpc::ServerBuilder builder;
    agrpc::GrpcContext grpc_context{ builder.AddCompletionQueue() };
    builder.AddListeningPort(worker_.get_config().grpc_server+":"+std::to_string(worker_.get_config().grpc_port), grpc::InsecureServerCredentials());
    ProtoAPI::Example::AsyncService  service;
    builder.RegisterService(&service);
    server = builder.BuildAndStart();
    abort_if_not(bool{ server });
    server_shutdown=std::make_unique<ServerShutdown>(*server, grpc_context );
    asio::thread_pool thread_pool{ 1 };
    register_client_streaming_handler(service, grpc_context);
    asio::co_spawn(
        grpc_context,
        [&]() -> asio::awaitable<void>
        {
            co_await handle_bidirectional_streaming_request(service, thread_pool);
        },
        asio::detached);
    grpc_context.run();  
    std::cout << "Shutdown completed\n";
}
void GrpcServer::stop()
{
    server_shutdown->shutdown();
}
GrpcServer::~GrpcServer()
{
    stop();
}