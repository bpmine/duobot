#include <Wire.h>

#define PIN_DIR_DTE     (3)
#define PIN_STEP_DTE    (4)
#define PIN_EN_DTE      (5)
#define PIN_DIR_GHE     (6)
#define PIN_STEP_GHE    (7)
#define PIN_EN_GHE      (8)

#define PIN_M2          (9)
#define PIN_M1          (10)
#define PIN_M0          (11)

#define MPU_ADDR        (0x68)
#define MPU_REG_POWER   (0x6B)
#define MPU_REG_MEASURE (0x3B)

#define MIN_VAL 265
#define MAX_VAL 402

void init_mpu()
{
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(MPU_REG_POWER);
  Wire.write(0);
  Wire.endTransmission(true);
}

int mpu_read_angle(void)
{
  int16_t AcX,AcY,AcZ;
  double x;
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(MPU_REG_MEASURE);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  int xAng = map(AcX,MIN_VAL,MAX_VAL,-90,90);
  int yAng = map(AcY,MIN_VAL,MAX_VAL,-90,90);
  int zAng = map(AcZ,MIN_VAL,MAX_VAL,-90,90);
  
  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);

  if (x>360)
    x=360;
    
  if (x>120)
    return -(360-x);
  else
    return x;
}

void setup() 
{
  Serial.begin(115200);
  Serial.println("BOOT Duobot.");

  pinMode(PIN_DIR_DTE,OUTPUT);
  digitalWrite(PIN_DIR_DTE,LOW);
  pinMode(PIN_STEP_DTE,OUTPUT);
  digitalWrite(PIN_STEP_DTE,LOW);
  pinMode(PIN_EN_DTE,OUTPUT);
  digitalWrite(PIN_EN_DTE,HIGH);

  pinMode(PIN_DIR_GHE,OUTPUT);
  digitalWrite(PIN_DIR_GHE,LOW);
  pinMode(PIN_STEP_GHE,OUTPUT);
  digitalWrite(PIN_STEP_GHE,LOW);
  pinMode(PIN_EN_GHE,OUTPUT);
  digitalWrite(PIN_EN_GHE,HIGH);

  pinMode(PIN_M0,OUTPUT);
  digitalWrite(PIN_M0,HIGH);
  pinMode(PIN_M1,OUTPUT);
  digitalWrite(PIN_M1,HIGH);
  pinMode(PIN_M2,OUTPUT);
  digitalWrite(PIN_M2,HIGH);

  init_mpu();
}

void runMotorLeft(int dir,unsigned long steps)
{
  runMotor(true, /*isleft*/
           dir,
           steps);
}
void runMotorRight(int dir,unsigned long steps)
{
  runMotor(false, /*isleft*/
           dir,
           steps);
}

void runMotor(int isleft,int dir,  unsigned long steps)
{
  int PIN_EN;
  int PIN_STEP;
  if (isleft) 
  { 
    digitalWrite(PIN_DIR_GHE,dir);
    PIN_EN = PIN_EN_GHE;
    PIN_STEP = PIN_STEP_GHE;
  } 
  else
  {
    digitalWrite(PIN_DIR_DTE,dir==HIGH?LOW:HIGH);
    PIN_EN  = PIN_EN_DTE;
    PIN_STEP = PIN_STEP_DTE;
  }

  digitalWrite(PIN_EN,LOW);
  delay(10);
    
  for (unsigned long i=0;i<steps;i++)
    {
      digitalWrite(PIN_STEP,HIGH);
      delayMicroseconds(100);
      digitalWrite(PIN_STEP,LOW);
      delayMicroseconds(500);
    }
  digitalWrite(PIN_EN, HIGH);    
}

void runMotors(int dir,unsigned long steps)
{
  digitalWrite(PIN_DIR_GHE,dir);
  digitalWrite(PIN_DIR_DTE,dir==HIGH?LOW:HIGH);
  
  digitalWrite(PIN_EN_GHE,LOW);
  digitalWrite(PIN_EN_DTE,LOW);
  delay(10);
  for (unsigned long i=0;i<steps;i++)
  {
    digitalWrite(PIN_STEP_GHE,HIGH);
    digitalWrite(PIN_STEP_DTE,HIGH);
    delayMicroseconds(100);
    digitalWrite(PIN_STEP_GHE,LOW);
    digitalWrite(PIN_STEP_DTE,LOW);
    delayMicroseconds(500);
  }
  
  digitalWrite(PIN_EN_GHE,HIGH);    
  digitalWrite(PIN_EN_DTE,HIGH);    
}


void serialEvent()
{
  int cmd=Serial.read();
  if (cmd=='a')
  {
    Serial.println("AV gauche");
    runMotorLeft(LOW,10000);
  }
  else if (cmd=='q')
  {
    Serial.println("AR gauche");
    runMotorLeft(HIGH,10000);
  }  
  else if (cmd=='z')
  {
    Serial.println("AV droite");
    runMotorRight(LOW,10000);
  }
  else if (cmd=='s')
  {
    Serial.println("AR droite");
    runMotorRight(HIGH,10000);
  }
  else if (cmd=='e')
  {
    Serial.println("AVANT");
    runMotors(LOW,2000);
  }
  else if (cmd=='d')
  {
    Serial.println("ARRIERE");
    runMotors(HIGH,2000);
  }
}

void loop() 
{
  unsigned long t=millis();
  
  delay(1000);

  int ang=mpu_read_angle();
  Serial.print("Angle: ");
  Serial.println(ang);

  /*if (ang>10)
    runMotors(LOW,1000);
  else if (ang<-10)
    runMotors(HIGH,1000);*/
}
