#include "fasttext_worker.h"

FasttextWorker::FasttextWorker(const Config& config)
{
    try
    {

        fasttext.loadModel(config.fasttext_data_file); // opt.fileName.toLocal8Bit().data()
    }
    catch (const std::exception&)
    {

        SPDLOG_ERROR( "Error loading model");
    }
	
}
FasttextWorker::~FasttextWorker()
{
}

std::string FasttextWorker::run_work(const std::string& in)
{
    auto start = std::chrono::steady_clock::now();
    std::vector<std::pair<fasttext::real, std::string>> predictions;
    
    std::string lang = "XXX";
    fasttext::real         threshold = 0.0;
    int32_t                k = 1;
    std::istringstream iss(in);

    fasttext.predictLine(iss, predictions, k, threshold);
    if (!predictions.empty())
    {
        for (auto it = predictions.cbegin(); it != predictions.cend(); it++)
        {
            auto p = exp(it->first);
            if (p > 0.5)
            {
                lang = it->second.substr(9, lang.size() - 9);
                if (lang_to_lang.count(lang) == 0)
                    return "XXX";
                return lang_to_lang[lang];
            }
        }

    }
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start);
    SPDLOG_TRACE("Language identification time  :{} seconds", duration.count());
    return lang;
}