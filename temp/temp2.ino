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
  //Serial.println(currentTemp);
  
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

  //Changes analog to voltage
  float voltage = currentTemp * (5.0 / 1023.0);
  //Serial.println(voltage);
  int fixedResistor = 10000;

  double thermResistance = fixedResistor * ((5/voltage) - 1);
  Serial.println(currentTemp);
  Serial.println(getTemperature(thermResistance));
  delay(1000);
 
}

// to convert analog input to degrees celsius using the Steinhart-Hart equation - https://www.ametherm.com/thermistor/ntc-thermistors-steinhart-and-hart-equation
double getTemperature(double thermResistance)
{
  // A, B, C are constants from callibration experiments and thermistor has resistance of 10K ohms
  
  double temp = 1/(-0.001787020645219 + (0.0007834559368536007 * log(thermResistance)) + (-0.000002593703712281802 * (log(thermResistance) * log(thermResistance) * log(thermResistance))));
   // Converts Kelvin to Celcius
   return temp - 273.15 + 6.132;
}
