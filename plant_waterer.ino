#include <dht.h>
#include <SoftwareSerial.h>

SoftwareSerial hc06(2,3);
dht DHT;

#define DHT11_PIN 7

const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int pMoisture=A1;       // moisture sensor at Arduino analog pin A1
const int ledPin=9;       // Led pin at Arduino pin 9
const int moisturePin=8;       // Moisture pin at Arduino pin 8
const int pump=4; 

//Variables
int value;          // Store value from photoresistor (0-1023)
int moisture;
int limit = 700;
unsigned int blue=0; 

void setup(){
 pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
 pinMode(moisturePin, OUTPUT);  // Set moisturePin - 8 pin as an output
 pinMode(pump,OUTPUT);
 pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
 pinMode(pMoisture, INPUT);// Set pResistor - A1 pin as an input (optional)
 Serial.begin(9600);

 Serial.println("Commands:");

 //Initialize Bluetooth Serial Port
 hc06.begin(9600);
}

void readSensors(){
  value = analogRead(pResistor);
  
  digitalWrite(moisturePin, HIGH);  //Turn led off
  moisture = analogRead(pMoisture);
  Serial.print("Moisture(high is dry) = ");
  Serial.println(moisture);
  
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  //You can change value "25"
  if (value > 150){
    digitalWrite(ledPin, LOW);  //Turn led off
  }
  else{
    digitalWrite(ledPin, HIGH); //Turn led on
  }
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
  }
}
