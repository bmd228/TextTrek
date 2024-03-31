#include "tesseract_worker.h"
TesseractWorker::TesseractWorker(const Config& config_):config(config_)
{
    
}
std::string TesseractWorker::run_ocr_image(const std::string& data, const std::string& lang)
{
    tesseract::TessBaseAPI* api;
    std::string outText;
    api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(config.tesseract_data_path.c_str(), lang.c_str())) {
        SPDLOG_ERROR("Could not initialize tesseract.");
        exit(1);
    }

    // Open input image with leptonica library
    Pix* image = pixReadMem((const l_uint8*)data.data(), data.size());
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();

    // Destroy used object and release memory
    api->End();

    pixDestroy(&image);
    return outText;

}
std::string TesseractWorker::run_ocr_pdf(const std::string& data, const std::string& lang)
{
    return "";
}


TesseractWorker::~TesseractWorker()
{
}