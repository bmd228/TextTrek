#include "App.h"

HttpServer::HttpServer(MainWorker& worker_):worker(worker_)
{
    oatpp::base::Environment::init();
    components = std::make_unique< AppComponent>();
    docEndpoints = std::make_unique< oatpp::web::server::api::Endpoints>();
}

HttpServer::~HttpServer()
{
    this->stop();
    oatpp::base::Environment::destroy();
}
void HttpServer::run() {

  /* Register Components in scope of run() method */
  

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Create MyController and add all of its endpoints to router */
  router->addController(std::make_shared<MyController>(worker));

  
  //OATPP_COMPONENT(std::shared_ptr<MainWorker>, worker);
  docEndpoints->append(router->addController(MyController::createShared(worker))->getEndpoints());

  router->addController(oatpp::swagger::AsyncController::createShared(*docEndpoints));

  /* Get connection handler component */
  //auto connectionHandler = oatpp::web::server::AsyncHttpConnectionHandler::createShared(router);
  //auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

  auto executor= std::make_shared<oatpp::async::Executor>(
      1 /* Data-Processing threads */,
      1 /* I/O threads */,
      1 /* Timer threads */
      );
  this->connectionHandler = oatpp::web::server::AsyncHttpConnectionHandler::createShared(router,executor);

  this->connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({ worker.get_config().http_server, worker.get_config().http_port, oatpp::network::Address::IP_4 });

  //OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  //OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  this->server = std::make_unique< oatpp::network::Server>(connectionProvider, connectionHandler);
 // oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Priny info about server port */
  OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

  
  oatppThread = std::thread([this] {
      /* Run server */
      this->server->run();
      });
  //server.run(true);

}
void HttpServer::stop() {
    if (server->getStatus() == oatpp::network::Server::STATUS_RUNNING) {
        server->stop();
    }

    /* Finally, stop the ConnectionHandler and wait until all running connections are closed */
    connectionHandler->stop();

    /* Before returning, check if the server-thread has already stopped or if we need to wait for the server to stop */
    if (oatppThread.joinable()) {

        /* We need to wait until the thread is done */
        oatppThread.join();
    }

}
//int main(int argc, const char * argv[]) {
//
//  oatpp::base::Environment::init();
//
//  run();
//
//  /* Print how much objects were created during app running, and what have left-probably leaked */
//  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
//  std::cout << "\nEnvironment:\n";
//  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
//  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
//
//  oatpp::base::Environment::destroy();
//
//  return 0;
//}
