/*
 *  Resistor values used for this program:
 *  
 *  Button 1/OK Button = 1k ohms
 *  Button 2/NEXT Button = 470 ohms
 *  Button 3/PREV Button = 330 ohms
 *  Button 4/CANCEL Button = 250 ohms
 * 
 *  Change the range based sa values ng bawat resistor
 */
 
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_TCS34725.h>
#define MUX_Address 0x70 // TCA9548A Encoders address
#define pirPin 13 // PIR Output Pin
#define echoPin 2  // Ultrasonic 1 Echo Pin
#define trigPin 3  // Ultrasonic 1 Trigger Pin
#define button1 A3 // Analog pin for Braille Button
#define motor1 9 // Motor 1
#define motor2 10 // Motor 2

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

//~~~~ DFPlayer mini MP3 Declaration
//declaration/initialization for DFplayer mini MP3


//~~~~ Obstacle Sensors Declaration
int distance; //Ultrasonic Distance
long duration; //Ultrasonic Duration
int pirStat = 0; // PIR status

//~~~~ Braille Button Declaration
int buttonState;
int lastButtonState;
int currentButtonState;
int room_ctr = 1; //counter for rooms
int printFlag = 0; 
int flag = 0;

//~~~~ Color Sensors Declaration

char ColorSensor_1, ColorSensor_2;
//char ColorSensor_3; //for color sensor 3


void tcaselect(uint8_t i2c_bus) {
  if (i2c_bus > 7) return;
  Wire.beginTransmission(MUX_Address);
  Wire.write(1 << i2c_bus);
  Wire.endTransmission();
}

char getColor()
{

   // still need calibration based on the colored tapes or colors to be used
    float r, g, b, y, o, s, v;
    tcs.getRGB(&r, &g, &b);
    char color_detected;
/*
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" "); */
    Serial.print(" ");

     if (r >= 170 && r <= 190 && g >= 35 && g <= 55 && b >= 35 && b <= 60)
     {
        Serial.print("[RED] ");
        color_detected = 'r';
     }
     
   if (r >= 57 && r <= 85 && g >= 105 && g<= 128 && b >= 50 && b <= 85)
     {
        Serial.print("[GREEN] ");
        color_detected = 'g';
     }

   if (r >= 57 && r <= 85 && g >= 70 && g <= 94 && b >= 89 && b <= 122)
     {
        Serial.print("[BLUE] ");
        color_detected = 'b';
     }

   if (r >= 105 && r <= 125 && g >= 80 && g <= 105 && b >= 27 && b <= 57)
     {
        Serial.print("[YELLOW] ");
        color_detected = 'y';
     }

   if (r >= 138 && r <= 158 && g >= 56 && g <= 77 && b >= 24 && b <= 54)
     {
        Serial.print("[ORANGE] ");
        color_detected = 'o';
     }
     
   if (r >= 39 && r <= 70 && g >= 80 && g <= 110 && b >= 90 && b <= 122)
     {
       Serial.print("[SKY BLUE] ");
        color_detected = 's';
     }
     
   if (r >= 92 && r <= 112 && g >= 61 && g <= 81 && b >= 70 && b <= 100)
     {
       Serial.print("[VIOLET] ");
        color_detected = 'v';
     }

    if (r >= 137 && r <= 168 && g >= 98 && g <= 120 && b >= 91 && b <= 118)
     {
       Serial.print("[CLEAR] ");
        color_detected = 'c';
     }

    return color_detected;
     Serial.print(" ");

}



void setup()
{
   Serial.begin(9600);

   Serial.println("Found Color Sensor");
   delay(1500);
   Serial.println("~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ");
   
/*
   tcs.begin();    // change it based sa ginamit na pins
   tcaselect(4);*/ // Sets color sensor to pin SCL4 and SDA4 of the multiplexer
   tcs.begin(); 
   tcaselect(6); // Sets color sensor to pin SCL6 and SDA6 of the multiplexer
   tcs.begin();
   tcaselect(5); // Sets color sensor to pin SCL5 and SDA5 of the multiplexer
   tcs.begin();

   pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
   pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
   
   pinMode(motor1, OUTPUT); // Sets the motor1 as an OUTPUT
   pinMode(motor2, OUTPUT);// Sets the motor2 as an OUTPUT
   
   analogWrite(motor1, 0); // Sets the motor 1 to LOW/0
   analogWrite(motor2, 0); // Sets the motor 1 to LOW/0
   
   currentButtonState = analogRead(button1); // reads the current state of the button1
  
}


void loop()
{
  /*  Sample Color Room Combination:
   *   For this program, 2 color sensor lang gamit ko
   *   Room A = Green-Yellow-Green
   *   Room B = Yellow-Green-Yellow
   *   Room C = Red-Red-Red
   *   Room D = Green-Green-Green
   *   Room E = Blue-Blue-Blue
   */

  if (flag == 0)
  {
      color_room_recognition(); //function to identify the color room combination/pattern
      flag = 1;
  }

}



void color_room_recognition()
{
   char A, B, C, D, E;
   char current_room_location;
   Serial.println("Please indicate current location!");
   Serial.println("- - - - - - - - - - - - - - - - - - ");
 
   
   while(true)
     { 
         tcaselect(6); 
         Serial.print("ColorSensor_1: ");
         ColorSensor_1 = getColor();
         tcaselect(5);
         Serial.print("ColorSensor_2: ");
         ColorSensor_2 = getColor();
         delay(500);
        Serial.println(" ");
        
     //sample lang to, kunyare may color combination per room
     //edit the code for three color sensor
      
         if (ColorSensor_1 == 'g' && ColorSensor_2 == 'y') 
         {
            Serial.println("- - - - - - - - - - - - - - ");
            Serial.println("You are in Room A!");
            current_room_location = 'A'; 
            Serial.println("- - - - - - - - - - - - - - ");
            while(true)
            {
                //call the function
                room_initialization(); 

                //when the OK button is pressed, proceed to the room function
                if(currentButtonState <= 200 && currentButtonState >= 0 && lastButtonState >= 1000 && lastButtonState <= 1040)  //OK button
                    {
                      //room_ctr 1 = Room A, room_ctr 2 = Room B, room_ctr 3 = Room C, room_ctr 4 = Room D, room_ctr 5 = Room E 
                      if (room_ctr == 2)
                       {
                          roomA_to_roomB(); 
                       }

                      else if (room_ctr == 3)
                       {
                          roomA_to_roomC();
                       }

                      else if (room_ctr == 4)
                       {
                          roomA_to_roomD();
                       }

                      else if (room_ctr == 5)
                       {
                          roomA_to_roomE();
                       }     
                    }
            }
            break;
         }
  
         if (ColorSensor_1 == 'y' && ColorSensor_2 == 'g')
         {
            Serial.println("- - - - - - - - - - - - - - ");
            Serial.println("You are in Room B!");
            current_room_location = 'B'; 
            Serial.println("- - - - - - - - - - - - - - ");
            while(true)
            {
                 //call the function
                room_initialization(); 
                
                //when the OK button is pressed, proceed to the room function
                if(currentButtonState <= 200 && currentButtonState >= 0 && lastButtonState >= 1000 && lastButtonState <= 1040)  //OK button
                    {
                      //room_ctr 1 = Room A, room_ctr 2 = Room B, room_ctr 3 = Room C, room_ctr 4 = Room D, room_ctr 5 = Room E 
                      if (room_ctr == 1)
                       {
                          roomB_to_roomA(); 
                       }

                      else if (room_ctr == 3)
                       {
                          roomB_to_roomC();
                       }

                      else if (room_ctr == 4)
                       {
                          roomB_to_roomD();
                       }

                      else if (room_ctr == 5)
                       {
                          roomB_to_roomE();
                       }     
                    }
            }
            break;
         }
  
         if (ColorSensor_1 == 'r' && ColorSensor_2 == 'r')
         {
            Serial.println("- - - - - - - - - - - - - - ");
            Serial.println("You are in Room C!");
            current_room_location = 'C'; 
            Serial.println("- - - - - - - - - - - - - - ");
           while(true)
           {
                //call the function
                room_initialization(); 

                //when the OK button is pressed, proceed to the room function
                if(currentButtonState <= 200 && currentButtonState >= 0 && lastButtonState >= 1000 && lastButtonState <= 1040)  //OK button
                    {
                      //room_ctr 1 = Room A, room_ctr 2 = Room B, room_ctr 3 = Room C, room_ctr 4 = Room D, room_ctr 5 = Room E 
                      if (room_ctr == 1)
                       {
                          roomC_to_roomA(); 
                       }

                      else if (room_ctr == 2)
                       {
                          roomC_to_roomB();
                       }

                      else if (room_ctr == 4)
                       {
                          roomC_to_roomD();
                       }

                      else if (room_ctr == 5)
                       {
                          roomC_to_roomE();
                       }     
                    }
            }
            break;
         }
  
         if (ColorSensor_1 == 'g' && ColorSensor_2 == 'g')
         {
            Serial.println("- - - - - - - - - - - - - - ");
            Serial.println("You are in Room D!");
            current_room_location = 'D'; 
            Serial.println("- - - - - - - - - - - - - - ");
            while(true)
           {
                //call the function
                room_initialization(); 
                
               //when the OK button is pressed, proceed to the room function
                if(currentButtonState <= 200 && currentButtonState >= 0 && lastButtonState >= 1000 && lastButtonState <= 1040) //OK button
                    {
                      //room_ctr 1 = Room A, room_ctr 2 = Room B, room_ctr 3 = Room C, room_ctr 4 = Room D, room_ctr 5 = Room E 
                      if (room_ctr == 1)
                       {
                          roomD_to_roomA(); 
                       }

                      else if (room_ctr == 2)
                       {
                          roomD_to_roomB();
                       }

                      else if (room_ctr == 3)
                       {
                          roomD_to_roomC();
                       }

                      else if (room_ctr == 5)
                       {
                          roomD_to_roomE();
                       }     
                    }
            }
            break;
         }
  
          if (ColorSensor_1 == 'b' && ColorSensor_2 == 'b')
         {
            Serial.println("- - - - - - - - - - - - - - ");
            Serial.println("You are in Room E!");
            current_room_location = 'E'; 
            Serial.println("- - - - - - - - - - - - - - ");
            while(true)
           {
                 //call the function
                 room_initialization(); 
                 
                //when the OK button is pressed, proceed to the room function
                if(currentButtonState <= 200 && currentButtonState >= 0 && lastButtonState >= 1000 && lastButtonState <= 1040) //OK button
                    {
                      //room_ctr 1 = Room A, room_ctr 2 = Room B, room_ctr 3 = Room C, room_ctr 4 = Room D, room_ctr 5 = Room E 
                      if (room_ctr == 1)
                       {
                          roomE_to_roomA(); 
                       }

                      else if (room_ctr == 2)
                       {
                          roomE_to_roomB();
                       }

                      else if (room_ctr == 3)
                       {
                          roomE_to_roomC();
                       }

                      else if (room_ctr == 4)
                       {
                          roomE_to_roomD();
                       }     
                    }
            }
            break;
         }
         

   }
       return current_room_location; // return the current room location of the user
      
         Serial.print(" ");
}


void room_initialization()
{
  //this function is for the braille button control, for choosing the target location

  lastButtonState = currentButtonState;
  currentButtonState = analogRead(button1);
 
  if(printFlag == 0)
  {
    switch(room_ctr)
    {
    case 1:
      Serial.println("Do you want to go to Room A?");
      break;
    case 2:
      Serial.println("Do you want to go to Room B?");
      break;
    case 3:
      Serial.println("Do you want to go to Room C?");
      break;
    case 4:
      Serial.println("Do you want to go to Room D?");

      break;
    case 5:
      Serial.println("Do you want to go to Room E?");
      break;
    default:
      break;
    }

    printFlag = 1;
  }

 //this is the button range, just change the values/range based on the resistor you used
  if(currentButtonState >= 310 && currentButtonState <= 400 && lastButtonState >= 1000 && lastButtonState <= 1040) //NEXT button
  {
    room_ctr++;
    printFlag = 0;
  }
  
  if(currentButtonState >= 220 && currentButtonState <= 300 && lastButtonState >= 1000 && lastButtonState <= 1040) //PREV button
  {
    room_ctr--;
    printFlag = 0; 
  }
  delay(100);
}


void motor_forward()
{ 
    //Both motors HIGH
    analogWrite(motor1, 200);
    analogWrite(motor2, 200);  
}

void motor_stop()
{
   //Both motors LOW
    analogWrite(motor1, 0);
    analogWrite(motor2, 0);  
}

void motor1_stop()
{
    //Motor 1 LOW, Motor 2 HIGH
    analogWrite(motor1, 0);
    analogWrite(motor2, 200);  
}

void motor2_stop()
{
    //Motor 2 LOW, Motor 1 HIGH
    analogWrite(motor1, 200);
    analogWrite(motor2, 0);  
}

int obstacle_main()
{
     //this function is for obstaccle sensors
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin, HIGH);
      // Calculating the distance
      distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
      // Displays the distance on the Serial Monitor


      if (distance<=20 || pirStat == HIGH)
      {
          Serial.println("OBSTACLE DETECTED!");  //MP3 player track 0018
      }

       else
      {
          
          Serial.println("NONE!");
      }
}


//~~~~~~~ ROOM A FUNCTIONS ~~~~~~~
void roomA_to_roomB() 
{


   //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room A going to Room B");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomA_to_roomC()
{
     //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room A going to Room C");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomA_to_roomD()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room A going to Room D");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomA_to_roomE()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room A going to Room E");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

//~~~~~~~ ROOM B FUNCTIONS ~~~~~~~

void roomB_to_roomA()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room B going to Room A");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomB_to_roomC()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room B going Room C");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomB_to_roomD()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room B going to Room D");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomB_to_roomE()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room B going to Room E");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}


//~~~~~~~ ROOM C FUNCTIONS ~~~~~~~

void roomC_to_roomA()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room C going to Room A");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomC_to_roomB()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room C going to Room B");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomC_to_roomD()
{
   //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room C going to Room D");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomC_to_roomE()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room C going to Room E");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

//~~~~~~~ ROOM D FUNCTIONS ~~~~~~~

void roomD_to_roomA()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room D going to Room A");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomD_to_roomB()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room D going to Room B");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomD_to_roomC()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room D going to Room C");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomD_to_roomE()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room D going to Room E");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

//~~~~~~~ ROOM E FUNCTIONS ~~~~~~~

void roomE_to_roomA()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room E going to Room A");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomE_to_roomB()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room E going to Room B");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomE_to_roomC()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room E going to Room C");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}

void roomE_to_roomD()
{
      //change Serial Output with mp3 voice output 
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("Room E going to Room D");
   Serial.println("- - - - - - - - - - - - - - ");
   Serial.println("MOTOR FORWARD");
     do
       {
         obstacle_main();

         if (distance>20 && pirStat != HIGH)
         {
          //function for main algo
            motor_forward(); 
            tcaselect(6);
            Serial.print("ColorSensor_1: ");
            ColorSensor_1 = getColor();
            tcaselect(5);
            Serial.print("ColorSensor_2: ");
            ColorSensor_2 = getColor();
            Serial.println(" ");
            delay(500);
         }

         else
         {
            motor_stop();
            Serial.println("Motor Stop!");
         }
         
        }while(ColorSensor_1 != 'v' && ColorSensor_2 != 's');
        
       motor_stop();
       Serial.println("- - - - - - - - - - - - - - ");
       Serial.println("STOP!");
       Serial.println("- - - - - - - - - - - - - - ");
        
}
