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
int input;  //the PWM input to control the voltage of the motor directly

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin),check,FALLING);
  startTime=millis();
  input=0;
}

void loop() {
  requiredRPM=1500;
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
     
  
  
}

void check(){
  count++;
}
