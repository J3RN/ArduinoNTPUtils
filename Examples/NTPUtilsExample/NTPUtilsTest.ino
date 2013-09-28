#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <NTPUtils.h>

EthernetUDP udp;
IPAddress timeServer(64, 90, 182, 55);
NTPUtils utils(-4);

unsigned int localPort = 8888;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup() {
  Serial.begin(9600);
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet");
    while (true);
  }
  
  udp.begin(localPort);
}

void loop() {
  timeOfDay current = utils.getTimeOfDay(udp, timeServer);
  
  String hour, minute, second;
  
  hour = String(current.hour);
  minute = String(current.minute);
  second = String(current.second);
  
  if (current.hour < 10) {
    hour = "0" + hour;
  } 
  
  if (current.minute < 10) {
    minute = "0" + minute;
  }
  
  if (current.second < 10) {
    second = "0" + second;
  }
  
  Serial.println(hour + ":" + minute + ":" + second);
  
  delay(10000);
}
