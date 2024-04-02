#pragma once
#include <thread>
#include "../moodycamel/concurrentqueue.h"
#include "../moodycamel/blockingconcurrentqueue.h"
#include <functional>
#include <iostream>
#include <future>
#include <map>
#include <numeric>
#include "redis_worker.h"
#include "tesseract_worker.h"
#include "poppler_worker.h"
#include "fasttext_worker.h"
#include "cascade.grpc.pb.h"

using Task = std::function<void()>;
using MessageResponse=ProtoAPI::Response;
using MessageRequest = ProtoAPI::Request;


class ThreadPool
{
public:
	ThreadPool(const Config&);
	~ThreadPool();
	std::future<MessageResponse> enqueue(const MessageRequest& input);
	std::future<std::pair<int, std::string>> ThreadPool::enqueueFromTask(const std::string& input, const std::string& lang ,const int& page);

private:
	std::unordered_map<std::thread::id,std::shared_ptr<FasttextWorker>> fasttext_workers;
	moodycamel::BlockingConcurrentQueue<Task> tasks; //¬ходна€ очередь тредпула
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
	const Config& get_config() { return config; };
private:
	ThreadPool pool;
	const Config config;
};

