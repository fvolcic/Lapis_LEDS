#ACTIONKEY:UPDA
#CLASSNAME:OTA_UPDATER
OTA_Updater.h
#enddependencies
{
    actionAvailable = false;
    xTaskCreatePinnedToCore(update_firmware, "UPDATE", 10000, NULL, 1, NULL, xPortGetCoreID());
    vTaskDelete(NULL);
}#funcend

#ACTIONKEY:BRIG
#CLASSNAME:BRIGHTNESS_UPDATER
#enddependencies
{
    actionAvailable = false;
    brightness = atoi(MQTTMessage);
    brightnessAvailable = true;
}#funcend

#ACTIONKEY:CLEA
#CLASSNAME:CLEAR_ACTIONS
#enddependencies
{
   killAllActions = true; 
}#funcend
#endfile