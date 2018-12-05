volatile int interruptPin = 2;
double rpm;
unsigned int sum=0;
int count=0;
unsigned long startTime;
unsigned long currentTime;
double averageRPM;
int count2=0;
double requiredRPM;
int input; //the PWM input to control the voltage of the motor directly

void setup() {
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin),check,FALLING);
  startTime=millis();
  input=0;
}

void loop() {
  requiredRPM=0; // User input
  analogWrite(10,input); //Start the motor
  currentTime = millis();

  //Measure the number of the times that interruptPin turns from high to low, and calculate rpm 
  //using that number. This happens every half second
  if(currentTime-startTime==500){
      detachInterrupt(interruptPin);
      rpm=count*60;
      count=0;
      startTime=millis();
      attachInterrupt(digitalPinToInterrupt(interruptPin),check,FALLING);
      count2++;
      sum=sum+rpm;

      //Modify the rpm according to the user input by increase PWM value or decrease PWM value.
      if(rpm<requiredRPM){
        if(abs(rpm-requiredRPM)>=200){
          input=input+5;
          }else{
            input=input+1;
            }
        analogWrite(10,input);
    }
      if(rpm>requiredRPM){
        if(abs(rpm-requiredRPM)>=200){
          input=input-5;
          }else{
            input=input-1;
            }
        analogWrite(10,input);
    }
     }

  //Calculate the mean value of rpm every ten seconds, this is just for checking whether the speed is controlled succesfully
  if(count2==20){
    count2=0;
    averageRPM=sum/20;  
    sum=0;
    Serial.println(averageRPM);
  }
     
  

}

void check(){
  count++;
}
