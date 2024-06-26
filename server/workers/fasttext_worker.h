#pragma once
#include <iostream>
#include<unordered_map>
#include <spdlog/spdlog.h>
#include <sstream>
#include "../fastText/src/fasttext.h"
#include "config.h"
static std::unordered_map<std::string, std::string> lang_to_lang =
{
	{"ab","abk"}, {"aa","aar"}, {"af","afr"}, {"ak","aka"}, {"sq","sqi"}, {"am","amh"}, {"ar","ara"}, {"an","arg"}, {"hy","hye"}, {"as","asm"}, {"av","ava"}, {"ae","ave"}, {"ay","aym"}, {"az","aze"}, {"bm","bam"}, {"ba","bak"}, {"eu","eus"}, {"be","bel"}, {"bn","ben"}, {"bi","bis"}, {"bs","bos"}, {"br","bre"}, {"bg","bul"}, {"my","mya"}, {"ca","cat"}, {"ch","cha"}, {"ce","che"}, {"ny","nya"}, {"zh","zho"}, {"cu","chu"}, {"cv","chv"}, {"kw","cor"}, {"co","cos"}, {"cr","cre"}, {"hr","hrv"}, {"cs","ces"}, {"da","dan"}, {"dv","div"}, {"nl","nld"}, {"dz","dzo"}, {"en","eng"}, {"eo","epo"}, {"et","est"}, {"ee","ewe"}, {"fo","fao"}, {"fj","fij"}, {"fi","fin"}, {"fr","fra"}, {"fy","fry"}, {"ff","ful"}, {"gd","gla"}, {"gl","glg"}, {"lg","lug"}, {"ka","kat"}, {"de","deu"}, {"el","ell"}, {"kl","kal"}, {"gn","grn"}, {"gu","guj"}, {"ht","hat"}, {"ha","hau"}, {"he","heb"}, {"hz","her"}, {"hi","hin"}, {"ho","hmo"}, {"hu","hun"}, {"is","isl"}, {"io","ido"}, {"ig","ibo"}, {"id","ind"}, {"ia","ina"}, {"ie","ile"}, {"iu","iku"}, {"ik","ipk"}, {"ga","gle"}, {"it","ita"}, {"ja","jpn"}, {"jv","jav"}, {"kn","kan"}, {"kr","kau"}, {"ks","kas"}, {"kk","kaz"}, {"km","khm"}, {"ki","kik"}, {"rw","kin"}, {"ky","kir"}, {"kv","kom"}, {"kg","kon"}, {"ko","kor"}, {"kj","kua"}, {"ku","kur"}, {"lo","lao"}, {"la","lat"}, {"lv","lav"}, {"li","lim"}, {"ln","lin"}, {"lt","lit"}, {"lu","lub"}, {"lb","ltz"}, {"mk","mkd"}, {"mg","mlg"}, {"ms","msa"}, {"ml","mal"}, {"mt","mlt"}, {"gv","glv"}, {"mi","mri"}, {"mr","mar"}, {"mh","mah"}, {"mn","mon"}, {"na","nau"}, {"nv","nav"}, {"nd","nde"}, {"nr","nbl"}, {"ng","ndo"}, {"ne","nep"}, {"no","nor"}, {"nb","nob"}, {"nn","nno"}, {"ii","iii"}, {"oc","oci"}, {"oj","oji"}, {"or","ori"}, {"om","orm"}, {"os","oss"}, {"pi","pli"}, {"ps","pus"}, {"fa","fas"}, {"pl","pol"}, {"pt","por"}, {"pa","pan"}, {"qu","que"}, {"ro","ron"}, {"rm","roh"}, {"rn","run"}, {"ru","rus"}, {"se","sme"}, {"sm","smo"}, {"sg","sag"}, {"sa","san"}, {"sc","srd"}, {"sr","srp"}, {"sn","sna"}, {"sd","snd"}, {"si","sin"}, {"sk","slk"}, {"sl","slv"}, {"so","som"}, {"st","sot"}, {"es","spa"}, {"su","sun"}, {"sw","swa"}, {"ss","ssw"}, {"sv","swe"}, {"tl","tgl"}, {"ty","tah"}, {"tg","tgk"}, {"ta","tam"}, {"tt","tat"}, {"te","tel"}, {"th","tha"}, {"bo","bod"}, {"ti","tir"}, {"to","ton"}, {"ts","tso"}, {"tn","tsn"}, {"tr","tur"}, {"tk","tuk"}, {"tw","twi"}, {"ug","uig"}, {"uk","ukr"}, {"ur","urd"}, {"uz","uzb"}, {"ve","ven"}, {"vi","vie"}, {"vo","vol"}, {"wa","wln"}, {"cy","cym"}, {"wo","wol"}, {"xh","xho"}, {"yi","yid"}, {"yo","yor"}, {"za","zha"}, {"zu","zul"}

};
class FasttextWorker
{
public:
	FasttextWorker(const Config&);
	FasttextWorker(const FasttextWorker&) = default;
	//FasttextWorker(const FasttextWorker&) = delete;
	~FasttextWorker();
	std::string run_work(const std::string& );
private:
	fasttext::FastText                                          fasttext;
};

