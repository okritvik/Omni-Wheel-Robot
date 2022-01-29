/****************** VERSION 2 **********************/
// UPDATED THE ULTRASONIC SENSOR DEFINITION
// ADDED SERIAL TIME OUT FUNCTION TO 50ms

#include <math.h>
int M11=22,M12=23,M21=24,M22=25,M31=26,M32=27,TRIG=32,ECHO=33;
int mstime = 40;
String degree="";
String inside = "";
int intdegree = 0;
int distance=0;
/*float a1=float(2)/3.0; float a2=float(0); float a3=float(1)/3.0;
float b1=float(-1)/3.0; float b2=float(1)/sqrt(3); float b3=float(1)/3.0;
float c1=float(-1)/3.0; float c2=float(-1)/sqrt(3); float c3=float(1)/3.0;
float fx=0,fy=0,fw=0;*/
int dir1,dir2,dir3;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(50);
  pinMode(M11,OUTPUT);
  pinMode(M12,OUTPUT);
  pinMode(M21,OUTPUT);
  pinMode(M22,OUTPUT);
  pinMode(M31,OUTPUT);
  pinMode(M32,OUTPUT);
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  //Declaring PWM Output Pins
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(13,OUTPUT);
  //255 PWM is Highest speed
  analogWrite(3,255);
  analogWrite(4,255);
  analogWrite(5,255);
  //Initial Setup of Motor ( Making all 0 )
  digitalWrite(M11,LOW);
  digitalWrite(M12,LOW);
  digitalWrite(M21,LOW);
  digitalWrite(M22,LOW);
  digitalWrite(M31,LOW);
  digitalWrite(M32,LOW);
  
}
//bools for left,right,back,forward,stop,rotate
bool l=false,r=false,b=false,f=false,s=false,rot=false;

void loop() {
 //Serial.print("A1: ");
  //Serial.println(a1);
  if(Serial.available()>0){
    degree="";
    degree = Serial.readString();
    //Serial.print("Degree String = ");
    //Serial.println(degree);
    if(degree.toInt()==0){
      if(degree=="L"){
        Circular(-90);
        l=true;
        r=false;
        b=false;
        f=false;
        s=false;
        rot=false;
      }
      else if(degree=="R"){
        Circular(90);
        l=false;
        r=true;
        b=false;
        f=false;
        s=false;
        rot=false;
      }
      else if(degree=="F"){
        f=true;
        r=false;
        b=false;
        l=false;
        s=false;
        rot=false;
      }
      else if(degree=="B"){
        b=true;
        r=false;
        l=false;
        f=false;
        s=false;
        rot=false;
      }
      else if(degree=="S"){
        s=true;
        r=false;
        b=false;
        f=false;
        l=false;
        rot=false;
      }
      else if(degree=="ACW"){
        Circular(-1);
        digitalWrite(13,LOW);
      }
      else if(degree=="CW"){
        Circular(1);
        digitalWrite(13,HIGH);
      }
      else if(degree=="USS"){
        while(true){
          if(Serial.available()>0){
            inside = Serial.readString();
            if(inside=="CW"){
              Circular(1);
            }
            else if(inside=="ACW"){
              Circular(-1);
            }
            else if(inside=="BREAK"){
              break;
            }
          }
          digitalWrite(TRIG,LOW);
          delayMicroseconds(10);
          digitalWrite(TRIG,HIGH);
          delayMicroseconds(10);
          digitalWrite(TRIG,LOW);
          long duration = pulseIn(ECHO,HIGH);
          distance = (duration*0.0343)/2;
          if(distance<21){
            //Serial.print("DONE\n");
            omni(0,0,0,0,0,0);
            break;
          }
          else{
            omni(0,1,0,0,255,255);
            delay(5);
            omni(0,0,0,0,0,0);
          }
        }
      }
    }
    else{
      intdegree = degree.toInt();
      //Serial.print("Degree Int = ");
      //Serial.println(intdegree);
      rot = true;
      s=false;
      r=false;
      b=false;
      f=false;
      l=false;
    }
  }
  else{
    if(l){
      omni(0,1,0,0,255,255);
      delay(5);
      omni(0,0,0,0,0,0);
    }
    else if(r){
      omni(0,1,0,0,255,255);
      delay(5);
      omni(0,0,0,0,0,0);
    }
    else if(f){
      omni(0,1,0,0,255,255);
      delay(5);
      omni(0,0,0,0,0,0);
    }
    else if(b){
      omni(0,0,1,0,255,255);
      delay(5);
      omni(0,0,0,0,0,0);
    }
    else if(s){
      omni(0,0,0,0,0,0);
      delay(5);
      omni(0,0,0,0,0,0);
    }
    else if(rot){
      Circular(intdegree);
      rot=false;
    }
  }
  //onlyCircular();
}
/*
void loop(){
  omni(0,0,0,0,0,0);
  delay(10);
  omni(0,0,0,0,0,0);
}
*/
void onlyCircular(){
  omni(0,0,0,255,255,255);
  delay(5);
  omni(0,0,1,0,0,0);
}
//Circular rotation according to rotation angle

void Circular(int deg){
  int  delaytime_ms=0;
  if(deg>0){
    delaytime_ms = 20*deg;
    omni(1,1,1,255,255,255);
  }
  else{
    delaytime_ms = -20*deg;
    omni(0,0,0,255,255,255);
  }
  delay(delaytime_ms);
  omni(0,0,0,0,0,0);
  delay(5);
}
//Motor running code
void omni(int dir_a, int dir_b, int dir_c, int pwm_a, int pwm_b, int pwm_c){
  analogWrite(3,pwm_a);
  if(dir_a == 0){
    digitalWrite(M11,HIGH);
    digitalWrite(M12,LOW);
  }
  if(dir_a == 1){
    digitalWrite(M11,LOW);
    digitalWrite(M12,HIGH);
  }
  analogWrite(4,pwm_b);
  if(dir_b == 0){
    digitalWrite(M21,HIGH);
    digitalWrite(M22,LOW);
  }
  if(dir_b == 1){
    digitalWrite(M21,LOW);
    digitalWrite(M22,HIGH);
  }
  analogWrite(5,pwm_c);
  if(dir_c == 0){
    digitalWrite(M31,HIGH);
    digitalWrite(M32,LOW);
  }
  if(dir_c == 1){
    digitalWrite(M31,LOW);
    digitalWrite(M32,HIGH);
  }
}

//CODE FOR OMNI DIRECTION (NOT WORKING PROPERLY) Check datatype resolution
/*      fy=sin(intdegree);
      fx=cos(intdegree);
      fw=0;
      float m1=(a1*fx+a2*fy+a3*fw)*300;
      float m2=(b1*fx+b2*fy+b3*fw)*300;
      float m3=(c1*fx+c2*fy+c3*fw)*300;
      Serial.println(int(m1));
      Serial.println(int(m2));
      Serial.println(int(m3));
      Serial.println();
      Serial.println();

      if(m1<0){
        dir1=0;
        m1=-m1;
      }
      else{
        dir1=1;
      }
      if(m2<0){
        dir2=0;
        m2=-m2;
      }
      else{
        dir2=1;
      }
      if(m3<0){
        dir3=0;
        m3=-m3;
      }
      else{
        dir3=1;
      }
      omni(dir1,dir2,dir3,int(m1),int(m2),int(m3));
      delay(5);
      omni(dir1,dir2,dir3,0,0,0);
      */
