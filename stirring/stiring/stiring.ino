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
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin),check,FALLING);
  startTime=millis();
  input=60;
}

void loop() {
  requiredRPM=800;
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
    Serial.println(averageRPM);
  }
}

void check(){
  //count++;
  Serial.println("test");
}
