/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.2.3
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */

#ifndef _PCAP2TAP_HPP_
#define _PCAP2TAP_HPP_

#include <list>

#include "pcap.hpp"
#include "tap.hpp"


class opt_t {
public:
	std::list<pcap> pcaps;
	std::list<tap> taps;
};


#endif /* _PCAP2TAP_HPP_ */

