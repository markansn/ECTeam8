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
  
  if (desiredTemp < getTemperature(currentTemp))
  {
    //turn heater on
    digitalWrite(9, HIGH);       // sets the digital pin 13 on
    delay(1000);
  }
  else if (desiredTemp > getTemperature(currentTemp))
  {
    //turn heater off
    digitalWrite(9, LOW);       // sets the digital pin 13 on
    delay(1000);
  }

  //delay(1000);
 
}

// to convert analog input to degrees celsius using the Steinhart-Hart equation - https://www.ametherm.com/thermistor/ntc-thermistors-steinhart-and-hart-equation
double getTemperature(void)
{
  // A, B, C are constants from callibration experiments 
   double temp = 1/(A + (B * log(10000)) + (C * (log(10000) * log(10000) * log(10000))))
   // Converts Kelvin to Celcius
   return temp - 273.15;
}
