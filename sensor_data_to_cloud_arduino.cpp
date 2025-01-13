
//sensor data using Arduino UNO 
#include <SoftwareSerial.h> 
#define RX 2 
#define TX 3 
#include "DHT.h" 
#define DHTPIN 6    
#define DHTTYPE DHT22  
 
DHT dht(DHTPIN, DHTTYPE); 
 
String AP = "AndroidAP_7822";        
String PASS = "485088c2825f";  
String API = "DRMINLS2DN88G1EZ";  
String HOST = "api.thingspeak.com"; 
String PORT = "80"; 
String field1 = "field1"; 
String field2 = "field2"; 
int countTrueCommand; 
int countTimeCommand;  
boolean found = false;  
float TempSensor = 1; 
float HumidSensor = 1; 
SoftwareSerial esp8266(RX,TX);  
  
   
void setup() { 
  Serial.begin(9600); 
  esp8266.begin(9600); 
  dht.begin(); 
  delay(1000); 
  sendCommand("AT",5,"OK"); 
  sendCommand("AT+CWMODE=1",5,"OK"); 
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK"); 
} 
 
void loop() { 
 TempSensor = getTempData(); 
 HumidSensor = getHumidData(); 
 String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+String(TempSensor)+"&"+ 
field2 +"="+String(HumidSensor); 
sendCommand("AT+CIPMUX=1",5,"OK"); 
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK"); 
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">"); 
 esp8266.println(getData);delay(1500);countTrueCommand++; 
 sendCommand("AT+CIPCLOSE=0",5,"OK"); 
} 
 
float getTempData(){ 
    delay(2000); 
    float t = dht.readTemperature(); 
    if (isnan(t)) { 
      Serial.println("Failed to read from DHT sensor!"); 
      return 0; 
    } 
    return t;  
} 
float getHumidData(){ 
    delay(2000); 
    float h = dht.readHumidity(); 
    if (isnan(h)) { 
        Serial.println("Failed to read from DHT sensor!"); 
        return 0; 
    } 
    return h;  
} 
 
void sendCommand(String command, int maxTime, char readReplay[]) { 
  Serial.print(countTrueCommand); 
  Serial.print(". at command => "); 
  Serial.print(command); 
  Serial.print(" "); 
  while(countTimeCommand < (maxTime*1)) 
  { 
    esp8266.println(command);//at+cipsend 
    if(esp8266.find(readReplay))//ok 
    { 
      found = true; 
      break; 
    } 
   
    countTimeCommand++; 
  } 
   
  if(found == true) 
  { 
    Serial.println("OK"); 
    countTrueCommand++; 
    countTimeCommand = 0; 
  } 
   
  if(found == false) 
  { 
    Serial.println("Fail"); 
    countTrueCommand = 0; 
    countTimeCommand = 0; 
  } 
   
  found = false; 
 } 
