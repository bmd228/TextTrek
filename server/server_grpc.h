#pragma once
//#define BOOST_ASIO_HAS_CO_AWAIT 1
#include <iostream>


#include "cascade.grpc.pb.h"



#include <agrpc/asioGrpc.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/experimental/channel.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/thread_pool.hpp>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <spdlog/spdlog.h>
#include "workers/main_worker.h"
#include "workers/config.h"
#include <optional>
#include <thread>
namespace asio = boost::asio;
using Channel = asio::experimental::channel<void(boost::system::error_code, ProtoAPI::Request)>;
struct ServerShutdown
{
	grpc::Server& server;
	asio::basic_signal_set<agrpc::GrpcContext::executor_type> signals;
	std::optional<std::thread> shutdown_thread;
	ServerShutdown(grpc::Server& server, agrpc::GrpcContext& grpc_context);
	~ServerShutdown();
	void shutdown();
};
class GrpcServer
{
public:
	GrpcServer(MainWorker&);
	~GrpcServer();
	void stop();
private:
	std::unique_ptr<grpc::Server> server;
	std::unique_ptr<ServerShutdown> server_shutdown;
	MainWorker& worker;
	asio::awaitable<void> reader(grpc::ServerAsyncReaderWriter<ProtoAPI::Response, ProtoAPI::Request>& reader_writer,
		Channel& channel);
	asio::awaitable<bool> writer(grpc::ServerAsyncReaderWriter<ProtoAPI::Response, ProtoAPI::Request>& reader_writer,
		Channel& channel, asio::thread_pool& thread_pool);
	asio::awaitable<void> handle_client_streaming_request(
		grpc::ServerContext&, grpc::ServerAsyncReader<ProtoAPI::Response, ProtoAPI::Request>& reader);
	asio::awaitable<void> handle_bidirectional_streaming_request(ProtoAPI::Example::AsyncService& service,
		asio::thread_pool& thread_pool);
	void register_client_streaming_handler(ProtoAPI::Example::AsyncService& service, agrpc::GrpcContext& grpc_context);
	asio::awaitable<void> handle_slow_unary_request(ProtoAPI::Example::AsyncService& service);
};