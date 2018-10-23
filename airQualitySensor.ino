
#include <SoftwareSerial.h>
SoftwareSerial Serial1(2,3);

char *api_key="Y7PHLZONPAN2VVVC";     // Enter your Write API key from ThingSpeak
static char postUrl[150];
//int hp=0;
void httpGet(String ip, String path, int port=80);


 int measurePin = A5;
int ledPower = 12;
int piezoPin = 8;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;


void setup()
{
  Serial.begin(115200);
 // pinMode(trig, OUTPUT);
 // pinMode(echo, INPUT);
  pinMode(ledPower,OUTPUT); 


 Serial1.begin(115200);
 Serial.begin(115200);
 Serial.println("Connecting Wifi....");
 connect_wifi("AT",1000);
 connect_wifi("AT+CWMODE=1",1000);
 connect_wifi("AT+CWQAP",1000);  
 connect_wifi("AT+RST",5000);
 connect_wifi("AT+CWJAP=\"things\",\"qwerty9876\"",10000);
 Serial.println("Wifi Connected"); 
 delay(2000);
}

void loop()
{

  
digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured*(5.0/1024);
  dustDensity = 0.17*calcVoltage-0.1;

  if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }

  Serial.println("Raw Signal Value :");
  Serial.println(voMeasured);

  Serial.println("Voltage:");
  Serial.println(calcVoltage);

  Serial.println("Dust Density:");
  Serial.println(dustDensity);

  if(voMeasured>3000){
    Serial.println(" Very poor Air Quality ");
    tone(piezoPin, 1000, 500);
    delay(1000);
  }

  if(voMeasured>1050&&voMeasured<3000){
    Serial.println(" Poor Air Quality ");
    tone(piezoPin, 1000, 100);
    delay(1000);
    }  

    
  if(voMeasured>300&&voMeasured<1050){
    Serial.println(" fair Air Quality ");
    //tone(piezoPin, 1000, 500);
    //delay(1000);
    }
    
  if(voMeasured>150&&voMeasured<300){
    Serial.println(" very Good  Air Quality ");
    //tone(piezoPin, 1000, 500);
    //delay(1000);
    }

  if(voMeasured>0&&voMeasured<75){
    Serial.println(" Excellent Air Quality ");
    //tone(piezoPin, 1000, 500);
    //delay(1000);
    }


  
  /*long t = 0, h = 0;
  
  // Transmitting pulse
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Waiting for pulse
  t = pulseIn(echo, HIGH);
  
  // Calculating distance 
  h = t / 58;
 
  h = h - 6;  // offset correction
  hp = 50 - h;  // water height, 0 - 50 cm
  
 // hp = 2 * h;  // distance in %, 0-100 %
  Serial.print(hp);
  Serial.print("\n");
  delay(1000);*/





  char h[8]
   //esp8266.println("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80");
   dtostrf(voMeasured,5,3,h)
 sprintf(postUrl, "update?key=Y7PHLZONPAN2VVVC&field1=",api_key,h);
 httpGet("184.106.153.149", postUrl, 80);
}

//GET https://api.thingspeak.com/update?api_key=SIWOYBX26OXQ1WMS&field1=0

void httpGet(String ip, String path, int port)
{
  int resp;
  String atHttpGetCmd = "GET /"+path+" HTTP/1.0\r\n\r\n";
  String atTcpPortConnectCmd = "AT+CIPSTART=\"TCP\",\""+ip+"\","+port+"";
  connect_wifi(atTcpPortConnectCmd,1000);
  int len = atHttpGetCmd.length();
  String atSendCmd = "AT+CIPSEND=";
  atSendCmd+=len;
  connect_wifi(atSendCmd,1000);
  connect_wifi(atHttpGetCmd,1000);
}


void connect_wifi(String cmd, int t)
{
  int temp=0,i=0;
  while(1)
  {
    Serial.println(cmd);-
    Serial1.println(cmd); 
    while(Serial1.available())
    {
      if(Serial1.find("OK"))

      i=8;
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8)
  {
   Serial.println("OK");
     
  }
  else
  {
   Serial.println("Error");
  }
}

