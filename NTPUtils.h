#ifndef NTPUtils_h
#define NTPUtils_h

#include "Arduino.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

#define NTP_PACKET_SIZE 48
#define SEVENTY_YEARS 2208988800UL

struct timeOfDay {
	int hour;
	int minute;
	int second;
};

class NTPUtils {
public:
	NTPUtils();
	NTPUtils(int offset);
	timeOfDay getTimeOfDay(EthernetUDP &udp, IPAddress &timeServer);
private:
	unsigned long sendNTPpacket(EthernetUDP &udp, IPAddress &address);
        uint8_t packetBuffer[ NTP_PACKET_SIZE ];
	int offset;
};

#endif
