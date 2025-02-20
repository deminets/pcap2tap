/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.1.2
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */

#include "dbgprn.hpp"
#include "pcap2tap.hpp"


// Опции
opt_t opt;


mac_t str2mac(std::string str) {
	mac_t mac = {0};
	int values[6];
	int i;

	if( 6 == sscanf( str.c_str(), "%x:%x:%x:%x:%x:%x%*c",
		&values[0], &values[1], &values[2],
		&values[3], &values[4], &values[5] ) )
	{
		/* convert to uint8_t */
		for( i = 0; i < 6; ++i )
			mac.mac[i] = (uint8_t) values[i];
	}else{
		/* invalid mac */
		DBG_ERR("ERROR: invalid mac = '%s'\n",str.c_str());
		exit(1);
	}

	return(mac);
}


void usage(char *prg) {
	PRN("Usage:\n"
		" %s [options]\n"
		"\nOptions:\n"
		" -h \t\t\t Print help and exit.\n"
		" -p <pcap file>\t\t Pcap-file for transmitted to TAP interface.\n"
		" -o <tap iface>\t\t name of the TAP-interface for transmitting packets\n"
		" -f <mac filter>\t Only src-MAC ==packets[] are allowed for the previously specified TAP interface.\n\t\t\t Can be specified multiple times.\n\t\t\t If not specified, all packets are transmitted\n"
		" -d <debug mode>\t 0-debug-off (default), 1=debig-on\n"
		"\nExample: %s -p test.pcap -o tap0 -f 00:11:22:33:44:55 -f 11:22:33:44:55:66\n"
		"\n", prg, prg
	);
}


int args(int ac, char *av[])
{
	int c;
	std::string str;
	mac_t mac;

	while ((c = getopt (ac, av, "hp:o:f:d:")) != -1)
		switch (c) {
		case 'h':
			usage(av[0]);
			exit(0);
			break;
		case 'p':
			str = (char*)optarg;
			opt.pcaps.emplace_back(str);
			break;
		case 'o':
			str = (char*)optarg;
			opt.taps.emplace_back(str);
			break;
		case 'f':
			str = (char*)optarg;
			mac = str2mac(str);
			if(opt.taps.empty()) {
				DBG_ERR("ERROR: first specify the interface (-f tap0)\n");
				exit(1);
			}
			opt.taps.back().add_filter_mac(mac);
			break;
		case 'd':
			str = (char*)optarg;
			dbgprn::setlevel( atoi( str.c_str() ) );
			break;
		default:
			exit(1);
			//abort();
		break;
		}

	return 0;
}


#define PRN_LOC(fmt, ...)

int main(int ac, char *av[])
{
	int r;
	int rpcks = 0, tpcks = 0;
	packet_t pkt;

	DBG_PRN("START\n");

	args(ac, av);

	DBG_PRN("READ\n");
	while(!opt.pcaps.empty()) {
		int count = 0;
		for (auto & it : opt.pcaps) {
			r = it.get_pkt(pkt);
			if(r == 1) {
				rpcks++;
				count++;
				for (auto & it : opt.taps) {
					r = it.send(pkt);
					if(r == 1) tpcks++;
				}
			}
		}
		if(!count) break;
	}

	DBG_PRN("read pcks = %d  send pcks = %d\n",rpcks,tpcks);
	
	DBG_PRN("EXIT\n");
	
	return 0;
}

