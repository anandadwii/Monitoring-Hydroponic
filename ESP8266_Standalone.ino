
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"
#define BLYNK_TEMPLATE_ID "TMPLnujbyxjI"
#define BLYNK_DEVICE_NAME "Monitoring Hydroponics"
#define BLYNK_AUTH_TOKEN "zrwwEEHeI88O3WtR7hHdjkbYKKXeBFQ-"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

String sData; //menampung data dari arduino
String arrData[3]; //array digunakan menjadi wadah dari setiap data yang sudah dipecah dari arduino
int warn;

bool parsing = false; //memecah data 

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Ananda_Dwi";
char pass[] = "thecruelworldinyourhand";

void sendSensor(){

  while(Serial.available() >0 ){ //fungsi untuk menampung data dari arduino 
    char in = Serial.read();  //variabel ini akan membaca setiap data kemudian ditumpuk di sData
    sData += in;
  }
  sData.trim(); //datanya dipotong ketika serial available sudah selesai

  if(sData != ""){
    int index =0;
    for (int i=0;i<=sData.length();i++){
      char delimiter = '#';
      if(sData[i] != delimiter){
        arrData[index] += sData[i];
      }else{
        index++;
      }
    }
    if(index == 2){
      Blynk.virtualWrite(V3, arrData[0]);
      String wan = arrData[0];
      warn = wan.toInt();
      if(warn>40){
        Blynk.notify("Peringatan, Suhu >40 °C. Silakan gelar atap");
       }
      Blynk.virtualWrite(V4, arrData[1]);
      Blynk.virtualWrite(V5, arrData[2]);
      delay(20);
    }
    //arrData[0].toFloat();
    arrData[0]="";
    arrData[1]="";
    arrData[2]="";
  }
  sData="";

  //minta data dengan kata ya wkwk
  Serial.println("Minta");
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  //Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
  timer.setInterval(3000L,sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
