/*
This project was created by Michael adawaren and will be updated by me copyright will include using the code for personal 
purpose only and not publishing it for more inquires contact me at gadocer54@gmail.com
*/
#include <EEPROM.h>
#include <Servo.h>
#include<SoftwareSerial.h>
SoftwareSerial   mySerial(2, 3);// GSM method for a6c only this will not work for sim900

// declare all pins to be used including data types function and methods 
Servo servo;   
int trigPinop = 9;
int echoPinop = 8;            
int trigPin = 5;
int echoPin = 4;              
int state = LOW;  
int servoPin = 7;   
int addr = 0;
int binHeight;        
int val = 0; 
long duration, durag, duration2, dist, dist2, dist3, average,average2;   
long aver[3]; 
long aver2[3];
int  full10;
int  full30;
int  full40;
int  full50;
int  full70;
int  full90;


               
void setup() {
        
    Serial.begin(115200);  
    mySerial.begin(115200);
    servo.attach(servoPin); 
    pinMode(trigPinop, OUTPUT);  
    pinMode(echoPinop, INPUT); 
    pinMode(trigPin, OUTPUT);  
    pinMode(echoPin, INPUT);
     servo.write(150);         //close cap on power on
    delay(100);
    servo.detach();
    Serial.println("Initializing..."); // gsm looding time 10s startup delay 

  delay(10000);        

  mySerial.println("AT"); //at command to check if the sim is working return value "OK"
  updateSerial();

  delay(2000);

   mySerial.println("AT");
  updateSerial();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  durag = pulseIn(echoPin, HIGH);
  
  dist3 = durag * 0.034 / 2;
 
     
  Serial.print("tankheight: ");
  Serial.print(dist3);
  Serial.println(" cm");
   EEPROM.write(addr, dist3);
   Serial.println("SAVED TO EPROMM");
   binHeight =  EEPROM.read(addr);
  Serial.println(binHeight);
   delay(4000);

 
    
}


void sendsms1()
{
 
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+CMGS=\"+2347060987527\"");
  updateSerial();
  mySerial.println("DUMPSTER 22 AT COMPUTER SCIENCE IS IS ABOUT 60% FULL");   
  updateSerial();
  mySerial.write((char)26);
  
}
void sendsms2()
{
 
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+CMGS=\"+2347060987527\"");
  updateSerial();
  mySerial.println("DUMPSTER 22 AT COMPUTER SCIENCE IS 90% FULL RETRIVE");   
  updateSerial(); 
  mySerial.write((char)26);
  
}
void sendsms3()
{
 
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+CMGS=\"+2347060987527\"");
  updateSerial();
  mySerial.println("DUMPSTER 22 AT COMPUTER SCIENCE IS 30% FULL ");   
  updateSerial(); 
  mySerial.write((char)26);
  
}



// to calculate the average didtance from the input pin of the ultsrsonic sensor 
void calculate() {  
   digitalWrite(10,HIGH);
   digitalWrite(trigPin, LOW);
   delayMicroseconds(10);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(25);
   digitalWrite(trigPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   dist = (duration/2) / 29.1;    //obtain distance
}

void calculate2() {  
   digitalWrite(10,HIGH);
   digitalWrite(trigPinop, LOW);
   delayMicroseconds(10);
   digitalWrite(trigPinop, HIGH);
   delayMicroseconds(25);
   digitalWrite(trigPinop, LOW);
   pinMode(echoPinop, INPUT);
   duration2 = pulseIn(echoPinop, HIGH);
   dist2 = (duration2/2) / 29.1;    //obtain distance
}

void percentage(){
   full10 = binHeight * 10/100;
   full10 = binHeight - full30;

   full30 = binHeight * 30/100;
   full30 = binHeight - full30;

   full50 = binHeight * 50/100;
   full50 = binHeight - full50;

   full70 = binHeight * 70/100;
   full70 = binHeight - full70;

   full90 = binHeight * 85/100;
   full90 = binHeight - full90;


  } 
  
void loop(){
   for (int j=0;j<=2;j++) {   //average distance
    calculate2();               
   aver2[j]=dist2;            
    delay(10);              //delay between measurements
  }
 dist2=(aver2[0]+aver2[1]+aver2[2])/3;    

if ( dist2 <= 70 ) {
Serial.println("open bin ");
 servo.attach(servoPin);
 delay(1);
 servo.write(0);  
      
}
else{
    servo.write(150);    
    delay(1000);
    servo.detach(); 
  }

 
  for (int i=0;i<=2;i++) {   //calculate average distance
    calculate();               
    aver[i]=dist;            
    delay(10);              //delay between measurements for accuracy 
  }
 dist=(aver[0]+aver[1]+aver[2])/3;   
 percentage(); 

if ( dist <= full10 ) {
       Serial.println("BIN IS 10% FULL"); 
  }
  
  else if (dist <= full30){
       Serial.println("BIN IS 30% FULL");
       for(int j=0; j<=3; j++){
       sendsms3();
       }
       delay(300);
    }
    
  else if (dist > full50 && dist <= full70){
       Serial.println("BIN IS ABOUT  60% FULL");
       for(int j=0; j<=3; j++){
       sendsms1();
       }
       delay(300);
    }

    else if (dist <= full90){
       Serial.println("BIN IS ABOUT  90%  FULL RETRIVE BIN ");
       for(int j=0; j<=3; j++){
       sendsms2();
       }
       delay(300);
      }
   else {
     Serial.println("BIN IS EMTY ");
     delay(300);
   
   }
}
void updateSerial()

{

  delay(500);                     // Hold By 500 ms

  while (Serial.available())

  {

    mySerial.write(Serial.read()); // Forward what Serial received to Software Serial Port

  }

  while(mySerial.available())

  {

    Serial.write(mySerial.read()); // Forward what Software Serial received to Serial Port

  }

}  
