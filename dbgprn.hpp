/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.1.2
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */
#ifndef _DBGPRN_HPP_
#define _DBGPRN_HPP_

#include <string>


class dbglocale {
public:
	dbglocale();
	~dbglocale();
	std::string get(void);
private:
	char lang[3] = {0};
};
	
extern class dbglocale lang;


namespace dbgprn
{
extern int getlevel(void);
extern int setlevel(int lvl);
}

//extern "C++" {
#include <stdio.h>
#define DBG_PRN(fmt, ...)	do{ if(dbgprn::getlevel()) printf(fmt "\n", ##__VA_ARGS__ ); }while(0)
#define DBG_ERR(fmt, ...)	do{ fprintf(stderr, "ERROR: " fmt "\n", ##__VA_ARGS__ ); }while(0)
#define PRN(fmt, ...)		do{ fprintf(stdout, fmt, ##__VA_ARGS__ ); }while(0)
#define LANG()				do{ fprintf(stdout, "LANG=%s\n", lang.get().c_str() ); }while(0)
//}

#endif /* _DBGPRN_HPP_ */

