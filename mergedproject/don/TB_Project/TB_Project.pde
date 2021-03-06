    //Don Charles - Lambert 2018 Mark Anson
import java.util.Random;


//mark
import processing.serial.*; //import the Serial library //<>// //<>//
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







ArrayList<PImage> images = new ArrayList<PImage>();
ArrayList<Float> pastpH = new ArrayList<Float>();
ArrayList<Float> pastTemp = new ArrayList<Float>();
ArrayList<Float> pastStir = new ArrayList<Float>();
long updateTimer[] = {0, 0, 0};
float desired[] = {5.7, 25, 1500};
float conditions[] = {0, 0, 0};
int state = 0;
int whichFloat = 0;


void setup(){
  size(900, 600);
  background(0, 0, 0);
  for(int x = 0; x <= 8; x++){
    PImage img = loadImage("images/" + x + ".png");
    images.add(img);
  }
  
  //mark
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[1], 9600);
  myPort.bufferUntil('\n'); 
}

void draw(){
  getValue("pH");
  getValue("temp");
  getValue("stirring");
  
  if(state == 0){
    drawMain(true);
  }else if(state == 1){
    drawMain(false);
  }else if(state == 2){
    drawGraph("pH");
  }else if(state == 3){
    drawGraph("temp");
  }else if(state == 4){
    drawGraph("stirring");
  }
  
  //team 8 tag
  drawTag();
}

void drawMain(boolean auto){
  //draws black background
  drawTitle();
  
  //panels
  //ph panel
  drawPanel(3, "pH", 50, desired[0], conditions[0]);
  
  //temp panel
  drawPanel(4, "temperature", 350, desired[1], conditions[1]);
  
  //stirring panel
  drawPanel(5, "stirring", 650, desired[2], conditions[2]);
  
  textSize(16);
  String mode = "";
  if(!auto){
    //Update Button
    image(images.get(6), 400, 525);
    //text says Manual
    mode = "Manual";
  }else{
    //text says Automatic
    mode = "Automatic";
  }
  text("Mode = " + mode, 185, 90);
}

void drawGraph(String variable){
  println("drawing graph");
  ArrayList<Float> listinuse;
  if(variable.charAt(0) == "p".charAt(0)){
    listinuse = pastpH;
  }else if(variable.charAt(0) == "t".charAt(0)){
    listinuse = pastTemp;
  }else{
    listinuse = pastStir;
  }
  
  drawTitle();
  stroke(255);
  //y axis
  line(100, 150, 100, 500);
  //x axis
  line(100, 500, 850, 500);
  //arrows
  line(90, 160, 100, 150);
  line(110, 160, 100, 150);
  line(840, 490, 850, 500);
  line(840, 510, 850, 500);
  //axis labels
  text(variable, 60, 325);
  text("Time", 825, 520);
  
  int NoOfLines = listinuse.size();
  for(int i = 0; i < NoOfLines; i++){
    Float y1;
    if(i != 0){
      y1 = 325 + 10*listinuse.get(i - 1);
    }else{
       y1 = 325 + 10*listinuse.get(i);
    }
    Float y2 = 325 + 10*listinuse.get(i);
    
    Float lineLength = (float)750 / NoOfLines;
    Float x = 100 + i * lineLength;
    line(x, y1, x + lineLength, y2);
  }
  
  image(images.get(8), 400, 525);
  
}

void drawPanel(int image, String variable, int x, float desired, float actual){
  rect(x, 120, 225, 400, 7);
  image(images.get(image), x + 5, 125);
  fill(0);
  textSize(16);
  
  //static text
  text("Desired " + variable + ":", x + 5, 170);
  text("Actual " + variable + ":", x + 5, 270);
  text("Percentage (%) Error: ", x + 5, 370);
  
  //dynamic text
  textSize(36);
  drawDesiredText(desired, x);
  text(actual, x + 25, 330);
  
  int percError = Math.abs(Math.round(((desired - actual)/desired) * 100));
  
  if(percError < 5){
    fill(0, 255, 0);
  }else if(percError < 10){
    fill(244, 134, 66);
  }else{
    fill(255, 0, 0);
  }
  text(percError + "%", x + 75, 430);
  image(images.get(7), x + 45, 470);
  //convert fill back to black
  fill(255);
}

void drawDesiredText(float desired, int x){
  if(whichFloat != 0){
   
  }else{
    text(desired, x + 25, 230);
  }
}

void getValue(String variable){
  int index = 0;
  if(variable.charAt(0) == "p".charAt(0)){
    index = 0;
  }else if(variable.charAt(0) == "t".charAt(0)){
    index = 1;
  }else{
    index = 2;
  }
  
  if(System.currentTimeMillis() - updateTimer[index] > 400){
    updateTimer[index] = System.currentTimeMillis();
  //  Random rand = new Random();
  //  conditions[index] = rand.nextFloat() * 10;
    
    if(variable.charAt(0) == "p".charAt(0)){
      pastpH.add(conditions[index]);
    }else if(variable.charAt(0) == "t".charAt(0)){
      pastTemp.add(conditions[index]);
    }else{
      pastStir.add(conditions[index]);
    }
    
  }
}

void mouseClicked(){
  //println(mouseX);
  //println(mouseY);
  if(inRange(mouseX, 55, 155) && inRange(mouseY, 75, 105)){
      state = 1;
  }else if(inRange(mouseX, 55, 155) && inRange(mouseY, 25, 55)){
      state = 0;
      whichFloat = 0;
  }
  
  if(state == 1){
    if(inRange(mouseX, 90, 185) && inRange(mouseY, 205, 230)){
      whichFloat = 1;
    }
  }
   
  if(state == 0 || state == 1){
    if(inRange(mouseX, 100, 188) && inRange(mouseY, 475, 505)){
      state = 2;
    }else if(inRange(mouseX, 400, 488) && inRange(mouseY, 475, 505)){
      state = 3;
    }else if(inRange(mouseX, 700, 788) && inRange(mouseY, 475, 505)){
      state = 4;
    }
  }else if(state > 1 && state < 5){
    if(inRange(mouseX, 402, 581) && inRange(mouseY, 529, 558)){
      state = 0;
      whichFloat = 0;
    }
  }
}

boolean inRange(int value, int x, int y){
  if(value < y && value > x){
    return true;
  }else{
    return false;
  }
}

void drawTitle(){
  background(0,0,0);
  //draws auto button
  image(images.get(0), 50, 20);
  //draws manual button
  image(images.get(1), 50, 70);
  //draws title
  image(images.get(2), 160, 30);
}

void drawTag(){
  textSize(12);
  text("Team 8 GUI (C) 2018", 760, 590);
  fill(255);
}
//Don Charles - Lambert 2018 Mark Anson





//mark 
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
      myPort.write("A\n");
      println("contact");
    }
  }
  
  else if(val.equals("t") || val.equals("p") || val.equals("s")) { 
    println("hereintps");
    println(val);
    prevVal = val;
    //TODO convert to float
  } else {  
    println("PREVVAL");
    println(prevVal);
    if(prevVal.equals("t")) {
      conditions[1] = float(val);
    } else if(prevVal.equals("p")) {
      conditions[0] = float(val);
    } else if(prevVal.equals("s")) {
      conditions[2] = float(val);
    }
     println(conditions);
   
    sendData();

    // when you've parsed the data you have, ask for more:
    myPort.write("A\n");
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
      myPort.write("t\n");        //send a 1
      myPort.write(str(desired[1]) + "\n"); 
      myPort.write("p\n"); 
      myPort.write(str(desired[0]) + "\n"); 
      myPort.write("s\n"); 
      myPort.write(str(desired[2]) + "\n"); 
      
    //}
  
}
