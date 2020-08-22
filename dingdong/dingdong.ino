#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
SimpleTimer timer;


// You should get Auth Token in the Blynk App.
// 개발자
char auth[] = "YJIRLSZpqVTjMKhw0e6jHiS0m_L_IPyF";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "jjangsvc";
char pass[] = "123456789a";

int howmany = 500;

BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  howmany = param.asInt(); 
//  howmany = param.asLong(); 
  Serial.print("receive:");
  Serial.println(howmany);
}


void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, sendSensor);
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
  }
}


void sendSensor()
{
    // 내용
Serial.println("------------------>1");  
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
Serial.println("------------------>2");  
  delay(howmany);                      // Wait for a second
Serial.println("------------------>3");  
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
Serial.println("------------------>4");  
  delay(howmany);    
}
