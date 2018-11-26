char val; // Data received from the serial port
int ledPin = 13; // Set the pin to digital I/O 13
boolean ledState = LOW; //to toggle our LED


void setup() {
  pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
  //initialize serial communications at a 9600 baud rate
  Serial.begin(9600);
  establishContact();  // send a byte to establish contact until receiver responds 

}



void loop() {
  // put your main code here, to run repeatedly:

    if (Serial.available() > 0) { // If data is available to read,
    val = Serial.read(); // read it and store it in val

    if(val != "A") //if we get a 1
    {
       //recieved array packet
    }
    delay(100);
  } 
    else {
    Serial.println("Hello, world!"); //send back a hello world
    delay(50);
    }

}


void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println("A");   // send a capital A
  delay(300);
  }
}
