#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "GravityTDS.h"
#define ONE_WIRE_BUS 5
#define TdsSensorPin A0
#define pumpA 8
#define pumpB 9
GravityTDS gravityTds;
OneWire oneWire(ONE_WIRE_BUS);
SoftwareSerial mySerial(2,3); 
//membuat pin serial secara software, pin 2 RX, pin 3 TX
DallasTemperature sensors(&oneWire);
float temperature,tdsValue = 0;
float Celcius=0;
float Fahrenheit=0;
String suhu;
String data; //dipakai untuk data "minta"
char c;
void setup(void)
{
  Serial.begin(115200);
  mySerial.begin(115200);
  sensors.begin();
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  
  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  
  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  pinMode(pumpA, OUTPUT);
  pinMode(pumpB, OUTPUT);
  digitalWrite(pumpA, LOW);
  digitalWrite(pumpB, LOW);
}

void loop(void)
{ 
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  temperature = Celcius;  
  //add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  
  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate 
  tdsValue = gravityTds.getTdsValue();  // then get the value
  
  Serial.print(Celcius);
  Serial.print(" C  \n");
  Serial.print(Fahrenheit);
  Serial.print(" F  \n");
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  if(tdsValue < 800){
    Serial.println("Pompa A dan B menyala");
    digitalWrite(pumpA,HIGH);
    digitalWrite(pumpB,HIGH);
    delay(1000);
    digitalWrite(pumpA,LOW);
    digitalWrite(pumpB,LOW);
    delay(4000);
  }else{
    Serial.println("Pompa A dan B mati");
    digitalWrite(pumpA,LOW);
    digitalWrite(pumpB,LOW);
  }
  if(Celcius > 35){
    suhu="PERINGATAN, SUHU >35 C. Silahkan gelar atap";
    Serial.println(suhu);
  
  }else{
    suhu="suhu Normal";
    Serial.println(suhu);
  }
  //komunikasi ke esp
  while(mySerial.available()>0){
    c = mySerial.read(); 
    data += c;
  }
  data.trim();
  if (data.length()>0)
    Serial.println(data);
    if (data == "minta")
      float t = Celcius;
      float p = tdsValue;
      String m = suhu;
      mySerial.print(Celcius);
      mySerial.print('#');
      mySerial.print(tdsValue);
      mySerial.print('#');
      mySerial.print(suhu);
      data ="";
    
  delay(2000);
}
