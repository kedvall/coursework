/**********************************************************************
*                                                                     *
* Control Software for ECE 110 Car                                    *
* Written by Kanyon Edvall and Mason Weigand                          *
*                                                                     *
* This program drives a wireless vehicle using joystick input         *
* Main components used: RF Receiver, Ultrasonic sensor, and H-Bridge  *
*                                                                     *
* This is the RF receiving and control loop part of the program!      *
*                                                                     *
**********************************************************************/
//Includes
#include <RH_ASK.h> //Library for RF communication
#include <SPI.h> //Not actually used but needed to compile
#include <NewPing.h> //Library for ultrasonic sonic sensor

//Defines
//Pins (Pin 11 is reserved for RF Receiver)
#define LED_PIN           13    //Internal LED pin to flash
#define TRIGGER_PIN       8     //Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN          9     //Arduino pin tied to echo pin on the ultrasonic sensor.
//Parameters
#define MAX_DISTANCE      100   //Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm
#define DIST_THRESHOLD    6     //Threshold distance for ultrasonic sensor
#define MIN_X             485   //Min x value to escape deadzone (for decreasing x) Defult: 485
#define MAX_X             501   //Max x value to escape deadzone (for increasing x) Defult: 502
#define MIN_Y             509   //Min y value to escape deadzone (for decreasing y) Defult: 509
#define MAX_Y             525   //Max y value to escape deadzone (for increasing y) Defult: 525
#define ENABLE_DEBUGGING  0  //Enable or disable USB serial debugging (set 1:TRUE or 0:FALSE)

//Motor and H-Bridge pin setup
const int EN_LEFT     = 3; //PWM pin, enable12Pin
const int EN_RIGHT    = 5; //PWM pin, enable34Pin
const int motor1APin  = 2;
const int motor2APin  = 4;
const int motor3APin  = 6;
const int motor4APin  = 7;

//Initialize stuct to store our data
struct dataStruct {
  int xPos;
  int yPos;
  bool btnState;
  long int packetsSent;
  } dataPacket;

//Enum to better organize travel directions
enum DIRECTION 
{
  deadZone, //Joystick in dead zone
  posX, //Joystick in positive X direction ONLY
  negX, //Joystick in negative X direction ONLY
  posY, //Joystick in positive Y direction ONLY
  negY, //Joystick in negative Y direction ONLY
  posXposY, //Joystick in positive X and positive Y direction
  negXnegY, //Joystick in negative X and negative Y direction
  posXnegY, //Joystick in positive X direction and negative Y direction
  negXposY  //Joystick in negative X direction and positive Y direction
} travelDir; //Declare var travelDir of type DIRECTION

//Other Variables
int yPosMapped;
int xPosMapped;
int reverseTime;
int reverseSpeed;

RH_ASK driver; //Initializes a driver to send/receive data
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //NewPing setup of pins and maximum distance.

///////////////////////////////////////////////////////////////////////
// Initial Setup:                                                    //
//    Starts serial debugging session at 9600 baud                   //
//    Checks for correct driver initialization                       //
//    Sets up pins for H-Brdige                                      //
///////////////////////////////////////////////////////////////////////
void setup() {
  //Serial console for debugging
  Serial.begin(9600); //Start serial debugging session at 9600 baud
  Serial.println("Init setup");
  if (!driver.init())
  Serial.println("Init failed");

  //H-Bridge enable pins
  pinMode(EN_RIGHT, OUTPUT); //right motor enable pin
  pinMode(EN_LEFT, OUTPUT); //left motor enable pin
  //Left motor H-Bridge - 3 High 4 Low = Forward
  pinMode(motor3APin, OUTPUT);
  pinMode(motor4APin, OUTPUT);
  //Right motor H-Bridge - 1 High 2 Low = Forward
  pinMode(motor1APin, OUTPUT);
  pinMode(motor2APin, OUTPUT);
} //End of setup

///////////////////////////////////////////////////////////////////////
// Main control loop:                                                //
//    Reads data from RF Receiver and updates struct variables       //
//    Reads data from Ultrasonic sensors for obstacle avoidance      //
//    Drives H-Bridge using PWM to control motors and drive car      //
//    Made assuming: (Verify to be correct)                          //
//        enable12Pin = left motor enable pin (EN_LEFT)              //
//        enable34Pin = right motor enable pin (EN_RIGHT)            //
//        motor3APin and motor4APin = left motor control             //
//        motor1APin and motor2APin = right motor control            //
///////////////////////////////////////////////////////////////////////
void loop() 
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN]; //Buffer for receiving data
  uint8_t buflen = sizeof(buf); //Length of buffer
  int dir;

  if (driver.recv(buf, &buflen)) //Check if RF Receiver if receiving data (non-blocking)
  {
    //Good checksum, dump message;
    memcpy(&dataPacket, buf, sizeof(dataPacket)); //Reserve memory for received data (based on size of data)

    if (ENABLE_DEBUGGING) //Print to serial console for debugging
    {
      Serial.print("Ping: ");
      Serial.print(sonar.ping_in());
      Serial.print(" xPos: ");
      Serial.print(dataPacket.xPos);
      Serial.print("  yPos: ");
      Serial.print(dataPacket.yPos);
      Serial.print("  btnState: ");
      Serial.print(dataPacket.btnState);
      Serial.print(" packetsSent: ");
      Serial.println(dataPacket.packetsSent);
    }

    // Car control loop
    if (dataPacket.btnState) //If btnState is true execute car control loop. This way the car starts disabled (in "safe mode").
    {
      digitalWrite(LED_PIN, LOW); //Turn off "safe mode" LED
      //////////////////////
      // Reverse Function //
      //////////////////////
      if ( (sonar.ping_in() < DIST_THRESHOLD) && (sonar.ping_in() != 0) ) //Check if value form ultrasonic sensor is smaller than threshold
      {
        //Reverse with increasing speed over 1.5 seconds
        for (reverseTime = 0; reverseTime < 50;  reverseTime++)
        {
          //Increase speed in negative direction as time increases
          reverseSpeed = map(reverseTime, 0, 50, 0, 255); //Scale counter to PWM output range
          //Reverse config
          digitalWrite(motor3APin, HIGH);
          digitalWrite(motor4APin, LOW);
          digitalWrite(motor1APin, LOW);
          digitalWrite(motor2APin, HIGH);
          //Use PWM to control enable
          digitalWrite(EN_LEFT, reverseSpeed);
          digitalWrite(EN_RIGHT, reverseSpeed);

          if (ENABLE_DEBUGGING) //Print to serial console for debugging
          {
            Serial.print("Ping: ");
            Serial.print(sonar.ping_in());
            Serial.print(" Speed: ");
            Serial.println(reverseSpeed);
          }
        }
        //Stop reversing
        digitalWrite(EN_LEFT, 0);
        digitalWrite(EN_RIGHT, 0);
        delay(250);
      } else //Car is under ultrasonic threshold (far enough away from wall)
      {
        //////////////////////
        // Standard Driving //
        //////////////////////
        dir = directionCalc(dataPacket.xPos, dataPacket.yPos, dataPacket.btnState);

        switch (dir) 
        {
          case 0: //deadZone
            digitalWrite(EN_LEFT, 0); //Disable left motor
            digitalWrite(EN_RIGHT, 0); //Disable right motor
            break;
          case 1: //posX
            //Map enable values
            xPosMapped = map(dataPacket.xPos, 498, 1023, 0, 255); //As x increases, both en increase in positive direction
            //Forward config
            digitalWrite(motor3APin, HIGH);
            digitalWrite(motor4APin, LOW);
            digitalWrite(motor1APin, LOW);
            digitalWrite(motor2APin, HIGH);
            //Use PWM to control enable
            digitalWrite(EN_LEFT, xPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;
          case 2: //negX
            //Map enable values
            xPosMapped = map(dataPacket.xPos, 0, 489, 255, 0); //As x decreases, both en increase in negative direction
            //Reverse config
            digitalWrite(motor3APin, LOW);
            digitalWrite(motor4APin, HIGH);
            digitalWrite(motor1APin, HIGH);
            digitalWrite(motor2APin, LOW);
            //Use PWM to control enable
            digitalWrite(EN_LEFT, xPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;
          case 3: //posY
            //Map enable values
            yPosMapped = map(dataPacket.yPos, 521, 1023, 0, 255); //As y increases, left en increases in positive direction
            //Forward config
            digitalWrite(motor3APin, LOW);
            digitalWrite(motor4APin, HIGH);
            digitalWrite(motor1APin, HIGH);
            digitalWrite(motor2APin, LOW);
            //Use PWM to control enable
            digitalWrite(EN_LEFT, yPosMapped);
            digitalWrite(EN_RIGHT, 0);
            break;
          case 4: //negY
            //Map enable values
            yPosMapped = map(dataPacket.yPos, 0, 513, 255, 0); //As y decreases, right en increases in positive direction
            //Forward config
            digitalWrite(motor3APin, LOW);
            digitalWrite(motor4APin, HIGH);
            digitalWrite(motor1APin, HIGH);
            digitalWrite(motor2APin, LOW);
            //Use PWM to control enable
            digitalWrite(EN_LEFT, 0);
            digitalWrite(EN_RIGHT, yPosMapped);
            break;
          case 5: //posXposY
            //Map enable values
            xPosMapped = map(dataPacket.xPos, 498, 1023, 0, 255); //As x increases, both en increase in positive direction
            yPosMapped = map(dataPacket.yPos, 521, 1023, 255, 0); //As y increases, right en decreases
            //Forward config
            digitalWrite(motor3APin, HIGH);
            digitalWrite(motor4APin, LOW);
            digitalWrite(motor1APin, HIGH);
            digitalWrite(motor2APin, LOW);
            //Use PWM to control enable
            digitalWrite(EN_LEFT, xPosMapped);
            digitalWrite(EN_RIGHT, yPosMapped);
            break;
          case 6: //negXnegY
            //Map enable values
            xPosMapped = map(dataPacket.xPos, 0, 489, 255, 0); //As x decreases, both en increase in negative direction
            yPosMapped = map(dataPacket.yPos, 0, 513, 0, 255); //As y decreases, left en decreases
            //Reverse config
            digitalWrite(motor3APin, LOW);
            digitalWrite(motor4APin, HIGH);
            digitalWrite(motor1APin, LOW);
            digitalWrite(motor2APin, HIGH);
            //Use PWM to control enable
            digitalWrite(EN_LEFT, yPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;
          case 7: //posXnegY
            //Map enable values
            xPosMapped = map(dataPacket.xPos, 498, 1023, 0, 255); //As x increases, both en increase in positive direction
            yPosMapped = map(dataPacket.yPos, 0, 513, 0, 255); //As y decreases, left en decreases
            //Forward config
            digitalWrite(motor3APin, HIGH);
            digitalWrite(motor4APin, LOW);
            digitalWrite(motor1APin, HIGH);
            digitalWrite(motor2APin, LOW);
            //Use PWM to control enable
            digitalWrite(EN_LEFT, yPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;
          case 8: //negXposY
            //Map enable values
            xPosMapped = map(dataPacket.xPos, 498, 1023, 0, 255); //As x decreases, both en increase in negative direction
            yPosMapped = map(dataPacket.yPos, 521, 1023, 255, 0); //As y increases, right en decreases
            //Reverse config
            digitalWrite(motor3APin, LOW);
            digitalWrite(motor4APin, HIGH);
            digitalWrite(motor1APin, HIGH);
            digitalWrite(motor2APin, LOW);
            //Use PWM to control enable
            digitalWrite(EN_LEFT, yPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;    
        } //End of switch statement
      } //End of ultrasonic checking block
    } //End of btnState checking block
  } //End of RF checking block
} //End of main control loop

///////////////////////////////////////////////////////////////////////
// Function: directionCalc                                           //
// Finds and returns the region joystick is in (NULL if deadzone)    //
// Nine possible cases:                                              //
//    Dead zone, Xinc, Xdec, Yinc, Ydec,                             //
//    Xinc & Yinc, Xdec & Ydec, Xinc & Ydec, Xdec & Yinc             //
// Map to: +X (Forward) -X (Reverse) +Y (Turn right) -Y (Turn left)  //
///////////////////////////////////////////////////////////////////////
int directionCalc(int xPos, int yPos, bool btnState) 
{
  if (btnState) //btnState has been toggled to TRUE
  {
    //Standard Movements
    if ( (xPos > MAX_X) && (yPos >= MIN_Y && yPos <= MAX_Y) )
      travelDir = posX; //Case 1: Joystick in positive X direction, move forward
    else if ( (xPos < MIN_X) && (yPos >= MIN_Y && yPos <= MAX_Y) )
      travelDir = negX; //Case 2: Joystick in negative X direction, move car backward
    else if ( (xPos >= MIN_X && xPos <= MAX_X) && (yPos > MAX_Y) )
      travelDir = posY; //Case 3: Joystick in positive Y direction, turn right
    else if ( (xPos >= MIN_X && xPos <= MAX_X) && (yPos < MIN_Y) )
      travelDir = negY; //Case 4: Joystick in negative Y direction, turn left
    //Diagonal Movements
    else if ( (xPos > MAX_X) && (yPos > MAX_Y) )
      travelDir = posXposY; //Case 5: Joystick in positive X and positive Y direction, move car forward and turn right
    else if ( (xPos < MIN_X) && (yPos < MIN_Y) )
      travelDir = negXnegY; //Case 6: Joystick in negative X and negative Y direction, move car backward and turn left
    else if ( (xPos > MAX_X) && (yPos < MIN_Y) )
      travelDir = posXnegY; //Case 7: Joystick in positive X direction and negative Y direction, move car forward and turn left
    else if ( (xPos < MIN_X) && (yPos > MAX_Y) )
      travelDir = negXposY; //Case 8: Joystick in negative X direction and positive Y direction, move car backward and turn right
    else
      travelDir = deadZone; //Case 9: Joystick is in dead zone
    return travelDir;
  } //End of direction calculation
  else
    return NULL; //Return NULL if btnState is toggled to FALSE
} //End of directionCalc function