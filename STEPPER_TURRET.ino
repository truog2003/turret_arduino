
//Written by Ahmet Burkay KIRNIK
//TR_CapaFenLisesi
//Measure Angle with a MPU-6050(GY-521)
//https://www.instructables.com/id/How-to-Measure-Angle-With-MPU-6050GY-521/
//https://www.usinainfo.com.br/blog/controle-de-posicao-do-micro-servo-motor-180/
//http://arduiniana.org/libraries/pwmservo/
//https://cdn.shopify.com/s/files/1/0651/1795/products/LANPARTE_LA3D_GIMBAL-4.jpg?v=1451936576
//https://images.slideplayer.com/36/10563831/slides/slide_11.jpg
//https://github.com/Stan-Reifel/SpeedyStepper/tree/master/examples

/**********************************************************************
 * Lógica, ler o MPU6050, esperar estabilizar, converter xyz pra angulo,
  este angulo será colocado como posição incial do motor de passo (antes de tudo),
  mover o motor de passo pra 0graus,
  angulo = angulompu + angulo comandado,
  liberar pra uso
  *********************************************************************/

#include<Wire.h>
#include <SpeedyStepper.h>


const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265; //Ajuste fino do MPU
int maxVal = 402;

double x;
double y;
double z;

int xplus; //Incremento do eixo
int yplus;
//int zplus;

int xajust; //Correção do angulo do MPU e adicionando/subtraindo o incremento, tendendo a zerar.
int yajust;
//int zajust;

int xval; //Angulo a girar o servo
int yval;
//int zval;

/*SpeedyStep Config*/

const int LED_PIN = 13;
const int MOTOR_X_STEP_PIN = 3; 
const int MOTOR_X_DIRECTION_PIN = 4;
const int MOTOR_Y_STEP_PIN = 10;
const int MOTOR_Y_DIRECTION_PIN = 5;

SpeedyStepper stepperx;
SpeedyStepper steppery;
int stepperspeed = 800; //Velocidade dos motores de passo, constante
int stepperaccel = 100; //Aceleração dos motores de passo, constante

/*******************************************/

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);

  /* Leitura do incremento*/

  pinMode(A0, INPUT); //Incremento x
  pinMode(A1, INPUT); //Incremento y
  //pinMode(A2, INPUT); //Incrmento Z

  /*SpeedyStep Setup*/

  pinMode(LED_PIN, OUTPUT);   
  Serial.begin(9600);

  stepperx.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN); //Turret eixo x
  steppery.connectToPins(MOTOR_Y_STEP_PIN, MOTOR_Y_DIRECTION_PIN); //Turret eixo y  
}

/*******************************************/

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  /*
    Serial.print("AngleX= ");
    Serial.println(x);

    Serial.print("AngleY= ");
    Serial.println(y);

    Serial.print("AngleZ= ");
    Serial.println(z);
    Serial.println("-----------------------------------------");
    delay(400);
  */

  xplus = analogRead(A0); //Incremento x
  yplus = analogRead(A1); //Incremento y
  //zplus = analogRead(A2); //Incremento z

  int xajust = map(xplus, 0, 1023, -90, 90);
  int yajust = map(yplus, 0, 1023, -90, 90);
  //int zajust = map(zplus, 0, 1023,-90,90);
  
  /*Zerar Servos*/

  for (int i = 0; i <= 1; i++) {
    delay(500);
    void TURRETZERAR();
    delay(10);
  }

  /*Lógica correção MPU*/

  void TURRETLOOP();
  delay(10);
}

/***** Rotinas**************************/

void TURRETLOOP() {
  if (x > 0 || x < 0)
  {
    unsigned char angulox; //unsigned char dah valores de 0 a 255 inteiros
    angulox == (1.8* (x * (-1)) + xajust);
    stepperx.setSpeedInStepsPerSecond(stepperspeed); //colocar como constante o passo a velocidade e aceleração
    stepperx.setAccelerationInStepsPerSecondPerSecond(stepperaccel);
    stepperx.moveRelativeInSteps(angulox); //associar com o angulo do MPU ....
    delay(10); //testar pra ver se mantem ou deixa apenas no final do loop pr aexecutar os 2 juntos
  }
  if (y > 0 || y < 0)
  {
    unsigned char anguloy;
    anguloy == (1.8* (y * (-1)) + yajust);
    steppery.setSpeedInStepsPerSecond(800); //colocar como constante o passo a velocidade e aceleração
    steppery.setAccelerationInStepsPerSecondPerSecond(800);
    steppery.moveRelativeInSteps(anguloy); //associar com o angulo do MPU ....
    delay(10);
  }
    //delay(10);
}

void TURRETZERAR() {
  if (x > 0 || x < 0)
  {
    unsigned char angulox; //unsigned char dah valores de 0 a 255 inteiros
    angulox == (1.8* (x * (-1)));
    stepperx.setSpeedInStepsPerSecond(stepperspeed); //colocar como constante o passo a velocidade e aceleração
    stepperx.setAccelerationInStepsPerSecondPerSecond(stepperaccel);
    stepperx.moveRelativeInSteps(angulox); //associar com o angulo do MPU ....
    delay(10);
  }
  if (y > 0 || y < 0)
  {
    unsigned char anguloy; //unsigned char dah valores de 0 a 255 inteiros
    anguloy == (1.8* (y * (-1)));
    steppery.setSpeedInStepsPerSecond(stepperspeed); //colocar como constante o passo a velocidade e aceleração
    steppery.setAccelerationInStepsPerSecondPerSecond(stepperaccel);
    steppery.moveRelativeInSteps(anguloy); //associar com o angulo do MPU ....
    delay(10);
  }
    //delay(10);
}
