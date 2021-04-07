#ifndef cstrcmpCPP
#define cstrcmpCPP

//#include <SoftwareSerial.h>
bool cstrcmp(const char  * c_str1,const char  * c_str2) {


  //Serial.println("Runnning cstrcmp while loop");
  while (*c_str1 && * c_str2) {
    if (*c_str1 != *c_str2)
      return false;

    //Serial.println("Increment");
    ++c_str1;
    ++c_str2;
  }

  // Serial.println("Left while loop");
  if ( (!*c_str1 && *c_str2) || (*c_str1 && !*c_str2))
    return false;



  return true;
}


#endif
