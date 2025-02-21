/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.2.3
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */
#ifndef _LOCLANG_HPP_
#define _LOCLANG_HPP_

#include <string>

// Объявление списка языков
#define LANG_LIST	\
	L("^ru.*$",ru)	\
	L("^en.*$",en)	\


typedef enum {
		lng_unk = 0,
#define	L(s,i)	lng_##i,
		LANG_LIST
#undef L
		langid_len
	} langid_t;


class locale_lang {
public:
	locale_lang();
	~locale_lang();
	int get(void);
	std::string tostr(void);
private:
	std::string lang;
	int lang_id;
};

	
extern class locale_lang llng;


#define LSW				switch(llng.get()) 
#define LS(LNG, BODY)	case lng_##LNG: do{ BODY }while(0); break;
#define LD(LNG, BODY)	case lng_##LNG: default: do{ BODY }while(0); break;


#endif /* _LOCLANG_HPP_ */

