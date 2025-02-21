/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.2.3
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */

#include "dbgprn.hpp"
#include "loclang.hpp"
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
	}else{ /* invalid mac */
		LSW{
			LS(ru, DBG_ERR("ОШИБКА: недействительный MAC = '%s'\n",str.c_str()); )
			LD(en, DBG_ERR("ERROR: invalid MAC = '%s'\n",str.c_str()); )
		}
		exit(1);
	}

	return(mac);
}


void usage(char *prg) {
	LSW{
		LS(ru, PRN("Использование:\n"
			" %s [параметры]\n"
			"\nПараметры:\n"
			" -h \t\t\t Распечатать справку и выйти.\n"
			" -p <pcap файл>\t\t PCAP-файл для передачи в TAP-интерфейс.\n"
			" -o <tap интерфейс>\t Имя TAP-интерфейса для передачи пакетов\n"
			" -f <mac фильтр>\t Для ранее указанного TAP-интерфейса, передавать пакеты у которых mac_фильтр == packets[src-MAC]\n\t\t\t Можно указывать несколько опций.\n\t\t\t Если не указано, передаются все пакеты.\n"
			" -d <debug режим>\t 0-debug-выключено (по умолчанию), 1=debig-включено\n"
			"\nПример: %s -p test.pcap -o tap0 -f 00:11:22:33:44:55 -f 11:22:33:44:55:66\n"
			"\n", prg, prg ); )
		LD(en, PRN("Usage:\n"
			" %s [options]\n"
			"\nOptions:\n"
			" -h \t\t\t Print help and exit.\n"
			" -p <pcap file>\t\t PCAP-file for transmitted to TAP interface.\n"
			" -o <tap iface>\t\t Name of the TAP-interface for transmitting packets\n"
			" -f <mac filter>\t Only mac_filter == packets[src-MAC] are allowed for the previously specified TAP interface.\n\t\t\t Can be specified multiple times.\n\t\t\t If not specified, all packets are transmitted\n"
			" -d <debug mode>\t 0-debug-off (default), 1=debig-on\n"
			"\nExample: %s -p test.pcap -o tap0 -f 00:11:22:33:44:55 -f 11:22:33:44:55:66\n"
			"\n", prg, prg ); )
		}
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
				LSW{
					LS(ru, DBG_ERR("ОШИБКА: сначала укажите интерфейс (-f tap0)\n"); )
					LD(en, DBG_ERR("ERROR: first specify the interface (-f tap0)\n"); )
				}
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

	LSW{
		LS(ru, DBG_PRN("ЗАПУСК\n"); )
		LD(en, DBG_PRN("START\n"); )
	}

	args(ac, av);

	LSW{
		LS(ru, DBG_PRN("ЧТЕНИЕ pcap\n"); )
		LD(en, DBG_PRN("READ pcap\n"); )
	}

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

	LSW{
		LS(ru, DBG_PRN("прочитано %d пакетов,  передано %d пакетов\n",rpcks,tpcks); )
		LD(en, DBG_PRN("read %d pcks,  send %d pcks\n",rpcks,tpcks); )
	}

	LSW{
		LS(ru, DBG_PRN("ВЫХОД\n"); )
		LD(en, DBG_PRN("EXIT\n"); )
	}

	return 0;
}

