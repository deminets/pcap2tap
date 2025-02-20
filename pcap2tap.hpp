/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.1.2
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */

#ifndef _PCAPLAY_HPP_
#define _PCAPLAY_HPP_

#include <list>

#include "pcap.hpp"
#include "tap.hpp"


class opt_t {
public:
	std::list<pcap> pcaps;
	std::list<tap> taps;
};


#endif /* _PCAPLAY_HPP_ */
