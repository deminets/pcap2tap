/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.2.3
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */

#include <string>
#include <regex>

#include "loclang.hpp"


#define TOSTR_(x...) #x


class locale_lang llng;


std::string lng_str[langid_len] = {
		"unk",
	#define	L(s,i)	TOSTR_(i),
		LANG_LIST
	#undef L
	};


locale_lang::locale_lang() {
	std::regex relng[] = {
	#define	L(s,i)	std::regex(s),
	LANG_LIST
	#undef L
	};
	int relng_sz = sizeof(relng)/sizeof(relng[0]);

	std::regex rg("^ru.*$");
	lang_id = lng_unk;
	lang = std::locale("").name();
	if(lang.find(".") > 0) lang.resize(lang.find("."));
	for(int i = 0 ; i < relng_sz; ++i) {
		if( std::regex_match(lang, relng[i]) ) {
			lang_id = i+1;
			break;
		}
	}
}

locale_lang::~locale_lang() {
}

int locale_lang::get(void) {
	return(lang_id);
}

std::string locale_lang::tostr(void) {
	return(lng_str[lang_id]);
}

