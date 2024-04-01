#pragma once 
struct Config
{
	std::string redis_server{"localhost"};
	uint16_t redis_port{ 63793 };
	int redis_db{ 1 };
	int num_thread_worker{ 8 };
	std::string tesseract_data_path{ "D:\\tessdata_best-main" };
	int max_render{ 10 };
	std::string fasttext_data_file{ "D:\\lid.176.bin" };
};