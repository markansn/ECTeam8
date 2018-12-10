#include<math.h>

void setup() {
  
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
   

}

void loop() {
  int temp = 293; //get temperetaure and convert to Kelvins
  double ES = 0.0; 
  double EX = analogRead(A1)*5.0/1023.0; //get voltage

  double F = 9.6485309*10000;
  double R = 8.314510;

  double phs = 7.0;

  double ph = phs + ((ES -EX) * F) / (R * temp * log(10)); //calculate pH 

  double usr_ph = 5.0 // get from user interface

 


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


  delay(10000);

  
  

}
