#include "tesseract_worker.h"
TesseractWorker::TesseractWorker(const Config& config_):config(config_)
{
    
}
std::string TesseractWorker::run_ocr_image(const std::string& data, const std::string& lang)
{
    auto start = std::chrono::steady_clock::now();
    tesseract::TessBaseAPI* api;
    std::string outText;
    api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(config.tesseract_data_path.c_str(), lang.c_str())) {
        SPDLOG_ERROR("Could not initialize tesseract.");
        exit(1);
    }
    //tesseract::LoadDataFromFile()
    //api->Init()
    //FileReader reader = NULL;
    api->SetPageSegMode(tesseract::PageSegMode::PSM_AUTO_OSD);
    // Open input image with leptonica library
    Pix* image = pixReadMem((const l_uint8*)data.data(), data.size());
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();

    // Destroy used object and release memory
    api->End();

    pixDestroy(&image);
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start);
    SPDLOG_TRACE("Language identification time  {} :{} seconds", std::get<0>(hash_pairr), duration.count());
    return outText;

}
std::string TesseractWorker::run_ocr_pdf(const std::string& data, const std::string& lang)
{
    return "";
}
 std::string TesseractWorker::parse_lang(const std::string& full_lang)
{
    std::istringstream ss(full_lang);
    std::unordered_set<std::string> languages{"eng"};

    std::string token;

    while (std::getline(ss, token, '+')) {
        if (support_lang.count(token) != 0)
            languages.emplace(support_lang[token]); // Move the token into the unordered set
        else
            SPDLOG_WARN("Language {} not support",token);
    }
    std::ostringstream oss;
    bool first = true; // флаг для первого элемента

    for (const auto& lang : languages) {
        if (!first) {
            oss << "+"; // Добавляем разделитель перед каждым элементом, кроме первого
        }
        else {
            first = false;
        }
        oss << lang; // Добавляем элемент в выходную строку
    }


    return oss.str();
}

TesseractWorker::~TesseractWorker()
{
}