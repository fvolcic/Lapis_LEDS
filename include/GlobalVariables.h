#include <MQTT.h>
#include <WiFi.h>

//These are variables for the clock and the data pin of the led strip
#ifndef LED_IOVALUES_H
#define LED_IOVALUES_H

#define DATAPIN 14
#define CLOCKPIN 12

#endif

//These are values for determining which core to use and where.
#ifndef CORE_VALUES_H
#define CORE_VALUES_H

#define LED_CORE 1
#define ALT_CORE 0

#endif

//global variables needed for MQTT
namespace MQTT_GLOBALS{
  
  extern MQTTClient client; 
  extern String MQTTChannel;

  extern char MQTT_Server_IP_Address[32]; 
  
}

//global variables needed for wifi
namespace WIFI_GLOBALS{

  extern WiFiClient net; 

  //Username and password for Wifi
  extern char * ssid; 
  extern char * pass; 
}

//global variables needed for the led strip
namespace LEDSTRIP_GLOBALS{
  extern const int led_count; 
}
