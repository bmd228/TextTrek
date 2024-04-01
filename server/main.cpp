// tesseract.cpp: определяет точку входа для приложения.
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
	
	//auto dd=worker.push("what");
	GrpcServer server_grpc(50051);
	cout << "Hello CMake." << endl;
	return 0;
}
