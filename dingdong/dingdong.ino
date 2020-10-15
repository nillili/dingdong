#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
SimpleTimer timer;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YJIRLSZpqVTjMKhw0e6jHiS0m_L_IPyF";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "neozi3";
char pass[] = "6259956";


int inputPin = 13;
int howmany  = 500;


BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  howmany = param.asInt(); 

  Serial.print("receive:");
  Serial.println(howmany);
}


void setup()
{
  // Debug console
//  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(inputPin, INPUT);
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, sendSensor);

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
    // 내용
Serial.println("------------------>1");  
//  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
//  // but actually the LED is on; this is because
//  // it is active low on the ESP-01)
//Serial.println("------------------>2");  
//  delay(howmany);                      // Wait for a second
//Serial.println("------------------>3");  
//  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
//Serial.println("------------------>4");  
//  delay(howmany);    


  int val = digitalRead(inputPin);
  Serial.println(val);
  if(val == HIGH){
    Serial.println("감지");      
    Blynk.virtualWrite(V2, 1);
  }else{
    Blynk.virtualWrite(V2, 0);    
  }

  
}
