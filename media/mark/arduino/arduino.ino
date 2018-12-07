String val; // Data received from the serial port
int ledPin = 13; // Set the pin to digital I/O 13
boolean ledState = LOW; //to toggle our LED
String prevVal;
String ctemp = "1";
String cspd = "1";
String cph = "1";

void setup() {
  pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
  //initialize serial communications at a 9600 baud rate
  Serial.begin(9600);
  establishContact();  // send a byte to establish contact until receiver responds 

}


//void getValues() {
//  double temp = getTemp();
//  double pH = getpH();
//  double spd = getSpd();
//  return [temp,pH,spd];
//}



void loop() {
  // put your main code here, to run repeatedly:

    if (Serial.available() > 0) { // If data is available to read,
    val = Serial.readStringUntil('\n'); // read it and store it in val
    //temp = val;
    //ctemp = "y" + val;
    if(val != "A") //if we get a 1
    {
    
      
      if(val == "t" || val == "s" || val == "p") {
        
        prevVal = val;
      } else if(prevVal == "t") {
        ctemp = val; 
      } else if(prevVal == "p") {
        cph = val; 
      } else if(prevVal == "s") {
        cspd = val;
      }
    }
    delay(100);
  } 
    else {
    //send back desired values 

//    array test = getValues();
    Serial.println("t");
    Serial.println(getTemp()); 
    Serial.println("p");
    Serial.println(getpH()); 
    Serial.println("s");
    Serial.println(getSpd()); 
    delay(50);
    }

}




void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println("A");   // send a capital A
 
  delay(300);
  }
}

String getTemp() {
  return ctemp;

}

String getpH() {
  return cph;
}

String getSpd() {
  return cspd;
}
