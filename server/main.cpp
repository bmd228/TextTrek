﻿// tesseract.cpp: определяет точку входа для приложения.
//

#include "main.h"

using namespace std;

int main()
{
#ifdef  WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif //  WIN32
	spdlog::set_level(spdlog::level::trace);
	Config config;
	SPDLOG_INFO("Worker init");
	MainWorker worker(config);
	SPDLOG_INFO("Start http server");
	HttpServer http_server(worker);
	http_server.run();
	
	GrpcServer server_grpc(worker);

	return 0;
}
