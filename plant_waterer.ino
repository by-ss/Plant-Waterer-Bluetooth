#include <dht.h>
#include <SoftwareSerial.h>

SoftwareSerial hc06(2,3);
dht DHT;

#define DHT11_PIN 5

const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int pMoisture=A1;       // moisture sensor at Arduino analog pin A1
const int pump=4; 
const int trigPin = 11;
const int echoPin = 10;

//Variables
int value;          // Store value from photoresistor (0-1023)
int moisture;
int limit = 700;
unsigned int blue=0; 
long duration;
int distance;

void setup(){
 pinMode(pump,OUTPUT);
 pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
 pinMode(pMoisture, INPUT);// Set pMoisture - A1 pin as an input (optional)
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 Serial.begin(9600);

 Serial.println("Commands:");

 //Initialize Bluetooth Serial Port
 hc06.begin(9600);
}

void readSensors(){
  value = analogRead(pResistor);
  Serial.print("Light(low is dark) = ");
  Serial.println(value);
  
  moisture = analogRead(pMoisture);
  Serial.print("Moisture(high is dry) = ");
  Serial.println(moisture);
  
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  //You can change value "25"
  if(moisture>limit)
  {
    Pump(1); 
  }
  else
  { 
    Pump(0);
  }
  delay(2000);
}

void Pump(int stat)
{
  if(stat == 0)
  {
    digitalWrite(pump,stat);
    Serial.println("Pump OFF");
  }
  if(stat == 1)
  {
    digitalWrite(pump,stat);
    Serial.println("Pump ON");
  }
}

int Bluetooth()
{
   // send data only when you receive data:
   if(Serial.available() > 0) 
   {
    // read the incoming byte:
    blue = Serial.read();

    // say what you got:
    Serial.print("Bluetooth");
    Serial.println(blue);
    if(blue == 48)
      return 0;
    if(blue == 49)
      return 1;
  }
  else 
  return 0;
}

void readWaterLevel(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}

void loop() 
{ 
  while(Bluetooth()==1)
  {
    blue = 0;
    break;
  }
  while(Bluetooth()==0)
  {
    readSensors();
    readWaterLevel();
  }
}
