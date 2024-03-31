#pragma once
#include <cpp/poppler-document.h>
#include <cpp/poppler-image.h>
#include <cpp/poppler-page.h>
#include <cpp/poppler-page-renderer.h>
#include <cpp/poppler-embedded-file.h>
#include <cpp/poppler-font.h>
#include <png.h>
#include <sstream>
#include <iostream>
#include <tuple>
#include <spdlog/spdlog.h>
class meta;
class PopplerWorker
{
public:
	PopplerWorker();
	~PopplerWorker();
	std::string write_png_memory(poppler::image& img, int actual_dpi);
	std::tuple < std::string, std::vector<std::string> > rendering_start(const std::string& in);
private:
	static void dummy_error_function(const std::string&, void*)
	{
		return; //перегрузка error
	}
};

struct meta
{
public:
    poppler::byte_array autor;
    poppler::byte_array title;
    poppler::byte_array subject;
    poppler::byte_array keywords;
    poppler::byte_array description;
    std::string      modification_date;
    std::string      dateCreated;
    poppler::byte_array company;
    meta() {}
    void meta_load(std::unique_ptr<poppler::document>& doc)
    {


        if (!doc->get_title().empty())
            title = doc->get_title().to_utf8();
        if (!doc->get_subject().empty())
            subject = doc->get_subject().to_utf8();
        if (!doc->get_author().empty())
            autor = doc->get_author().to_utf8();
        if (!doc->get_keywords().empty())
            keywords = doc->get_keywords().to_utf8();
        modification_date = doc->get_modification_date();
        dateCreated = doc->get_creation_date();
        if (!doc->get_producer().empty())
        {
            company = doc->get_producer().to_utf8();
        }


    }
    ~meta()
    {
        autor.clear();
        title.clear();
        subject.clear();
        keywords.clear();
        description.clear();
        modification_date.clear();
        dateCreated.clear();
        company.clear();
    }
};