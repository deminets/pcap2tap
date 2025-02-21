/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.2.3
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */

#include "dbgprn.hpp"


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

