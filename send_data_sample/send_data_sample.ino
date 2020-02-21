#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(5, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "saurabh";       // WiFi SSID
String myPWD = "password"; // WiFi Password
String myAPI = "UQ9HZSO6SW17FR4Q";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field2"; 
int sendVal=0;


void setup()
{
  Serial.begin(9600);
  espSerial.begin(115200);
  delay(7000);
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  while(!espSerial.find("OK")) 
  {          
      //Wait for connection
  }
  delay(1000);
  
}

  void loop()
  {
    /* Here, I'm using the function random(range) to send a random value to the 
     ThingSpeak API. You can change this value to any sensor data
     so that the API will show the sensor data  
    */
    delay(2000);
    sendVal = random(1000); // Send a random number between 1 and 1000
    String sendData = "GET https://api.thingspeak.com/update?api_key="+ myAPI +"&"+ myFIELD +"="+String(sendVal);
    //espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=" +String(sendData.length()+4),1000,DEBUG);
    delay(2000);  
//    if(!espSerial.find(">"))
//    Serial.print("ERROR");
    Serial.print("AT Command ==> ");
  Serial.print(sendData);
  Serial.println("     "); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sendVal);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(15000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  debug=true;
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  delay(3000);
  long int time = millis();
//  while ( (time + timeout) > millis())
//  {
//    while (espSerial.available())
//    {
//      char c = espSerial.read();
//      response += c;
//    }
//  }
  //if(espSerial.find("Error")){
    //Serial.println("Error");}
  
//  if (debug)
//  {
//  Serial.print("RESPONSE========>");
//    Serial.println(response);
//  }
return response;
  }

