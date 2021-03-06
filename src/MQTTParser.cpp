//This is the class for the MQTT Parser and MQTT Mangager
#include "MQTTParser.h"
#include <MQTT.h>
#include <ArduinoJson.h>
#include "ActionExecuter.h"
#include "LedAction.h"
#include "GlobalVariables.h"

#ifndef MQTTParserGaurd
#define MQTTParserGaurd

#include "SolidColorAction.cpp"
#include "Gradient.cpp"
#include "GradientMoving.cpp"
#include "cstrcmp.h"
#include "LEDPoint.cpp"
#include "SolidColorTransition.cpp"
#include "uninitilizedData.cpp"
#include "OTA_Updater.h"
#include "BrightnessGradient.cpp"
#include "SubAction.cpp"
#include "StripCopyShift.cpp"
#include "DynamicLedShift.cpp"
#include "BrightnessRandomizer.cpp"
#include "MirrorAction.cpp"
#include "LEDSerial.cpp"
#include "SnapshotAction.cpp"
#include "LEDTinting.cpp"

extern void update_firmware();



//The constructor
MQTTParser::MQTTParser(int led_count) : led_count(led_count), callActionKill(false), actionKillIndex(0), killAllActions(0), callEditAction(false), editActionIndex(0) {
  actionAvailable = false;
  MQTTMessage[117] = '\0'; //Makes sure that there is a last character in the allocated c string array
  int_msgLength = 0;
}



//MESSAGE FORMAT
// 3 BYTES - LENGTH OF MESSAGE
// 4 BYTES - ACTION KEY
// 116 BYTES - MESSAGE INFORMATION
// 2 BYTES - MESSAGE END (##)
// EXAMPLE MESSAGE - 6E00010001009SOLI*********##

void MQTTParser::set_led_count(int led_count) {
  this->led_count = led_count;
}

//This is what parses the MQTT message.
//This is for parsing default style messages for this device. These messages are ultralight weight
void MQTTParser::parseMessageNormal(String &message) {

  callActionKill = false;
  killAllActions = false;
  callEditAction = false;

  uint16_t index = 0;
  int messageLength = message.length();

  if (message.length() <= 128) {
    for (const char * msgPtr = message.c_str(); *msgPtr; ++msgPtr) {
      fullMessage[index] = *msgPtr;
      ++index;
    }

    fullMessage[index] = '\0';
  }

  char * msgPtr = fullMessage;

  for (int i = 0; i < messageLength; i++) {

    //Store the message length
    if (i >= 0 && i < 3) {
      msgLength[i] = *msgPtr;
    }

    if (i == 3) {
      msgLength[3] = '\0';// end the first message string
      int_msgLength = atoi(msgLength);
    }


    if (i >= 3 && i < 7) {
      actionKey[i - 3] = *msgPtr;
    }

    if (i == 7)
      actionKey[4] = '\0';

    if (i >= 7 && i < 7 + int_msgLength) {
      MQTTMessage[i - 7] = *msgPtr;
    }

    if (i == (6 + int_msgLength))
      MQTTMessage[int_msgLength] = '\0';

    ++msgPtr;
  }



}

/*
//This formulates an action after parse message has been called.
//REQUIRES: LED Count
//EFFECTS: creates an action that the action pointer will point to.
void MQTTParser::createAction(const uint8_t led_count) {

  Serial.println("Create Action Called");

  //Delete the action if the current stored action was never used.
  if (actionAvailable)
    delete action;

  static const char SolidAction[5] = "SOLI";
  static const char GradientAction[5] = "GRAD";
  static const char MovingGradientAction[5] = "MGRD";
  static const char ClearAction[5] = "CLEA";
  static const char PointAction[5] = "PONT";
  static const char SolidColorTransition[5] = "STRN";
  static const char Brightness[5] = "BRIG";
  static const char uninitialized[5] = "UNIT";
  static const char OTA_Update[] = "UPDA";
  static const char BGradAction[] = "BGRD";
  static const char SubActions[] = "SACT";
  static const char ShiftActionKey[] = "SHFT";
  static const char GetAllActions[] = "AGET";
  static const char DynamicShiftActionKey[] = "DSHF";
  static const char RandomBAction[] = "RBRI";
  static const char MirrorActionKey[] = "MIRR";
  static const char LEDSerialKey[] = "SERI";
  static const char SnapshotKey[] = "SNAP";
  static const char TintKey[] = "TINT";

  //This detemines what key to use

  Serial.println("Running cstrcmp");

  if (cstrcmp(SolidAction, actionKey)) {
    action = new SolidColor(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("SOLID COLOR");
  }

  else  if (cstrcmp(GradientAction, actionKey)) {
    action = new Gradient(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("GRADIENT");
  }
  else  if (cstrcmp(MirrorActionKey, actionKey)) {
    action = new MirrorAction(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("GRADIENT");
  }
  else  if (cstrcmp(ShiftActionKey, actionKey)) {
    action = new ShiftAction(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("Shift");
  }

  else  if (cstrcmp(MovingGradientAction, actionKey)) {
    action = new MovingGradient(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("MOVING GRADIENT");
  }
  else  if (cstrcmp(TintKey, actionKey)) {
    action = new TintRandomizer(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("LED TINT");
  }
  else if (cstrcmp(BGradAction, actionKey)) {
    Serial.println("Brightness Gradient");
    action = new BrightnessGradient(led_count, MQTTMessage);
    actionAvailable = true;
  }

  else  if (cstrcmp(uninitialized, actionKey)) {
    action = new UninitilizedData(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("MOVING GRADIENT");

  } else  if (cstrcmp(RandomBAction, actionKey)) {
    action = new BrightnessRandomizer(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("Random Brightnesses");
  }

  else  if (cstrcmp(PointAction, actionKey)) {
    action = new LEDPoint(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("LEDPoint");
  }

  else  if (cstrcmp(ClearAction, actionKey)) {
    Serial.println("CLEAR");
    killAllActions = true;
  }

  else  if (cstrcmp(SolidColorTransition, actionKey)) {
    action = new SolidTransition(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("Solid Transition");

  }
  else  if (cstrcmp(DynamicShiftActionKey, actionKey)) {
    action = new DynamicShiftAction(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("Dynamic Shift");

  }
  else  if (cstrcmp(SubActions, actionKey)) {
    action = new SubAction(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("SubAction called");
  }

  else  if (cstrcmp(Brightness, actionKey)) {
    actionAvailable = false;
    brightness = atoi(MQTTMessage);
    brightnessAvailable = true;
  } else  if (cstrcmp(LEDSerialKey, actionKey)) {
    action = new LEDSerial(led_count, MQTTMessage);
    actionAvailable = true;
    Serial.println("LEDSerial");
  }
  else if (cstrcmp(SnapshotKey, actionKey)) {
    actionAvailable = true;
    action = new SnapshotShiftAction(led_count, MQTTMessage);
    Serial.println("Snapshot");
  }
  else if (cstrcmp(OTA_Update, actionKey)) {
    actionAvailable = false;
    xTaskCreatePinnedToCore(update_firmware, "UPDATE", 10000, NULL, 1, NULL, xPortGetCoreID());
    vTaskDelete(NULL);
  } else if (cstrcmp(GetAllActions, actionKey)) {
    Serial.println("Request for listing all actions");

    actionAvailable = false;
    sendActionsToMqtt = true;

    int messageLength = 0;
    int iter = 0;
    Serial.print("Sending to ");
    while (iter < 129 && MQTTMessage[iter] != '~') {
      Serial.print(MQTTMessage[iter]);
      ++iter;
    }

    Serial.println("");

    MQTTMessage[iter] = '\0';

    delete this->topic;

    topic = new char[iter + 1];

    for (int i = 0; i <= iter; ++i) {
      topic[i] = MQTTMessage[i];
      Serial.print(topic[i]);
    }
    Serial.println ("output finished");
  }
  else {
    Serial.println("Finished Running cstrcmp");
    actionAvailable = false;
  }

}
*/

//This function determines what style the message is,
//and from that then passes the message to the proper
//message parser.
void MQTTParser::parseMessage(String message) {
  int letterIndex = 0;
  while (message.charAt(letterIndex) == ' ')
    ++letterIndex;

  //If the message was a json style message,
  //Then parse the message here, and return
  if (message.charAt(letterIndex) == '{') {
    this->parseJSONMessage(message);
    return;
  }

  //if the message was a normal style message,
  //Then parse the message here.
  this->parseMessageNormal(message);


}

//this is for parsing json style messages

//This works by using the information that is in the JSON style message and returning it as something
//that can be read using the normal protocol.
//The best way to determine the type of message that has been received (JSON or normal),
//it is easiest to just check if the received message starts with '{'
//if it does not contain that character, then attempt to parse message normally.
void MQTTParser::parseJSONMessage(String message) {

}


//This is run once the message has been parsed.
//This will add actions if there are available actions.
//This function takes in the manager pointer, and
//With that manages the led actions.
void MQTTParser::manageLEDs(ActionExecuter * manager) {

  this->createAction(led_count);

  if (this->getActionAvailable())
    manager->add_LED_action(this->getCreatedAction());

  if (this->getKillAction())
    manager->remove_LED_action(this->killActionIndex());

  if (this->getEditAction())
    manager->edit_LED_action(this->getEditIndex(), this->getCreatedAction());

  if (this->getClearActions())
    manager->clear_LED_actions();

  if (this->getBrightnessAvailable())
    manager->setBrightness(this->getBrightness());

  if (this->getSendMQTTActionList())
    manager->publishActionStatesMQTT(MQTT_GLOBALS::client, this->topic);
}


//Checks if there is a new brightness
bool MQTTParser::getBrightnessAvailable() {
  return brightnessAvailable;
}

//Return the brightness
int MQTTParser::getBrightness() {
  brightnessAvailable = false;
  return brightness;
}

//This returns the action after an action was created
//REQUIRES: create action has already been called, and there is an available action waiting
//EFFECTS: Returns the current action that is being pointed to by the action pointer.
LED_Action * MQTTParser::getCreatedAction() {

  if (actionAvailable) {
    Serial.println("Returing Created Action");
    actionAvailable = false;
    return action;
  }
  else
    return nullptr;

}

//getter function for if there is an available action.
bool MQTTParser::getActionAvailable() {
  return actionAvailable;
}

//getter for if there is an action that is to be edited
bool MQTTParser::getEditAction() {
  return callEditAction;
}

//Returns the index that is to be edited
int MQTTParser::getEditIndex() {
  return editActionIndex;
}

//getter for if there was a command to clear all current running device actions
bool MQTTParser::getClearActions() {
  return killAllActions;
}

bool MQTTParser::getKillAction() {
  return callActionKill;
}

int MQTTParser::killActionIndex() {
  return actionKillIndex;
}

bool MQTTParser::getSendMQTTActionList() {
  if (sendActionsToMqtt) {
    sendActionsToMqtt = false;
    return true;
  }
  return false;
}

#endif
