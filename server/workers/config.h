#pragma once 
struct Config
{
	std::string redis_server{"localhost"};
	uint16_t redis_port{ 6379 };
	int redis_db{ 1 };
	int num_thread_worker{ 8 };
	std::string tesseract_data_path{ "D:\\tessdata_best-main" };
};