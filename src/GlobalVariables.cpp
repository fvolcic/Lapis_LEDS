#include "GlobalVariables.h"

//WIFI_GLOBALS
char * WIFI_GLOBALS::ssid = "NETGEAR24";//"NETGEAR24"; 
char * WIFI_GLOBALS::pass = "littlecartoon561"; 
WiFiClient WIFI_GLOBALS::net = WiFiClient(); 


//MQTT_GLOBALS
MQTTClient MQTT_GLOBALS::client = MQTTClient(); 
String MQTT_GLOBALS::MQTTChannel = "/barLights";
//String MQTT_GLOBALS::MQTT_Server_IP_Address = "192.168.1.142";


//LEDSTRIP Globals

const int LEDSTRIP_GLOBALS::led_count = 240; 
