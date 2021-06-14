#include <WiFi.h>
#include <MQTT.h>
#include <APA102.h>
#include "MQTTParser.h"
#include "ActionExecuter.h"
#include "GlobalVariables.h"

#define LED_BUILTIN 2

int brightness = 8;

TaskHandle_t ledTask;
TaskHandle_t WiFiTask;


void messageReceived(String &topic, String &payload); 
void runWiFiTasks(void * parameter); 
void runLEDs(void * parameter); 
void connect();

//PIN 14: Data Pin
//PIN 12: Clock Pin
APA102<DATAPIN, CLOCKPIN> ledStrip;

//These are the units that will manage the data for device
ActionExecuter manager = ActionExecuter(ledStrip, 240);
MQTTParser parser = MQTTParser(LEDSTRIP_GLOBALS::led_count);


void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_GLOBALS::ssid, WIFI_GLOBALS::pass);

  MQTT_GLOBALS::client.begin(/*MQTT_GLOBALS::MQTT_Server_IP_Address*/"10.0.0.178", WIFI_GLOBALS::net);
  MQTT_GLOBALS::client.onMessage(messageReceived);

  manager.setNumLEDs(LEDSTRIP_GLOBALS::led_count); 

  pinMode(LED_BUILTIN, OUTPUT);

  //xTaskCreatePinnedToCore(updater, "OTA_Update", NULL, 
  
  connect();

  xTaskCreatePinnedToCore(runLEDs, "LED TASK", 10000, NULL, 0, &ledTask, LED_CORE); //Pin to core 1
  xTaskCreatePinnedToCore(runWiFiTasks, "WiFi TASK", 10000, NULL, 0, &WiFiTask, ALT_CORE); //Pin to core 0
  
}
//delete the loop function.
void loop(){vTaskDelete(NULL);}

//This is linked to the MQTT Client. This is called whenever a new message is received on a subscribed topic.
void messageReceived(String &topic, String &payload) {
  Serial.println("MESSAGE RECEIVED. PARSE STARTING");
  parser.parseMessage(payload);
  parser.manageLEDs(&manager);
}

//This connects the WiFi along with the MQTT server
void connect() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("checking wifi...");
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  
  while (!MQTT_GLOBALS::client.connect("ESP2!", "try2", "try2")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  //Subscribe to the name of the device
  MQTT_GLOBALS::client.subscribe(MQTT_GLOBALS::MQTTChannel);
  digitalWrite(LED_BUILTIN, HIGH);
}

//This is for running the LEDs on a seperate core.
void runLEDs(void * parameter) {
  for (;;) {
    manager.executeActions();
  }
}

void runWiFiTasks(void * parameter) {
  for (;;) {
    
    //Ensure the device is connected.
    if (!MQTT_GLOBALS::client.connected())
      connect();

    MQTT_GLOBALS::client.loop();
 
    vTaskDelay(10.0 / portTICK_PERIOD_MS); //This will delay the run WiFi task for 10 milliseconds.
    //This ensures that the task is delayed, but the core is not in use.

  }

}
