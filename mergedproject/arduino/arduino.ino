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
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin),check,FALLING);
  startTime=millis();
  input=60;



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

    //delay(700);

    digitalWrite(3, LOW);
  }

  if(ph > usr_ph + 0.2){
    digitalWrite(4, HIGH);
    
    

    //delay(700);

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
  if(currentTime-startTime==500){
     noInterrupts();
     //detachInterrupt(interruptPin);
     if (input >60){
        input=60;
        }
      if(input<30){
        input=30;
        }
      rpm=count*60;
      startTime=millis();
      //attachInterrupt(digitalPinToInterrupt(interruptPin),check,FALLING);
      count2++;
      sum=sum+rpm;
      if(rpm<requiredRPM){
        if(abs(rpm-requiredRPM)>=200){
          input=input+1;
          //Serial.println("less than 200");
          }else{
            //Serial.println("less");
            input=input+1;
            }
        //Serial.print("input: ");
       // Serial.println(input);   
    }
      if(rpm>requiredRPM){
        if(abs(rpm-requiredRPM)>=200){
          Serial.println("more than 200");
          input=input-1;
          }else{
            Serial.println("more");
            input=input-1;
            }
        Serial.print("input: ");
        Serial.println(input);
    }
    analogWrite(10,input);
    interrupts();
   }
   
  
  if(count2==40){
    count2=0;
    averageRPM=sum/40;  
    sum=0;
   
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
  
}







void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println("A");   // send a capital A
 
  delay(300);
  }
}
