#pragma once
#include "controller/MyController.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "AppComponent.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp/web/server/api/Endpoint.hpp"

#include "oatpp-swagger/AsyncController.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include <iostream>

class HttpServer
{
public:
	HttpServer(MainWorker& worker);
	~HttpServer();
	void run();
	void stop();
private:
	MainWorker& worker;
	std::unique_ptr<AppComponent> components;
	std::unique_ptr <oatpp::web::server::api::Endpoints> docEndpoints;
	std::thread oatppThread;
	std::unique_ptr< oatpp::network::Server> server;
	std::shared_ptr<oatpp::web::server::AsyncHttpConnectionHandler> connectionHandler;
	std::shared_ptr< oatpp::network::tcp::server::ConnectionProvider> connectionProvider;
};

