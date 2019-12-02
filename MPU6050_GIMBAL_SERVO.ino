//Written by Ahmet Burkay KIRNIK
//TR_CapaFenLisesi
//Measure Angle with a MPU-6050(GY-521)
//https://www.instructables.com/id/How-to-Measure-Angle-With-MPU-6050GY-521/
//https://www.usinainfo.com.br/blog/controle-de-posicao-do-micro-servo-motor-180/
//http://arduiniana.org/libraries/pwmservo/
//https://cdn.shopify.com/s/files/1/0651/1795/products/LANPARTE_LA3D_GIMBAL-4.jpg?v=1451936576
//https://images.slideplayer.com/36/10563831/slides/slide_11.jpg


// http://arduiniana.org/libraries/pwmservo/

//   Board                     SERVO_PIN_A   SERVO_PIN_B   SERVO_PIN_C
//   -----                     -----------   -----------   -----------
//   Arduino Uno, Duemilanove       9            10          (none)
//   Arduino Mega                  11            12            13
//   Sanguino                      13            12          (none)
//   Teensy 1.0                    17            18            15
//   Teensy 2.0                    14            15             4
//   Teensy++ 1.0 or 2.0           25            26            27
//   Teensy LC & 3.x                 (all PWM pins are usable)
////////////////////////////////////////////////////////////////////////

#include<Wire.h>
#include <PWMServo.h>

PWMServo myservo;  // create servo object to control a servo

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

/*Lógica, ler o MPU6050, esperar estabilizar, converter xyz pra angulo,
  este angulo será colocado como posição incial do servo (antes de tudo),
  mover o servo pra 0graus,
  angulo = angulompu + angulo comandado,
  liberar pra uso*/


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

  /* config dos servos*/

  myservo.attach(SERVO_PIN_A); //No mega eh o pino 11
  myservo.attach(SERVO_PIN_B); //No mega eh o pino 12
  //myservo.attach(SERVO_PIN_C); //No mega eh o pino 13
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
    void MPUZERAR();
    delay(10);
  }

  /*Lógica correção MPU*/

  void MPULOOP();
  delay(10);
}

/***** Rotinas**************************/

void MPULOOP() {
  if (x > 0 || x < 0)
  {
    myservo.write((x * (-1)) + xajust); // Posição que o servo fica
    //delay(10);                      // waits for the servo to get there
  }
  if (y > 0 || y < 0)
  {
    myservo.write((y * (-1)) + yajust); // Posição que o servo fica
    //delay(10);                      // waits for the servo to get there
  }
}


void MPUZERAR() {
  if (x > 0 || x < 0)
  {
    myservo.write(x * (-1)); // Zera x
    delay(10);
  }
  if (y > 0 || y < 0)
  {
    myservo.write(y * (-1)); // Zera y
    delay(10);
  }
}
