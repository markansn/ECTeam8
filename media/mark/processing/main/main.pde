import processing.serial.*; //import the Serial library //<>//
 Serial myPort;  //the Serial port object
 String val;
 String prevVal = "n";
 String ph = "0";
 String spd = "0";
 String temp = "0";
 Boolean valuesToSend = false;
// since we're doing serial handshaking, 
// we need to check if we've heard from the microcontroller
boolean firstContact = false;

void setup() {
  size(200, 200); //make our canvas 200 x 200 pixels big
  //  initialize your serial port and set the baud rate to 9600
  myPort = new Serial(this, "COM4", 9600);
  myPort.bufferUntil('\n'); 
}

void draw() {
  //println("running");
  //we can leave the draw method empty,  
  //because all our programming happens in the serialEvent (see below)
}


void serialEvent( Serial myPort) {
try {
//put the incoming data into a String - 
//the '\n' is our end delimiter indicating the end of a complete packet
val = myPort.readStringUntil('\n');
//make sure our data isn't empty before continuing
if (val != null) {
  //trim whitespace and formatting characters (like carriage return)
  println("THEVAL");
  println(val);
  val = trim(val);
  

  //look for our 'A' string to start the handshake
  //if it's there, clear the buffer, and send a request for data

  if (firstContact == false) {
    if (val.equals("A")) {
      myPort.clear();
      firstContact = true;
      myPort.write("A");
      println("contact");
    }
  }
  
  else if(val.equals("t") || val.equals("p") || val.equals("s")) { 
    println("hereintps");
    print(val);
    prevVal = val;
    //TODO convert to float
  } else {  
    println("PREVVAL");
    println(prevVal);
    if(prevVal.equals("t")) {
      temp = val;
    } else if(prevVal.equals("p")) {
      ph = val;
    } else if(prevVal.equals("s")) {
      spd = val;
    }
 
    println(temp);
    println(ph);
    println(spd);
    sendData();

    // when you've parsed the data you have, ask for more:
    myPort.write("A");
    }
  }
  
  
  
  
} catch(RuntimeException e) {
  e.printStackTrace();
}
}






void sendData() {
    //FloatList values = [];
    //if (valuesToSend == true) 
    //{ 
      ////values = getValues();
      println("here4");
      myPort.write("t");        //send a 1
      myPort.write(getdTemp()); 
      myPort.write("p"); 
      myPort.write(getdPH()); 
      myPort.write("s"); 
      myPort.write(getdSpd()); 
      
    //}
  
}


String getdPH() {
  return "2";
}

String getdTemp() {
  return "2";
}
String getdSpd() {
  return "2";
}
