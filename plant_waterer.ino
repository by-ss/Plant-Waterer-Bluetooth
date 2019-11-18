#include <dht.h>
#include <SoftwareSerial.h>

SoftwareSerial HM10(2,3);
dht DHT;

#define DHT11_PIN 5

const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int pMoisture=A1;       // moisture sensor at Arduino analog pin A1
const int pump=4; 
const int pump2=13; 
const int trigPin = 11;
const int echoPin = 10;
const int holdPin = 12;

//Variables
int value;          // Store value from photoresistor (0-1023)
int moisture;
int sleep;
int limit = 700;
unsigned int blue=0;
long duration;
int distance;
char appData;  
String inData = "";
unsigned long startMillis = 0;  
unsigned long currentMillis = 0;
const unsigned long interval = 5000;  //5 second time period 

void setup(){
 pinMode(pump,OUTPUT);
 pinMode(pump2,OUTPUT);
 pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
 pinMode(pMoisture, INPUT);// Set pMoisture - A1 pin as an input (optional)
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 pinMode(holdPin, INPUT); // hold evreything
 pinMode(6, OUTPUT); // onboard LED
 digitalWrite(6, LOW); // switch OFF LED
 pinMode(7, OUTPUT); // onboard LED
 digitalWrite(7, LOW); // switch OFF LED
 pinMode(8, OUTPUT); // onboard LED
 digitalWrite(8, LOW); // switch OFF LED
 pinMode(9, OUTPUT); // onboard LED
 digitalWrite(9, LOW); // switch OFF LED
 
 Serial.begin(9600);

 Serial.println("Commands:");

 //Initialize Bluetooth Serial Port
 HM10.begin(9600);
}

void readSensors(){
  value = analogRead(pResistor);
  //Serial.print("Light(low is dark) = ");
  //Serial.println(value);
  
  moisture = analogRead(pMoisture);
  //Serial.print("Moisture(high is dry) = ");
  //Serial.println(moisture);
  
  int chk = DHT.read11(DHT11_PIN);
  //Serial.print("Temperature = ");
  //Serial.println(DHT.temperature);
  //Serial.print("Humidity = ");
  //Serial.println(DHT.humidity);
  //You can change value "25"
  if(moisture>limit)
  {
    digitalWrite(7, HIGH); // switch OFF LED
    Pump(1); 
  }
  else
  { 
    Pump(0);
    digitalWrite(7, LOW); // switch OFF LED
  }
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
    //Serial.println("Pump ON");
    delay(1000);
    digitalWrite(pump,0);
  }
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
  //Serial.print("Distance: ");
  //Serial.println(distance);

  if (distance<10){
    digitalWrite(8, HIGH); // switch ON LED
  }else if (distance<200 & distance>10){
    digitalWrite(8, LOW); // switch OFF LED
  } 
}

void loop() 
{ 
  sleep = digitalRead(holdPin);
  /*
  while(sleep == 0){
    sleep = digitalRead(holdPin);
    delay(1000);
  }*/
  if (sleep == 1){
    currentMillis = millis();  //timer start
    if (currentMillis - startMillis >= interval) {
      readWaterLevel();
      readSensors();
      startMillis = currentMillis;
    }
  }else{
    digitalWrite(6, LOW); // switch OFF LED
    digitalWrite(7, LOW); // switch OFF LED
    digitalWrite(8, LOW); // switch OFF LED
    digitalWrite(9, LOW); // switch OFF LED
  }
  
  HM10.listen();  // listen the HM10 port
  while (HM10.available() > 0) {   // if HM10 sends something then read
    appData = HM10.read();
    inData = String(appData);  // save the data in string format
    Serial.println(appData);
  }
 
  if (Serial.available()) {           // Read user input if available.
    delay(10);
    HM10.write(Serial.read());
  }
  
  if ( inData == "F") {
    //Serial.println("LED OFF");
    digitalWrite(6, LOW); // switch OFF LED
  }
  if ( inData == "N") {
    //Serial.println("LED ON");
    digitalWrite(6, HIGH); // switch OFF LED
  }
  
}
