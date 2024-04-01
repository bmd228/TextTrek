#include "main_worker.h"





ThreadPool::ThreadPool(const Config& config_):config(config_) {
    redis = std::make_shared<RedisWorker>(config_);
    
    for (size_t i = 0; i < config.num_thread_worker; ++i) {
        
        threads.emplace_back([this] {
            fasttext_workers.emplace(std::this_thread::get_id(), std::make_shared<FasttextWorker>(config));
            while (true) {
                Task task;
                if (tasks.try_dequeue(task)) {
                    task(); // ¬ыполн€ем задачу
                }
                else {
                    // ≈сли очередь пуста, поток засыпает на короткое врем€
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
            });
    }
}

ThreadPool::~ThreadPool()
{
    for (std::thread& worker : threads) {
        worker.join();
    }
}

std::future<std::pair<int,std::string>> ThreadPool::enqueueFromTask(const std::string& input, const std::string& lang,const int& page) {
    auto task = std::make_shared<std::packaged_task<std::pair<int, std::string>()>>([input,page,lang, this] {
        TesseractWorker tesseract(config);
        auto utf8 = tesseract.run_ocr_image(input, lang);
        return std::make_pair(page, utf8); // ƒождитесь выполнени€ вложенной задачи и верните результат
        });

    std::future<std::pair<int, std::string>> result = task->get_future();

    tasks.enqueue([task]() {
        (*task)();
        });

    return result;
}

std::future<MessageResponse> ThreadPool::enqueue(const MessageRequest& input) {
    auto task = std::make_shared<std::packaged_task<MessageResponse()>>([input,this] {
        // ¬ыполн€ем задачу, использу€ переданную строку
        // ¬ данном примере просто возвращаем строку
        MessageResponse message;
        
        auto hash_pairr=redis->check(input.pData);
        if (!std::get<1>(hash_pairr).empty() && !std::get<2>(hash_pairr).empty())
        {
            message.pData = std::get<1>(hash_pairr);
            message.language = std::get<2>(hash_pairr);
            message.state = MessageResponse::State::OK;
            return message;
        }
        else
        {
        auto start = std::chrono::steady_clock::now();
        std::string utf8;
        

        if (input.format == MessageRequest::Formats::IMAGE)
        {
            TesseractWorker tesseract(config);
            auto sup_lang=tesseract.parse_lang(input.language);
            utf8 = tesseract.run_ocr_image(input.pData, sup_lang);
        }
        if(input.format == MessageRequest::Formats::PDF)
        {
            std::vector< std::future<std::pair<int, std::string>>> complited_tasks;

            PopplerWorker poppler(config);
            auto pdf=poppler.rendering_start(input.pData);
            message.pData_recognize = std::accumulate(std::get<0>(pdf).begin(), std::get<0>(pdf).end(), std::string());
            message.language =fasttext_workers[std::this_thread::get_id()]->run_work(message.pData_recognize);
            auto sup_lang=TesseractWorker::parse_lang(input.language + "+" + message.language);
            for (size_t i = 0; i < std::get<1>(pdf).size(); i++)
            {
                complited_tasks.push_back(this->enqueueFromTask(std::get<1>(pdf).at(i), sup_lang, i));
            }
            
            for (auto& k : complited_tasks)
            {
                utf8 +=k.get().second;
            }

        }
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start);
        SPDLOG_TRACE("Elapsed time for hash {} :{} seconds", std::get<0>(hash_pairr), duration.count());        
        redis->push(utf8, message.language, std::get<0>(hash_pairr));
        message.pData = utf8;
        
      //  message.language = input.language;
        message.state = MessageResponse::State::OK;
        return message;
        }
        
    });

    std::future<MessageResponse> result = task->get_future();

    tasks.enqueue([task]() {
        (*task)();
    });
  //  std::this_thread::sleep_for(std::chrono::seconds(10));
    return result;
}
MessageRequest::Formats MessageRequest::stringToFormat(const std::string& str)
{
    static const std::map<std::string, Formats> formatMap = {
        {"AUTO", Formats::AUTO},
        {"IMAGE", Formats::IMAGE},
        {"PDF", Formats::PDF}
        // ƒобавьте другие соответстви€, если необходимо
    };

    // »щем соответствие строки в карте
    auto it = formatMap.find(str);
    if (it != formatMap.end()) {
        return it->second;
    }

    // ≈сли соответствие не найдено, возвращаем значение по умолчанию
    return Formats::AUTO; // или любое другое значение по умолчанию
}
MainWorker::MainWorker(const Config& config):pool(config)
{

}

MainWorker::~MainWorker()
{

}
MessageResponse MainWorker::push(const MessageRequest& in)
{
        auto future = pool.enqueue(in);
    //    std::cout << "dd" << future.get().text;
        return future.get();   
}
