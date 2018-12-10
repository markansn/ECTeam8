#include <stdio.h>
#include <stdlib.h>
//serial link
String val; 
String prevVal;

//DO TOMORROW
String ctemp = "1";
String cspd = "1";
String cph = "1";


String cdesiredtemp = "25";
String cdesiredspd = "800";
String cdesiredph = "5";

double desiredtemp = 0;
double desiredspd = 0;
double desiredph = 0;


double dtemp = 0;
double dph = 0;
double dspd = 0;
//temp




//string 
volatile int interruptPin = 2;
double duration;
double duration2;
double rpm;
unsigned long time;
unsigned int sum=0;
int count=0;
unsigned long startTime;
unsigned long currentTime;
double averageRPM;
int count2=0;
double requiredRPM;
int input;


void setup() {
  //serial 
  Serial.begin(9600);
  establishContact();  // send a byte to establish contact until receiver responds 


 //temp
 pinMode(A0, INPUT);
 pinMode(9, OUTPUT);


 //stiring
  pinMode(10,OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin),check,FALLING);
  startTime=millis();
  input=0;



  //ph
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  
  
}



void loop() {


 desiredtemp = cdesiredtemp.toDouble();
 desiredph = cdesiredph.toDouble();
 desiredspd = cdesiredspd.toDouble();
//
// 
//  

  //Serial.println(desiredtemp);
  handleSerial();
  

//  
  dtemp = temp();
  dph = ph();
  dspd = str();
// 
  ctemp = String(dtemp);
  cph = String(dph);
  cspd = String(dspd);
  
  delay(1000);

}






//PH
double ph() {
  double temp = dtemp + 273.15; //get temperetaure and convert to Kelvins
  double ES = 0.0; 
  double EX = analogRead(A1)*5.0/1023.0; //get voltage

  double F = 9.6485309*10000;
  double R = 8.314510;

  double phs = 7.0;

  double ph = phs + ((ES -EX) * F) / (R * temp * log(10)); //calculate pH 

  double usr_ph = desiredph; // get from user interface

 


  if(ph < usr_ph - 0.2) {
    digitalWrite(3, HIGH);

    delay(700);

    digitalWrite(3, LOW);
  }

  if(ph > usr_ph + 0.2){
    digitalWrite(4, HIGH);
    
    

    delay(700);

    digitalWrite(4, LOW);

    
  }


  //delay(10000);

  

  return ph;
}


//TEMP
double temp() {

 int currentTemp = analogRead(A0);

 //Changes analog to voltage
 double voltageResistor = currentTemp * (5.0 / 1023.0);
 //Serial.println(voltage);
 int fixedResistor = 10000;

 double thermResistance = fixedResistor * ((5/voltageResistor) - 1);


 if (desiredtemp < getTemperature(thermResistance))
 {
   //turn heater off
  
   //digitalWrite(9, LOW);       // sets the digital pin 13 on
   analogWrite(9, 0);
   //delay(1000);
 }
 else if (desiredtemp > getTemperature(thermResistance))
 {
   //turn heater on
 
   //digitalWrite(9, HIGH);       // sets the digital pin 13 on
   analogWrite(9,150);
   //delay(1000);
 }
 
 return(getTemperature(thermResistance));


  
}

// to convert analog input to degrees celsius using the Steinhart-Hart equation - https://www.ametherm.com/thermistor/ntc-thermistors-steinhart-and-hart-equation
double getTemperature(double thermResistance)
{
 // A, B, C are constants from callibration experiments and thermistor has resistance of 10K ohms

 //double temp = 1/(-0.001787020645219 + (0.0007834559368536007 * log(thermResistance)) + (-0.000002593703712281802 * (log(thermResistance) * log(thermResistance) * log(thermResistance))));
  // Converts Kelvin to Celcius
 // return temp - 273.15 + 5; //+ 6.132;
  int R0 = 10000;
  float T0 = 298.15;
  int B = 4080;

  double kelvinTemp = 1/((1/T0) + ((log(thermResistance/R0))/B));
  return kelvinTemp - 273.15 - 1.458 - 1.64;
}



//STIRRING
double str() {



 requiredRPM=desiredspd;
  analogWrite(10,input);
  currentTime = millis();

  //Measure the number of the times that interruptPin turns from high to low, and calculate rpm 
  //using that number. This happens every half second
  if(currentTime-startTime==500){
      if(input>255){
        input=255;
        }
      if(input<0){
        input=0;
        }
      detachInterrupt(interruptPin);
      rpm=count*60;
      count=0;
      startTime=millis();
      attachInterrupt(digitalPinToInterrupt(interruptPin),check,FALLING);
      count2++;
      //Serial.println(rpm);
      sum=sum+rpm;

      //Change the rpm according to the user input by increase PWM value or decrease PWM value.
      if(rpm<requiredRPM){
        if(abs(rpm-requiredRPM)>=200){
          input=input+2;
          }else{
            input=input+1;
            }
        analogWrite(10,input);
        //Serial.println(rpm);
        //Serial.println("in 1");
        //Serial.println(input);
    }
      if(rpm>requiredRPM){
        if(abs(rpm-requiredRPM)>=200){
          input=input-2;
          }else{
            input=input-1;
            }
        analogWrite(10,input);
        //Serial.println(rpm);
        //Serial.println("in 2");
        //Serial.println(input);
    }
     }


  //Calculate the mean value of rpm every ten seconds, this is just for checking whether the speed is controlled succesfully
  if(count2==20){
    //Serial.println(sum);
    count2=0;
    averageRPM=sum/20;  
    sum=0;
    Serial.println(averageRPM);
  }
  
  return rpm;
}




void handleSerial() {



    if (Serial.available() > 0) { // If data is available to read,
    val = Serial.readStringUntil('\n'); // read it and store it in val
    //temp = val;
    //ctemp = "y" + val;
    if(val != "A") //if we get a 1
    {
    
      
      if(val == "t" || val == "s" || val == "p") {
        
        prevVal = val;
      } else if(prevVal == "t") {
        cdesiredtemp = val; 
      } else if(prevVal == "p") {
        cdesiredph = val; 
      } else if(prevVal == "s") {
        cdesiredspd = val;
      }
    }
    delay(100);
  } 
    else {
    //send back desired values 

//    array test = getValues();
    Serial.println("t");
    Serial.println(ctemp); 
    Serial.println("p");
    Serial.println(cph); 
    Serial.println("s");
    Serial.println(cspd); 
    delay(50);
    }




}



void check(){
  count++;
}







void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println("A");   // send a capital A
 
  delay(300);
  }
}
