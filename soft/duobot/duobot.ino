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

#define MIN_VAL         (265)
#define MAX_VAL         (402)

typedef struct
{
  int pin_enable;
  int pin_dir;
  int pin_step;
  bool is_inverted;
} T_DRV_DEF;

#define DIR_AVANT   (LOW)
#define DIR_ARRIERE (HIGH)

#define MOTOR_GHE   (0)
#define MOTOR_DTE   (1)
const T_DRV_DEF motors[]=
{
  {PIN_EN_GHE, PIN_DIR_GHE, PIN_STEP_GHE, false },
  {PIN_EN_DTE, PIN_DIR_DTE, PIN_STEP_DTE, true }
};


volatile unsigned long g_ulCounter=0;
volatile bool ton=false;
volatile int g_speed=0;
volatile bool g_enabled=false;

inline void pulse_step(int id)
{
  if (g_enabled==false)
  {
    digitalWrite(motors[id].pin_enable,HIGH);
    return;
  }

  digitalWrite(motors[id].pin_enable,LOW);  

  if ( g_speed>=0 )
    digitalWrite(motors[id].pin_dir, motors[id].is_inverted==false ? DIR_AVANT : DIR_ARRIERE);
  else
    digitalWrite(motors[id].pin_dir, motors[id].is_inverted==false ? DIR_ARRIERE : DIR_AVANT);
  
  if ( ( ton==true ) && (g_speed!=0) && (g_enabled==true))
    digitalWrite(motors[id].pin_step,HIGH);    
  else
    digitalWrite(motors[id].pin_step,LOW);
}

ISR(TIMER2_COMPA_vect)
{
  g_ulCounter++;
  
  int cnt=100-min(abs(g_speed),100);
  
  if (ton==false)
  {
    if (g_ulCounter>cnt)
      ton=true;
  }
  else
  {
    g_ulCounter=0;
    ton=false;
  }

  pulse_step(MOTOR_GHE);
  pulse_step(MOTOR_DTE);
}

void init_motor(int id)
{
  digitalWrite(motors[id].pin_enable,HIGH);
  digitalWrite(motors[id].pin_dir,DIR_AVANT);
  digitalWrite(motors[id].pin_step,LOW);
}

void init_timer(void)
{
  cli();
  TCCR2A = 0b00000010;
  TCCR2B = 0b00000110;  ///< Clock/256 pour faire 16us
  TIMSK2 = 0b00000010;
  OCR2A = 10;
  sei();
}

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
  init_motor(MOTOR_GHE);
  init_motor(MOTOR_DTE);
  init_timer();
}

void serialEvent()
{  
  int cmd=Serial.read();
  if (cmd=='a')
  {

    AVANCE();

    void AVANCE() {
       Serial.println("AV gauche");
       g_enabled=true;
       g_speed=abs(g_speed);  
    }
  }
  else if (cmd=='q')
  {

    STOP();
    void STOP() {
    Serial.println("Stop");
    g_enabled=false;
    }
  }
  else if (cmd=='w')
  {
    ARRIERE();
    void ARRIERE(void) {
       Serial.println("AR gauche");
       g_enabled=true;
       g_speed=-abs(g_speed);
    }
  }
  else if (cmd=='0')
  {
    SPEED_0();
    void SPEED_0() {
    Serial.println("Speed 0");
    g_speed=0;
    }
  }
  else if (cmd=='1')
  {
        SPEED_1();
    void SPEED_1() {
       Serial.println("Speed 1");
       g_speed=1;
    }
  }
  else if (cmd=='2')
  {
    SPEED_2();
   void SPEED_2() {
    Serial.println("Speed 2");
    g_speed=20;
   }
  }
  else if (cmd=='3')
  {
    SPEED_3();
    void SPEED_3[]{
    Serial.println("Speed 3");
    g_speed=50;
    }
  }
  else if (cmd=='4')
  {
    SPEED_4();
    void SPEED_4 () {
    Serial.println("Speed 4");
    g_speed=80;
    }
  }
  else if (cmd=='5')
  {
    SPEED_5();
    SPEED_5() {
    Serial.println("Speed 5");
    g_speed=100;
    }
  }
}

void loop() 
{
  unsigned long t=millis();
  
  delay(1000);

  int ang=mpu_read_angle();
  Serial.print("Angle: ");
  Serial.println(ang);

  /*if (ang>2)
  {
    g_enabled=true;
    g_speed=10;
  }
  else if (ang<-2)
  {
    g_enabled=true;
    g_speed=-10;
  }
  else if (abs(ang)>=2)
  {
    g_enabled=true;
  }
  else
  {
    g_enabled=false;
  }*/
}
