/* Name:                pcap2tap
 * Date:                2025.02.15
 * Version:             0.2.3
 * Brief:               Play pcap to TAP.
 * License:             Apache License 2.0
 * Author(s):           Deminets
 * */

#ifndef _TAP_HPP_
#define _TAP_HPP_

#include <list>
#include <string>
#include <cstring>

#include <unistd.h>

#include <pcap/pcap.h>

#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>

#include "dbgprn.hpp"
#include "loclang.hpp"


#define PATH_NET_TUN "/dev/net/tun"


typedef struct mac_s {
	uint8_t mac[ETH_ALEN];
} mac_t;


class tap {
public:
	tap(std::string name) {
	struct ifreq ifr = { 0 };
	int fd, err;

	tapname = name;

	ifr.ifr_flags = IFF_TAP | IFF_NO_PI | IFF_ATTACH_QUEUE;
	tapname.copy(ifr.ifr_name, IFNAMSIZ);

	LSW{
		LS(ru, DBG_PRN("ОткрытиеTAP(%s)\n",ifr.ifr_name); )
		LD(en, DBG_PRN("TAPOPEN(%s)\n",ifr.ifr_name); )
	}

	if( (fd = open(PATH_NET_TUN, O_RDWR)) < 0 ) {
		LSW{
			LS(ru, DBG_ERR("ОШИБКА: Открытия " PATH_NET_TUN); )
			LD(en, DBG_ERR("ERROR: Opening " PATH_NET_TUN); )
		}
		exit(1);
	}

	if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
		close(fd);
		LSW{
			LS(ru, DBG_ERR("ОШИБКА: открытия интерфейса '%s'/'%s' err=(%d)%s\n",ifr.ifr_name,tapname.c_str(),err,strerror(-err)); )
			LD(en, DBG_ERR("ERROR: open iface '%s'/'%s' err=(%d)%s\n",ifr.ifr_name,tapname.c_str(),err,strerror(-err)); )
		}
		exit(1);
	}

	//Set TAP Link UP
	setflags( tapname.c_str(), IFF_UP | IFF_RUNNING, 1);

	LSW{
		LS(ru, DBG_PRN("открыт TAP имя = '%s' флаги=%X\n",ifr.ifr_name, ifr.ifr_ifru.ifru_flags & IFF_UP); )
		LD(en, DBG_PRN("open TAP name = '%s' flags=%X\n",ifr.ifr_name, ifr.ifr_ifru.ifru_flags & IFF_UP); )
	}

	dev_fd = fd;
}

	~tap() {
	LSW{
		LS(ru, DBG_PRN("ЗакрытиеTAP(%s)\n",tapname.c_str()); )
		LD(en, DBG_PRN("TAPCLOSE(%s)\n",tapname.c_str()); )
	}
	if(dev_fd > 0) close(dev_fd);
	dev_fd = -1;
}

	int add_filter_mac(mac_t &mac) {
	macs.push_back(mac);
	LSW{
		LS(ru, DBG_PRN("ДОБАВИТЬ(%s) фильтр MAC %02X:%02X:%02X:%02X:%02X:%02X\n",tapname.c_str(),mac.mac[0],mac.mac[1],mac.mac[2],mac.mac[3],mac.mac[4],mac.mac[5]); )
		LD(en, DBG_PRN("ADD(%s) filter MAC %02X:%02X:%02X:%02X:%02X:%02X\n",tapname.c_str(),mac.mac[0],mac.mac[1],mac.mac[2],mac.mac[3],mac.mac[4],mac.mac[5]); )
	}
	return(0);
}

	int send(packet_t &p) {
        unsigned int ret = 0;
        
        if(!macs.empty()) {
			for (auto & it : macs) {
				if(memcmp(&(p.eth->h_source), it.mac, ETH_ALEN) == 0) goto cont;
			}
			return(0);
        }
cont:
        ret = write(dev_fd, p.data, p.hdr.caplen);
        if(ret < 0) return(-1); // error
        if(ret == p.hdr.caplen) return(1);
        return(0);
}

	int recv(packet_t &p) {
	return(0);
}

private:
	void setflags(const char *name, unsigned short flags, int set) {
	struct ifreq ifr = {0};

	int skfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(skfd < 0) {
		LSW{
			LS(ru, DBG_ERR("ОШИБКА: открытыия сокета PF_INET\n"); )
			LD(en, DBG_ERR("ERROR: open socket PF_INET\n"); )
		}
	}

	strcpy(ifr.ifr_name, (char *)name);
	/* get original flags */
	if (ioctl(skfd, SIOCGIFFLAGS, (void *)&ifr) < 0) {
		close(skfd);
		LSW{
			LS(ru, DBG_ERR("ОШИБКА: сокета SIOCGIFFLAGS\n"); )
			LD(en, DBG_ERR("ERROR: socket SIOCGIFFLAGS\n"); )
		}
	}
	/* set new flags */
	if (set)
		ifr.ifr_flags |= flags;
	else
		ifr.ifr_flags &= ~flags & 0xffff;
	if (ioctl(skfd, SIOCSIFFLAGS, (void *)&ifr) < 0) {
		close(skfd);
		LSW{
			LS(ru, DBG_ERR("ОШИБКА: сокета SIOCGIFFLAGS\n"); )
			LD(en, DBG_ERR("ERROR: socket SIOCGIFFLAGS\n"); )
		}
	}
	close(skfd);
  }

	int dev_fd = -1;
	std::string tapname;
	std::list<mac_t> macs;
};


#endif /* _TAP_HPP_ */

