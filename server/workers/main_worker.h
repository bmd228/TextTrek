#pragma once
#include <thread>
#include "../moodycamel/concurrentqueue.h"
#include "../moodycamel/blockingconcurrentqueue.h"
#include <functional>
#include <iostream>
#include <future>
#include <map>
#include "redis_worker.h"
#include "tesseract_worker.h"
#include "poppler_worker.h"
using Task = std::function<void()>;
struct MessageResponse
{
	enum  State {
		OK = 0,
		Error = 1
	};

	State state;
	std::string language;
	std::string pData;
	std::string error;

};
struct MessageRequest
{
	enum  Formats {
		AUTO =0,
		IMAGE = 1,
		PDF = 2,
		// Добавьте другие значения перечисления по мере необходимости
	};
	MessageRequest() {};
	MessageRequest(const std::string& pData_, const std::string& language_, const Formats& format_) :pData(pData_), language(language_), format(format_) {};
	static Formats stringToFormat(const std::string& str);
	Formats format;
	std::string language;
	std::string pData;

};
class ThreadPool
{
public:
	ThreadPool(const Config&);
	~ThreadPool();
	std::future<MessageResponse> enqueue(const MessageRequest& input);
	std::future<std::pair<int, std::string>> ThreadPool::enqueueFromTask(const std::string& input, const std::string& lang ,const int& page);

private:
	
	moodycamel::BlockingConcurrentQueue<Task> tasks; //Входная очередь тредпула
	std::vector<std::thread> threads;
	std::shared_ptr<RedisWorker> redis;
	const Config& config;

};


class MainWorker
{
public:
	MainWorker(const Config&);
	~MainWorker();
	MessageResponse push(const MessageRequest& in);
private:
	ThreadPool pool;

};

