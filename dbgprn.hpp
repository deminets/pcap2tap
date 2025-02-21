/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.2.3
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */
#ifndef _DBGPRN_HPP_
#define _DBGPRN_HPP_

namespace dbgprn
{
class dbglocale {
public:
	dbglocale() { }
	~dbglocale() { }

private:
};
	
extern int getlevel(void);
extern int setlevel(int lvl);
}

#include <stdio.h>
#define DBG_PRN(fmt, ...)	do{ if(dbgprn::getlevel()) printf(fmt, ##__VA_ARGS__ ); }while(0)
#define DBG_ERR(fmt, ...)	do{ fprintf(stderr, fmt, ##__VA_ARGS__ ); }while(0)
#define PRN(fmt, ...)		do{ fprintf(stdout, fmt, ##__VA_ARGS__ ); }while(0)

#endif /* _DBGPRN_HPP_ */

