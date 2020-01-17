#include <dht.h>
#include <SoftwareSerial.h>

SoftwareSerial HM10(2,3); // bluetooth module at PIN 2,3
dht DHT;

#define DHT11_PIN 5       // temperature and humidity sensor at Arduino digital pin 5

const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int pMoisture=A1;   // moisture sensor at Arduino analog pin A1
const int pump_led=A2;   // pump led at Arduino analog pin A2
const int pump_led2=A3;   // pump led at Arduino analog pin A3
const int pump=4;         // water pump sensor at Arduino digital pin 4
const int pump2=13;       // nurient pump at Arduino digital pin 13

// configuration for the ultrasonic sensor
const int trigPin = 11;   
const int echoPin = 10;

const int holdPin = 12;   // power switch at Arduino digital pin 12

//Variables
int value;          // Store value from photoresistor (0-1023)
int moisture;       // moisture reading
int sleep;          // sleep mode set by the switch
int limit = 600;    // moisture limit level 
int sleep_blue = 1; // bluetooth power enable
long duration;      // ultrasonic setting
int distance;       // water level reading
char appData;       
String inData = "";
unsigned long startMillis = 0;  
unsigned long currentMillis = 0;
unsigned long ledMillis = 0;
unsigned long pumpMillis = 0;
const unsigned long interval = 5000;  //5 second time period 
int water_led_state = 0;  // blinking LED
int blink_water_led = 0;        // notification of blinking LED
int blink_light_led = 0;
int blink_temp_led = 0;
int blink_hum_led = 0;
int pump_stat = 0;

void setup(){
 pinMode(pump,OUTPUT);
 pinMode(pump2,OUTPUT);
 pinMode(pump_led, OUTPUT);
 pinMode(pump_led2, OUTPUT);
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

 Serial.println("Welcome!");

 //Initialize Bluetooth Serial Port
 HM10.begin(9600);
}

void readSensors(){
  value = analogRead(pResistor);
  moisture = analogRead(pMoisture);
  int chk = DHT.read11(DHT11_PIN);
  if(moisture>limit)
  {
    analogWrite(pump_led, 255);
    pump_stat = 1; 
    digitalWrite(pump,1);
  }
  else
  { 
    pump_stat = 0;
    digitalWrite(pump,0);
    analogWrite(pump_led, 0);
  }
  
  if (DHT.temperature < 15){
    digitalWrite(6, HIGH); // switch ON LED
    blink_temp_led = 0;
  }else if(DHT.temperature > 40){
    blink_temp_led = 1;
  }else{
    digitalWrite(6, LOW); // switch OFF LED
    blink_temp_led = 0;
  } 

  if (DHT.humidity < 30){
    digitalWrite(9, HIGH); // switch ON LED
    blink_hum_led = 0;
  }else if(DHT.humidity  > 65){
    blink_hum_led = 1;
  }else{
    digitalWrite(9, LOW); // switch OFF LED
    blink_hum_led = 0;
  } 

  if (value < 250){
    digitalWrite(7, HIGH); // switch ON LED
    blink_light_led = 0;
  }else if(value  > 700){
    blink_light_led = 1;
  }else{
    digitalWrite(7, LOW); // switch OFF LED
    blink_light_led = 0;
  } 
}

void Pump()
{
  if (currentMillis - pumpMillis >= 3000) {
     // save the last time you blinked the LED
     pumpMillis = currentMillis;
     if (pump_stat == 1){
      digitalWrite(pump,0);
     }
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
  
  if (distance < 5){
    digitalWrite(8, HIGH); // switch ON LED
    blink_water_led = 0;
  }else if(distance > 15){
    blink_water_led = 1;
  }else{
    digitalWrite(8, LOW); // switch OFF LED
    blink_water_led = 0;
  } 
}

void blinking(int period){
  currentMillis = millis();
  if (currentMillis - ledMillis >= period) {
     // save the last time you blinked the LED
     ledMillis = currentMillis;
  
     // if the LED is off turn it on and vice-versa:
     if (water_led_state == LOW) {
       water_led_state = HIGH;
     }else {
       water_led_state = LOW;
     }
     // set the LED with the ledState of the variable:
     if (blink_water_led) {
      digitalWrite(8, water_led_state);
     }
     if (blink_temp_led) {
      digitalWrite(6, water_led_state);
     }
     if (blink_hum_led) {
      digitalWrite(9, water_led_state);
     }
     if (blink_light_led) {
      digitalWrite(7, water_led_state);
     }
   }
}

void LED_off(){
  digitalWrite(6, LOW); // switch OFF LED
  digitalWrite(7, LOW); // switch OFF LED
  digitalWrite(8, LOW); // switch OFF LED
  digitalWrite(9, LOW); // switch OFF LED
  analogWrite(pump_led, 0);
  digitalWrite(pump,0);
}

void send_data(){
  HM10.println("M"+String(moisture));
  delay(10);
  HM10.println("T"+String(DHT.temperature));
  delay(10);
  HM10.println("H"+String(DHT.humidity));
  delay(10);
  HM10.println("W"+String(distance));
  delay(10);
  HM10.println("L"+String(value));
  delay(10);
}

void print_data(){
  Serial.print("Light(low is dark) = ");
  Serial.println(value);
  Serial.print("Moisture(high is dry) = ");
  Serial.println(moisture);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.println();
}

void bluetooth(){
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
    analogWrite(pump_led, 0);
    digitalWrite(pump,0);
  }
  if ( inData == "N") {
    //Serial.println("LED ON");
    analogWrite(pump_led, 255);
    digitalWrite(pump,1);
  }
  if ( inData == "f") {
    //Serial.println("LED OFF");
    analogWrite(pump_led2, 0);
    digitalWrite(pump2,0);
  }
  if ( inData == "n") {
    //Serial.println("LED ON");
    analogWrite(pump_led2, 255);
    digitalWrite(pump2,1);
  }
  if ( inData == "O") {
    sleep_blue = 1;
  }
  if ( inData == "P") {
    sleep_blue = 0;
  }
  if ( inData == "A") {
    limit = 1300;
    //Serial.println("moisture limit: 300");
  }
  if ( inData == "B") {
    
    limit = 600;
    //Serial.println("moisture limit: 600");
  }
  if ( inData == "C") {
    limit = 200;
    //Serial.println("moisture limit: 1300");
  }
  if ( inData == "z") {
    analogWrite(pump_led, 255);
    digitalWrite(pump,1);
  }
}

void main_function(int printing){
  sleep = digitalRead(holdPin);
  if (sleep_blue == 1){
    currentMillis = millis();  //timer start
    if (currentMillis - startMillis >= 4000) {
      readWaterLevel();
      readSensors();
      send_data();
      if (printing == 1){ 
        print_data();
      }
      startMillis = currentMillis;
      pumpMillis = currentMillis;
      ledMillis = currentMillis;
    }
    if (currentMillis - pumpMillis >= 2000) {
     // save the last time you blinked the LED
     pumpMillis = currentMillis;
     if (pump_stat == 1){
      digitalWrite(pump,0);
     }
    }
    blinking(200);
  }else{
    LED_off();
  }
}

void loop() 
{ 
  main_function(1);
  bluetooth(); // interact with bluetooth
}
