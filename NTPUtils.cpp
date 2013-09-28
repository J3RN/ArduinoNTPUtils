#include "NTPUtils.h"

NTPUtils::NTPUtils() {
	offset = 0;
}

NTPUtils::NTPUtils(int offset) {
	this->offset = offset;
}

timeOfDay NTPUtils::getTimeOfDay(EthernetUDP &udp, IPAddress &timeServer) {
	timeOfDay currentTime;	

	sendNTPpacket(udp, timeServer);

	while(!udp.parsePacket());

	udp.read(packetBuffer, NTP_PACKET_SIZE);

	unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
	unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

	unsigned long secsSince1900 = highWord << 16 | lowWord;

	unsigned long epoch = secsSince1900 - SEVENTY_YEARS;

	currentTime.hour = (epoch % 86400L) / 3600;
	currentTime.minute = (epoch  % 3600) / 60;
	currentTime.second = epoch % 60;

	currentTime.hour += offset;

	if (currentTime.hour < 0) {
		currentTime.hour = 23 - currentTime.hour;
	}

	return currentTime;
}

unsigned long NTPUtils::sendNTPpacket(EthernetUDP &udp, IPAddress &address) {
  // set all ints in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 ints of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp: 		   
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket(); 
}
