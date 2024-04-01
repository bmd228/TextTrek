#pragma once
#include <tesseract/baseapi.h>
#include <tesseract/renderer.h>
#include <leptonica/allheaders.h>
#include <spdlog/spdlog.h>
#include <iostream>
#include "config.h"
#include <unordered_set>
#include <sstream>
#include <string>

static std::unordered_map<std::string, std::string> support_lang =
{
	{"afr","afr"}, {"amh","amh"}, {"ara","ara+fas+kur_ara+pus+snd+uig+urd"}, {"asm","asm"}, {"aze","aze+aze_cyrl"}, {"bel","bel"}, {"ben","ben"}, {"bod","bod"}, {"bos","bos"}, {"bre","bre"}, {"bul","bul"}, {"cat","cat"}, {"ceb","ceb"}, {"ces","ces"}, {"zho","chi_sim+chi_tra"}, {"chr","chr"}, {"cos","cos"}, {"cym","cym"}, {"dan","dan"}, {"deu","deu"}, {"dzo","dzo"}, {"ell","ell"}, {"eng","eng"}, {"enm","enm"}, {"epo","epo"}, {"equ","equ"}, {"est","est"}, {"eus","eus"}, {"fao","fao"}, {"fas","fas"}, {"fil","fil"}, {"fin","fin"}, {"fra","fra"}, {"frk","frk"}, {"frm","frm"}, {"fry","fry"}, {"gla","gla"}, {"gle","gle"}, {"glg","glg"}, {"grc","grc"}, {"guj","guj"}, {"hat","hat"}, {"heb","heb"}, {"hin","hin"}, {"hrv","hrv"}, {"hun","hun"}, {"hye","hye"}, {"iku","iku"}, {"ind","ind"}, {"isl","isl"}, {"ita","ita"}, {"jav","jav"}, {"jpn","jpn"}, {"kan","kan"}, {"kat","kat"}, {"kaz","kaz"}, {"khm","khm"}, {"kir","kir"}, {"kmr","kmr"}, {"kor","kor+kor_vert"}, {"lao","lao"}, {"lat","lat"}, {"lav","lav"}, {"lit","lit"}, {"ltz","ltz"}, {"mal","mal"}, {"mar","mar"}, {"mkd","mkd"}, {"mlt","mlt"}, {"mon","mon"}, {"mri","mri"}, {"msa","msa"}, {"mya","mya"}, {"nep","nep"}, {"nld","nld"}, {"nor","nor"}, {"oci","oci"}, {"ori","ori"}, {"osd","osd"}, {"pan","pan"}, {"pol","pol"}, {"por","por"}, {"pus","pus"}, {"que","que"}, {"ron","ron"}, {"rus","rus"}, {"san","san"}, {"sin","sin"}, {"slk","slk"}, {"slv","slv"}, {"snd","snd"}, {"spa","spa"}, {"sqi","sqi"}, {"srp","srp+srp_latn"}, {"sun","sun"}, {"swa","swa"}, {"swe","swe"}, {"syr","syr"}, {"tam","tam"}, {"tat","tat"}, {"tel","tel"}, {"tgk","tgk"}, {"tha","tha"}, {"tir","tir"}, {"ton","ton"}, {"tur","tur"}, {"uig","uig"}, {"ukr","ukr"}, {"urd","urd"}, {"uzb","uzb+uzb_cyrl"}, {"vie","vie"}, {"yid","yid"}, {"yor","yor"}

};
class TesseractWorker
{
public:
	TesseractWorker(const Config&);
	~TesseractWorker();
	
	std::string run_ocr_image(const std::string& data, const std::string& lang);
	std::string run_ocr_pdf(const std::string& data, const std::string& lang);
	static std::string parse_lang(const std::string& );
private:
	const Config& config;

};

