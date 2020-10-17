#define BLYNK_PRINT Serial

#include "userinfo.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
SimpleTimer timer;
WidgetRTC rtc;

extern const char auth[];
extern const char ssid[];
extern const char pass[];


int inputPin = 13;
int howmany  = 500;
int is_touch = 0;
int scase = 1;//  상황 1:출근, 2:퇴근

String currentDate = "";    
String currentTime = "";

// 지연시간 설정
unsigned long previousMillis  = 0;
unsigned long currentMillis   = 0;
int interval=1000;
int delay_time = -1;     // 지연시간(분)

WidgetTerminal terminal(V3);


BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  howmany = param.asInt(); 

  Serial.print("receive:");
  Serial.println(howmany);
}

BLYNK_WRITE(V5) 
{
  switch(param.asInt())
  {
    case 1:
      Serial.println("case1");  
      scase = 1;
      break;
    case 2:
      Serial.println("case2");  
      scase = 2;
      break;      
    default:
      scase = 1;
      
  }
}
BLYNK_WRITE(V9) 
{
 terminal.clear();
}

void setup()
{
  // Debug console
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(inputPin, INPUT);
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, sendSensor);


  rtc.begin();            // 위젯 시간
  
  Serial.begin(9600);
}

BLYNK_CONNECTED() 
{
  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();
}


void loop()
{
  if (Blynk.connected())
  {
    Blynk.run();
    timer.run();
  }else
  {
    Serial.print("Blynk Server not connected!!!");    
  }
}


void sendSensor()
{
  // 지연 시간동안 들어 가지 못한다
  if((unsigned long)(millis() - previousMillis) >= interval)
  {
      // 내용
    Serial.println("------------------>");  
  
    int val = digitalRead(inputPin);
    Serial.println(val);
    if(val == HIGH){
      Serial.println("감지");    
      is_touch = 1;
      digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
      Blynk.virtualWrite(V2, 1);

      currentDate = String(year()) + "/" + month() + "/"+ day() + " " ;    
      currentTime = String(hour()) + ":" + minute() + ":" + second();    
      
      if(scase == 1)
      {
        Blynk.notify("alert - 딩동");
      }else if(scase == 2)
      {
        Blynk.email("ds1lph@gmail.com",
        "blynk 알림 - dingdong - "+currentDate + currentTime,
        "접근 확인"+currentDate + currentTime);        
      }
      
      terminal.println("Detect:" +currentDate + currentTime);
      terminal.flush();      
      snapshotTime(5000);  // 5초 얼음
      
    }else{
      if(is_touch == 1)
      {
        is_touch = 0;
        Blynk.virtualWrite(V2, 0);    
        digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
      }
    }
  }
  
}
