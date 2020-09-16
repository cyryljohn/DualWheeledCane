#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_TCS34725.h>
#define MUX_Address 0x70 // TCA9548A Encoders address
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

#define motor1 6
#define motor2 9
char CS_L, CS_M, CS_R;
int Max_Motor_speed = 220;
int Min_Motor_speed = 0;
int Ave_Motor_speed = 120;
int Max1_Motor_speed = 210;

void tcaselect(uint8_t i2c_bus) {
  if (i2c_bus > 7) return;
  Wire.beginTransmission(MUX_Address);
  Wire.write(1 << i2c_bus);
  Wire.endTransmission();
}

char getColor(){
  uint16_t r, g, b, c;
  char color_detected;
  tcs.getRawData(&r, &g, &b, &c);
 Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
 

  if(r > g && r > b){
   Serial.print("[RED]");
    color_detected = 'r';
  }
  if(g > r && g > b){
   Serial.print("[GREEN]");
    color_detected = 'g';
  }
  if(b > r && b > g){
    Serial.print("[BLUE]");
    color_detected = 'b';
  }
  return color_detected;

   Serial.println(" ");
}
void setup() {
  Serial.begin(9600);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);

  Serial.println("Found Color Sensor");
  delay(3000);

  tcs.begin();
  tcaselect(6);
  tcs.begin();
  tcaselect(1);
  tcs.begin();
  tcaselect(5);
  tcs.begin();
}

void loop(void)
{
  tcaselect(6);
  Serial.print("LEFT -> ");
  CS_L = getColor();
  tcaselect(1);
  // delay(100);
  Serial.print("| MIDDLE -> ");
  CS_M = getColor();
  tcaselect(5);
  Serial.print("| RIGHT ->  ");
  CS_R = getColor();
  forward();


}

void forward(void)
{
  //motor1 = left motor , motor2 = right motor
  if (CS_L != 'r' && CS_M != 'r' && CS_R != 'r') // stop
  {
    Serial.println("-> STOP");
    analogWrite(motor1, Min_Motor_speed);
    analogWrite(motor2, Min_Motor_speed);
  }
  else if (CS_L != 'r' && CS_M != 'r' && CS_R == 'r') // right
  {
    Serial.println("-> RIGHT");
    analogWrite(motor1, Min_Motor_speed);
    analogWrite(motor2, Max_Motor_speed);
  }
  else if (CS_L != 'r' && CS_M == 'r' && CS_R != 'r') // Forward
  {
    Serial.println("-> FORWARD");
    analogWrite(motor1, Max1_Motor_speed);
    analogWrite(motor2, Max_Motor_speed);
  }
  else if (CS_L != 'r' && CS_M == 'r' && CS_R == 'r') //M.Right
  {
    Serial.println("-> RIGHT");
    analogWrite(motor1, Max1_Motor_speed);
    analogWrite(motor2, Ave_Motor_speed);
  }
  else if (CS_L == 'r' && CS_M != 'r' && CS_R != 'r') //Left
  {
    Serial.println("-> LEFT");
    analogWrite(motor1, Max1_Motor_speed);
    analogWrite(motor2, Min_Motor_speed);
  }
  else if (CS_L == 'r' && CS_M != 'r' && CS_R == 'r') //
  {
    Serial.println("-> STOP");
    analogWrite(motor1, Min_Motor_speed);
    analogWrite(motor2, Min_Motor_speed);
  }
  else if (CS_L == 'r' && CS_M == 'r' && CS_R != 'r')
  {
    Serial.println("-> LEFT");
    analogWrite(motor1, Ave_Motor_speed);
    analogWrite(motor2, Max1_Motor_speed);
  }
  else if (CS_L == 'r' && CS_M == 'r' && CS_R == 'r')
  {
    Serial.println("-> DESTINATION REACHED");
    analogWrite(motor1, Min_Motor_speed);
    analogWrite(motor2, Min_Motor_speed);
  }


}
