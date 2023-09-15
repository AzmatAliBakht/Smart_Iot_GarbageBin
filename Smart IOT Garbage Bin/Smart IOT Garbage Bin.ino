#define BLYNK_TEMPLATE_ID "TMPLx1xNV9WP"
#define BLYNK_DEVICE_NAME "MICROCEP"
#define BLYNK_AUTH_TOKEN "cYOPBez0mmaM3zSM-w5mkxSDTeBN3vFV"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

char auth[] = "cYOPBez0mmaM3zSM-w5mkxSDTeBN3vFV";

char ssid[] = "EL PLAN";

char pass[] = "tsjm1938";

 
const int trigPin = 12;
const int echoPin = 14;
const int button = D1;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034

long duration;
float distanceCm, percent_empty;
bool is_empty = true;
bool is_open = true;
Servo s1;

void Close_bin()
{
  if(is_open)
  {
    for(int pos=90;pos<=180;pos++)
     {
        s1.write(pos);
        delay(10);
     }
   is_open = false;
  }
}

void Open_bin()
{
  if(!is_open)
  {
    for(int pos=180;pos>=90;pos--)
     {
        s1.write(pos);
        delay(10);
     }
   is_open = true;
  }
}

void Check_button()
{
  if(digitalRead(button) == 1)
  {
    Open_bin();
    delay(5000);
  }
}

float Distance()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  float distanceinCm = duration * SOUND_VELOCITY/2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceinCm);

  return distanceinCm;
}

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Blynk.begin(auth, ssid, pass);
  s1.attach(0);
  Open_bin();
}

void loop() {
  percent_empty =((Distance()) / 11.0) * 100;
  Serial.print("Percent empty: ");
  Serial.println(percent_empty);
  Blynk.virtualWrite(V1, percent_empty);
  Serial.println(digitalRead(button));
  delay(200);
  if(percent_empty < 50)
  {
    is_empty = false;
    Close_bin();
    Serial.println("Sending notification");
    Blynk.logEvent("alert", "Bin_Update. Dustbin overload");
    Check_button();
  }else if(percent_empty > 50)
  {
    is_empty = true;
    Open_bin();
  }
}