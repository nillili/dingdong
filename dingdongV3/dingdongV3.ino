#define BLYNK_PRINT Serial

#include "userinfo.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <NewPing.h>
SimpleTimer timer;
WidgetRTC rtc;

extern const char auth[];
extern const char ssid[];
extern const char pass[];
extern const char auth_listen[];


//const int trigPin = 13;
//const int echoPin = 12;

const int TRIGGER_PIN  = 13;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int ECHO_PIN     = 12;  // Arduino pin tied to echo pin on the ultrasonic sensor.
const int MAX_DISTANCE = 400; // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.



//int inputPin = 13;
int howmany  = 500;
int is_touch = 0;
int scase = 1;//  상황 1:출근, 2:퇴근

String currentDate = "";    
String currentTime = "";

// 지연시간 설정
unsigned long previousMillis  = 0;
unsigned long currentMillis   = 0;
int interval=1000;
//int delay_time = -1;     // 지연시간(분)
int how_long = 0;
long duration;
int  distance;
int  cnt_detected = 0;
int  wait_time = 0;
int  sensitive = 1;     // 민감도(카운트)
int  delay_time = 0;

WidgetTerminal terminal(V3);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


/**
 * 민감도(속도)
 * 멈춤시간(delay)를 조정해서 초당 반복회수를 결정
 */
BLYNK_WRITE(V8) //Button Widget is writing to pin V1
{
  delay_time = param.asInt(); 

  Serial.print("receive delay:");
  Serial.println(delay_time);
}


/**
 * 민감도(카운트)
 * 몇번만에 검출할것인지 결정
 */
BLYNK_WRITE(V7) //Button Widget is writing to pin V1
{
  sensitive = param.asInt(); 

  Serial.print("receive wait:");
  Serial.println(sensitive);
}

BLYNK_WRITE(V6) //Button Widget is writing to pin V1
{
  wait_time = param.asInt(); 

  Serial.print("receive wait:");
  Serial.println(wait_time);
}


BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  howmany = param.asInt(); 

  Serial.print("receive:");
  Serial.println(howmany);
}

/**
 * 감지 거리 
 */
BLYNK_WRITE(V4) 
{
  how_long = param.asInt(); 

  Serial.print("receive:");
  Serial.println(how_long);
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
//  pinMode(inputPin, INPUT);

//  pinMode(trigPin, OUTPUT);
//  pinMode(echoPin, INPUT);


  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(20L, sendSensor);


  rtc.begin();            // 위젯 시간
  
  Serial.begin(115200);
}

WidgetBridge bridge(V0);

BLYNK_CONNECTED() 
{
  bridge.setAuthToken(auth_listen);
  Blynk.syncAll();
}




int cnt_error = 0;

void loop()
{
  if (Blynk.connected())
  {
    cnt_error= 0;
    Blynk.run();
    timer.run();
  }else
  {
    Serial.print("Blynk Server not connected!!!");    
    cnt_error++;
    if(cnt_error > 30)
    {
      cnt_error= 0;
      delay(1000);
      ESP.reset();
      delay(1000);      
    }
  }
}


void sendSensor()
{
  delay(delay_time);
  // 지연 시간동안 들어 가지 못한다
  if((unsigned long)(millis() - previousMillis) >= interval)
  {
      // 내용
    Serial.println("------------------>");  

    delay(50);
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
  
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
  
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration*0.034/2; //Cm
  
  
    Serial.print("Distance : ");
    Serial.print(distance);
    Serial.println("CM");
  

//    distance = sonar.ping()/US_ROUNDTRIP_CM; 

    if(distance != 0){
      Serial.println(distance);    
      if(distance < how_long){
        cnt_detected++;
      }else{
        cnt_detected = 0;
      }
      if(distance < how_long && cnt_detected >= sensitive){
        Serial.println("감지");    
        is_touch = 1;
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
        Blynk.virtualWrite(V2, 1);
  
        currentDate = String(year()) + "/" + month() + "/"+ day() + " " ;    
        currentTime = String(hour()) + ":" + minute() + ":" + second();    
        
        if(scase == 1)
        {
          Blynk.notify("alert - 딩동");
          bridge.virtualWrite(V30,1);
        }else if(scase == 2)
        {
          Blynk.email("ds1lph@gmail.com",
          "blynk 알림 - dingdong - "+currentDate + currentTime,
          "접근 확인"+currentDate + currentTime);        
        }
        
        terminal.println("Detect:" +currentDate + currentTime);
        terminal.flush();      
        snapshotTime(wait_time*1000);  // 5초 얼음
        
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
  
}
