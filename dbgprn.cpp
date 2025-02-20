/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.1.2
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */

#include <locale>

#include "dbgprn.hpp"

dbglocale::dbglocale() {
	std::locale("").name().copy(lang,2);
}

dbglocale::~dbglocale() {
}

std::string dbglocale::get(void) {
	return(lang);
}

class dbglocale lang;

namespace dbgprn
{

static int dbglvl = 0;

int getlevel(void) {
	return(dbglvl);
}

int setlevel(int lvl) {
	int ret = dbglvl;
	dbglvl = lvl;
	return(ret);
}


}

