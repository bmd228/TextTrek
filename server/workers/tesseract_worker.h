#pragma once
#include <tesseract/baseapi.h>
#include <tesseract/renderer.h>
#include <leptonica/allheaders.h>
#include <spdlog/spdlog.h>
#include <iostream>
#include "config.h"


class TesseractWorker
{
public:
	TesseractWorker(const Config&);
	~TesseractWorker();
	
	std::string run_ocr_image(const std::string& data, const std::string& lang);
	std::string run_ocr_pdf(const std::string& data, const std::string& lang);
private:
	const Config& config;

};

