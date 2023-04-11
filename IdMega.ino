#include <DFRobot_ID809.h>


DFRobot_ID809 fingerprint;
//String desc;

void setup(){
  Serial.begin(9600);
  Serial1.begin(115200);
  fingerprint.begin(Serial1);
  while(!Serial);
  
  while(fingerprint.isConnected() == false){
    Serial.println("Communication with device failed, please check connection");
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
    delay(1000);
  }
}

void loop(){
  uint8_t ret = 0;
  fingerprint.ctrlLED( fingerprint.eBreathing, fingerprint.eLEDBlue,0);
  Serial.println("Please press down your finger");
  if((fingerprint.collectionFingerprint(0)) != ERR_ID809){
 fingerprint.ctrlLED( fingerprint.eFastBlink, fingerprint.eLEDYellow, 3);
    Serial.println("Capturing succeeds");
      Serial.println("Please release your finger");
    while(fingerprint.detectFinger());
    ret = fingerprint.search();
    //ret = fingerprint.verify(1);  
    if(ret != 0){
      fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDGreen, 0);
      Serial.print("Matching succeeds,ID=");
      Serial.println(ret);
    }else{
      fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDRed, 0);
      Serial.println("Matching fails");
    }
  }else{
    Serial.println("Capturing fails");
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
  }
  Serial.println("-----------------------------");
  delay(1000);
}