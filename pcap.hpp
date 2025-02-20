/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.1.2
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */

#ifndef _PCAP_HPP_
#define _PCAP_HPP_

#include <list>
#include <string>

#include "dbgprn.hpp"

#include <unistd.h>
#include <pcap/pcap.h>


typedef struct packet_s {
	struct pcap_pkthdr hdr;
	const uint8_t *data;
} packet_t;


class pcap {
public:
	pcap(std::string filename) {
	char errbuf[PCAP_ERRBUF_SIZE] = {0};
	pcapfn = filename;

	fd = pcap_open_offline(pcapfn.c_str(), errbuf);
	if (fd == NULL) {
		DBG_ERR("OPEN(%s) = '%s'\n",pcapfn.c_str(),errbuf);
		exit(1);
	}
	DBG_PRN("OPEN(%s)\n",pcapfn.c_str());
}

	~pcap() {
	DBG_PRN("CLOSE(%s)\n",pcapfn.c_str());
	if(fd) pcap_close(fd);
	fd = NULL;
}

	int get_pkt(packet_t &pkt) {
	pkt.data = pcap_next(fd, &pkt.hdr);
	if(pkt.data)
		return(1);
	return(0);
}

private:
	std::string pcapfn;
	pcap_t *fd;
};


#endif /* _PCAP_HPP_ */

