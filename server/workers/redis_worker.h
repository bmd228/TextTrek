#pragma once
#include <sw/redis++/redis++.h>
#include <spdlog/spdlog.h>
#include "config.h"
class RedisWorker
{
public:
	RedisWorker(const Config&);
	~RedisWorker();
	std::tuple<size_t, std::string, std::string> check(const std::string&);
	void push(const std::string& data, const std::string& lang, const size_t& ke);
private:

	std::unique_ptr<sw::redis::Redis>                                redis;
};

