#pragma once 
struct Config
{
	std::string redis_server{"localhost"};
	uint16_t redis_port{ 6379 };
	int redis_db{ 1 };
	int num_thread_worker{ 8 };
	std::string tesseract_data_path{ "D:\\tessdata_best-main" };
	int max_render{ 10 };
	std::string fasttext_data_file{ "D:\\lid.176.bin" };
	std::string http_server{ "localhost" };
	uint16_t http_port{ 8000 };
	std::string grpc_server{ "localhost" };
	uint16_t grpc_port{ 50051 };
};