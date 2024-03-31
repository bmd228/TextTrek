#include "redis_worker.h" 
RedisWorker::RedisWorker(const Config& config)
{
    std::string uri = "tcp://"+ config.redis_server+":"+ std::to_string(config.redis_port) + "/" + std::to_string(config.redis_db);
    try
    {
        redis = std::make_unique<sw::redis::Redis>(uri);
       // sync_with_redis();
    }
    catch (const std::exception&)
    {
        SPDLOG_ERROR("Error connecting uri: {}. Shutting down...", uri);
    }
    catch (sw::redis::Error& err)
    {
        SPDLOG_ERROR(err.what());
    }
}
std::tuple<size_t,std::string,std::string> RedisWorker::check(const std::string & data)
{
    auto hash = std::hash<std::string>{}(data);
    auto string_hash_value = std::to_string(hash);
    std::vector<std::string> vals;
    try {
        if(redis->exists(string_hash_value))            
            redis->hmget(string_hash_value, {"data","lang"},std::back_inserter(vals));
        else
        {
            redis->set(string_hash_value,"", std::chrono::hours(3));
        }
    }
    catch (sw::redis::Error& err)
    {
        SPDLOG_ERROR("Get hash error: {}", err.what());
    }
    if (vals.size() == 2)
    {

    }
    else
    {
        auto start = std::chrono::steady_clock::now();
        while (true)
        {
            auto duration = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start);
            if (duration.count() > 3)
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            redis->hmget(string_hash_value, { "data","lang" }, std::back_inserter(vals));
            if (vals.size() == 2)
                break;
        }
    }
    //return vals.size()==2 ? std::make_tuple(hash, vals[0],vals[1]) : std::make_tuple(hash,"","");
}
void RedisWorker::push(const std::string& data, const std::string& lang,const size_t& key)
{
    auto string_hash_value = std::to_string(key);   
    try {
        redis->hmset(string_hash_value, { std::make_pair("data",data),std::make_pair("lang",lang) });
       // redis->hset(string_hash_value, std::make_pair(data, lang));
        redis->expire(string_hash_value, std::chrono::hours(3));
    }
    catch (sw::redis::Error& err)
    {
        SPDLOG_ERROR("Set hash error: {}", err.what());
    }
}
RedisWorker::~RedisWorker()
{
}