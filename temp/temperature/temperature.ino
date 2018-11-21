// desiredTemp comes from user input on control panel

int desiredTemp = 470;

void setup() 
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(9, OUTPUT);

}

void loop() 
{
  int currentTemp = analogRead(A0);
  Serial.println(currentTemp);
  //if (Serial.available())
  //{
    //desiredTemp = serial.read();
  //}

  // this loops 60 times a second anyway - so do I need a while loop?
  if (desiredTemp < currentTemp)
  {
    //turn heater on
    digitalWrite(9, HIGH);       // sets the digital pin 13 on
    delay(1000);
  }
  else if (desiredTemp > currentTemp)
  {
    //turn heater off
    digitalWrite(9, LOW);       // sets the digital pin 13 on
    delay(1000);
  }

  //delay(1000);
 
}

// to convert analog input to degrees celsius using the Steinhart-Hart equation
//void getTemperature()
//{
 // ...
//}
