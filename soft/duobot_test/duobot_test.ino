#include <GY521.h>

#define PIN_DIR_DTE     (3)
#define PIN_STEP_DTE    (4)
#define PIN_EN_DTE      (5)
#define PIN_DIR_GHE     (6)
#define PIN_STEP_GHE    (7)
#define PIN_EN_GHE      (8)

#define PIN_M2          (9)
#define PIN_M1          (10)
#define PIN_M0          (11)

GY521 sensor(0x68);

void init_gy521()
{
  Wire.begin();

  delay(100);
  while (sensor.wakeup() == false)
  {
    Serial.println("\tImpossible de se connecteur au GY521");
    delay(1000);
  }
  sensor.setAccelSensitivity(0);  //  2g
  sensor.setGyroSensitivity(0);   //  250 degrees/s

  sensor.setThrottle();
  
  sensor.axe = -0.1128369;
  sensor.aye = 0.0038721;
  sensor.aze = -1.0322095;
  sensor.gxe = 8.0835113;
  sensor.gye = -1.0030534;
  sensor.gze = -0.5611451;  
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

  init_gy521();
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

void runMotor(int isleft,int dir,  unsigned long steps) {
  int PIN_STAGE_10;
  int PIN_STAGE_9;

  if (isleft) { 
    digitalWrite(PIN_DIR_GHE,dir);
    digitalWrite(PIN_EN_GHE,LOW);
    PIN_STAGE9 = PIN_STEP_GHE;
    PIN_STAGE10 = PIN_EN_GHE;
    } 
  else 
  {
    digitalWrite(PIN_DIR_DTE,dir==HIGH?LOW:HIGH);  
    digitalWrite(PIN_EN_DTE,LOW);
    PIN_STAGE_9 = PIN_STEP_DTE;
    PIN_STAGE_10 =PIN_EN_DTE;
  }

  delay(10);
    
  for (unsigned long i=0;i<steps;i++)
    {
      digitalWrite(PIN_STAGE9,HIGH);
      delayMicroseconds(100);
      digitalWrite(PIN_SSTAGE9,LOW);
      delayMicroseconds(100);
    }
  digitalWrite(PIN_STEP_10, HIGH);    
  
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

int counter=0;

void loop() 
{
  delay(100);

  sensor.read();
  int ax = sensor.getAccelX();
  int ay = sensor.getAccelY();
  int az = sensor.getAccelZ();
  int gx = sensor.getGyroX();
  int gy = sensor.getGyroY();
  int gz = sensor.getGyroZ();
  int t = sensor.getTemperature();

  if (counter % 10 == 0)
  {
    Serial.println("\n\tACCELEROMETER\t\tGYROSCOPE\t\tTEMPERATURE");
    Serial.println("\tax\tay\taz\tgx\tgy\tgz\tT");
  }

  Serial.print(counter);
  Serial.print('\t');
  Serial.print(ax);
  Serial.print('\t');
  Serial.print(ay);
  Serial.print('\t');
  Serial.print(az);
  Serial.print('\t');
  Serial.print(gx);
  Serial.print('\t');
  Serial.print(gy);
  Serial.print('\t');
  Serial.print(gz);
  Serial.print('\t');
  Serial.print(t);
  Serial.println();

  counter++;
}
