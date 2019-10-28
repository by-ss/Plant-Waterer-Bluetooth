#include <dht.h>

dht DHT;

#define DHT11_PIN 2

const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int pMoisture=A1;       // Led pin at Arduino pin 9
const int ledPin=9;       // Led pin at Arduino pin 9
const int moisturePin=8;       // Led pin at Arduino pin 9

//Variables
int value;          // Store value from photoresistor (0-1023)
int moisture;

void setup(){
 pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
 pinMode(moisturePin, OUTPUT);  // Set lepPin - 8 pin as an output
 pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
 pinMode(pMoisture, INPUT);// Set pResistor - A1 pin as an input (optional)
 Serial.begin(9600);

 Serial.write("write this");
 Serial.println();
}

void loop(){
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

  delay(2000); //Small delay
}
