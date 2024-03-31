#include "poppler_worker.h"
PopplerWorker::PopplerWorker()
{
    poppler::set_debug_error_function(dummy_error_function, nullptr);
}

PopplerWorker::~PopplerWorker()
{
}
std::tuple<std::string,std::vector<std::string>> PopplerWorker::rendering_start(const std::string& in)
{
    if (!poppler::page_renderer::can_render())
    {
        SPDLOG_WARN( "Renderer compiled without Splash support");
        return std::make_tuple("", std::vector<std::string>());
    }
    std::unique_ptr<poppler::document> doc(poppler::document::load_from_raw_data(in.data(), in.size()));

    if (!doc)
    {

        SPDLOG_INFO("Cant read pdf");
        return std::make_tuple("", std::vector<std::string>());
    }
    if (doc->is_locked())
    {
        SPDLOG_INFO("PDF is locked");
        return std::make_tuple("", std::vector<std::string>());
    }
    std::string text;
    double      dpi_x = 200.0;
    double      dpi_y = 200.0;
    meta        obj;
    int         num_check(0);
    obj.meta_load(std::ref(doc));
    poppler::font_iterator* it = doc->create_font_iterator();

    while (it->has_next())
    {
        std::vector<poppler::font_info> fonts = it->next();
        for (auto& k : fonts)
        {

            auto dd = k.name();
            auto ffd = k.file();
            auto dsd = k.is_embedded();

        }
        // do domething with the fonts
    }
    delete it;
    //if (doc->has_embedded_files())
    //{
    //    auto emb = doc->embedded_files();
    //    for (auto& k : emb)
    //    {

    //    }
    //}
    std::string utf8;
    std::vector<std::string> images;
    for (int i = 0; i < doc->pages(); i++)
    {
        poppler::page_renderer render;
        render.set_render_hint(poppler::page_renderer::text_antialiasing, true);
        std::unique_ptr<poppler::page> page(doc->create_page(i));
        if (!page)
        {
            continue;
        }
        utf8+=page->text().to_utf8().data();
        auto img = render.render_page(page.get(), dpi_x, dpi_y);
        ///////////////////////////////////////////
        int       dpi = -1;
        const int actual_dpi = dpi == -1 ? 75 : dpi;

        ///////////////////////////////////////////
        images.push_back(write_png_memory(img, actual_dpi));
    }
    return std::make_tuple(utf8, images);
}
std::string PopplerWorker::write_png_memory(poppler::image& img, int actual_dpi)
{
    int         width = img.width();
    int         height = img.height();
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
    {
        SPDLOG_ERROR("Not library libpng");
        return "";
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, nullptr);
        SPDLOG_INFO("Can't create png info");
        return "";
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        SPDLOG_ERROR("Can't write png header");
        return "";
    }
    std::stringstream out_stream;
    png_set_write_fn(
        png_ptr, &out_stream,
        [](png_structp png_ptr, png_bytep data, png_size_t lenght) {
            std::ostream* out_stream = reinterpret_cast<std::ostream*>(png_get_io_ptr(png_ptr));
            out_stream->write(reinterpret_cast<char*>(data), lenght);
        },
        nullptr);

    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    png_set_pHYs(png_ptr, info_ptr, actual_dpi / 0.0254, actual_dpi / 0.0254, PNG_RESOLUTION_METER);
    png_write_info(png_ptr, info_ptr);

    std::vector<unsigned char> row(3 * width);
    char* hptr = img.data();
    for (int y = 0; y < height; ++y)
    {
        unsigned char* rowptr = &row[0];
        for (int x = 0; x < width; ++x, rowptr += 3)
        {
            const unsigned int pixel = *reinterpret_cast<unsigned int*>(hptr + x * 4);
            rowptr[0] = (pixel >> 16) & 0xff;
            rowptr[1] = (pixel >> 8) & 0xff;
            rowptr[2] = pixel & 0xff;
        }
        rowptr = &row[0];
        png_write_rows(png_ptr, &rowptr, 1);
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            return "";
        }

        hptr += img.bytes_per_row();
    }

    //    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);
    //     delete[] row_pointers;
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return out_stream.str();
}