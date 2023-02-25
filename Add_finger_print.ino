#include <DFRobot_ID809.h>

#define COLLECT_NUMBER 3  //Fingerprint sampling times, can be set to 2-3

/*Use software serial when using UNO or NANO */
#if ((defined ARDUINO_AVR_UNO) || (defined ARDUINO_AVR_NANO))
    #include <SoftwareSerial.h>
    SoftwareSerial Serial1(2, 3);  //RX, TX
    #define FPSerial Serial1
#else
    #define FPSerial Serial1
#endif

DFRobot_ID809 fingerprint;
//String desc;

void setup(){
  /*Init print serial port */
  Serial.begin(9600);
  /*Init FPSerial*/
  FPSerial.begin(115200);
  /*Take FPSerial as communication port of the module*/
  fingerprint.begin(FPSerial);
  /*Wait for Serial to open*/
  while(!Serial);
  /*Test whether the device can communicate properly with mainboard 
    Return true or false
    */
  while(fingerprint.isConnected() == false){
    Serial.println("Communication with device failed, please check connection");
    /*Get error code information*/
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
    delay(1000);
  }
}

uint8_t ID,i,ret;

void loop(){
  /*Get an unregistered ID for saving fingerprint 
    Return ID when succeeded 
    Return ERR_ID809 if failed 
   */
  if((ID = fingerprint.getEmptyID()) == ERR_ID809){
    while(1){
      /*Get error code information*/
      //desc = fingerprint.getErrorDescription();
      //Serial.println(desc);
      delay(1000);
    }
  }
  Serial.print("unresistered ID,ID=");
  Serial.println(ID);
  i = 0;   //Clear sampling times 
  /*Fingerprint sampling 3 times*/
  while(i < COLLECT_NUMBER){
    
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eBreathing, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
    Serial.print("The fingerprint sampling of the");
    Serial.print(i+1);
    Serial.println("(th) is being taken");
    Serial.println("Please press down your finger");
    /*Capture fingerprint image, 10s idle timeout, if timeout=0,Disable  the collection timeout function
      IF succeeded, return 0, otherwise, return ERR_ID809
     */
    if((fingerprint.collectionFingerprint(/*timeout = */10)) != ERR_ID809){
      /*Set fingerprint LED ring to quick blink in yellow 3 times */
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3);
      Serial.println("Sampling succeeds");
      i++;   //Sampling times +1
    }
    else{
      Serial.println("Sampling failed");
      /*Get error code information*/
      //desc = fingerprint.getErrorDescription();
      //Serial.println(desc);
    }
    Serial.println("Please release your finger");
    /*Wait for finger to release 
      Return 1 when finger is detected, otherwise return 0 
     */
    while(fingerprint.detectFinger());
  }

  /*Save fingerprint in an unregistered ID */
  if(fingerprint.storeFingerprint(/*Empty ID = */ID) != ERR_ID809){
    Serial.print("Saving succeed，ID=");
    Serial.println(ID);
    Serial.println("-----------------------------");
    /*Set fingerprint LED ring to always ON in green */
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
    delay(1000);
    /*Turn off fingerprint LED ring */
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eNormalClose, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
    delay(1000);
  }else{
    Serial.println("Saving failed");
    /*Get error code information*/
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
  }
}
