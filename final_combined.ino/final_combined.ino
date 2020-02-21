   
/***************WIFI_CODE****************/
#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(5, 3);   //Pin 5 and 3 act as TX and RX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "kumar";       // WiFi SSID
String myPWD = "password";       // WiFi Password
String myAPI = "UQ9HZSO6SW17FR4Q";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD; 
/***************WIFI_CODE****************/

double price=10;        //10rs per unit
double meter=0;
double cost=0;
long long int prevtime=0; 
double prevwatt=0;



/***************CUR_CODE****************/
const int sensor_IN = A2;
//use 185 for 5A, 100 for 20A Module and 66 for 30A Module
int mv_per_amp = 185;
double voltage = 0;
double v_rms = 0; 
double amps_rms = 0;
/***************CUR_CODE****************/

bool debug[3]={false,false,false};      //V,A,WIFI


void setup() {
  
  Serial.begin(9600);
  /***************CUR_CODE****************/
  pinMode(A2,INPUT);
  /***************CUR_CODE****************/

  
  /***************VOL_CODE****************/
  pinMode(A3,INPUT);
  /***************VOL_CODE****************/


/***************WIFI_CODE****************/
  espSerial.begin(115200);
  delay(1000);
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  while(!espSerial.find("OK")) 
  {//Wait for connection
    }
  delay(1000);
/***************WIFI_CODE****************/

}
void loop() {

/*********VOL_CODE**********/
  int x=analogRead(A3);
/* if(x<700&&X>500)
  {Serial.println(x);
  delay(100);}
*/
int y=(x*.380156);      //for 550peak=5vout(i.e1023),for 550peak=389rms,so,y=389/1023=0.380156*x
y+=17;
if(debug[0]){
Serial.print(" analog input :");
Serial.print(x);
Serial.print("   analog voltage :");
Serial.println(y);}
/*********VOL_CODE**********/

/***************CUR_CODE****************/
   voltage = getVPP();
    v_rms = (voltage) * 0.707;
    amps_rms = (v_rms * 1000)/mv_per_amp;
    amps_rms=amps_rms-0.3;
    amps_rms=amps_rms*1.414;
    if(debug[1])
    {Serial.print(amps_rms);
    Serial.println(" Amps RMS");}
/***************CUR_CODE****************/


double power=amps_rms*y;
//double total_pow;
double timeint=(millis()-prevtime)/1000;

//total_pow+=power; // calculate total power
//double avgamps=total_amps/timep; // average amps
//energy=(power*timep)/3600; Watt-sec is again convert to Watt-Hr by dividing 1hr(3600sec)
double energy=(power*timeint);      //in watt-sec
double average=(energy+prevwatt)/2;
prevwatt=energy;
energy=average*timeint;
energy=energy/(3600000);
meter+=energy;
prevtime=millis();
cost=meter*price;


/***************WIFI_CODE****************/
send_data(y,amps_rms,power,energy,cost);
/***************WIFI_CODE****************/
}


/***************CUR_CODE****************/
float getVPP(){
    float result;
    int read_value;             //value read from the sensor
    int max_value = 0;          // store max value here
    int min_value = 1024;       // store min value here

    uint32_t start_time = millis();
    //sample for 1 Sec
    while((millis()-start_time) < 1000) {
        read_value = analogRead(sensor_IN);
        //Serial.print(read_value);
        // see if you have a new max_value
        if (read_value > max_value){
            /*record the maximum sensor value*/
            max_value = read_value;
        }
        if (read_value < min_value){
            /*record the maximum sensor value*/
            min_value = read_value;
        }
    }
    if(debug[1]){
    Serial.print("MAX:");
    Serial.println(max_value);
    Serial.print("MIN:");
    Serial.println(min_value);}
    // Subtract min from max
    result = (((max_value - min_value) * 0.5)*5)/1024.0;
    return result;
 }
/***************CUR_CODE****************/

/***************WIFI_CODE****************/
 String espData(String command, const int timeout, boolean debug)
{
  debug=true;
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  delay(1000);
/*  long int time = millis();
/  while ( (time + timeout) > millis())
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
*/
return response;
  }
/***************WIFI_CODE****************/

/***************WIFI_CODE****************/
void send_data(double vol,double cur,double p,double m,double c)
  {
String sendData = "GET https://api.thingspeak.com/update?api_key="+ myAPI +"&field1="+String(vol)+"&field2="+String(cur)+"&field3="+String(p)+"&field4="+String(m)+"&field5="+String(c);
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
    //Serial.println(vol,cur,p,m,c);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(18000);
  }
