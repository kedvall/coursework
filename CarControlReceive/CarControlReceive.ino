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
#include "CarControlReceive.h" //Header file with various declarations

//Motor and H-Bridge pin setup
const int EN_LEFT     = 3; //PWM pin, enable12Pin
const int EN_RIGHT    = 5; //PWM pin, enable34Pin
const int motor1APin  = 2;
const int motor2APin  = 4;
const int motor3APin  = 6;
const int motor4APin  = 7;

//Other Variables
int yPosMapped;
int xPosMapped;

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
    //////////////////////
    // Car control loop //
    //////////////////////
    if ( (sonar.ping_in() < DIST_THRESHOLD) && (sonar.ping_in() != 0) ) //Check if value form ultrasonic sensor is smaller than threshold
    {
      if (dataPacket.btnState) //btnState has been toggled to TRUE
        reverseCar();
    } 
    else //Car is under ultrasonic threshold (far enough away from wall)
    {
      //////////////////////
      // Standard Driving //
      //////////////////////
      dir = directionCalc(dataPacket.xPos, dataPacket.yPos, dataPacket.btnState);

      if (dir != -1)
      {
        switch (dir) 
        {
          case 0: //deadZone
            digitalWrite(EN_LEFT, 0); //Disable left motor
            digitalWrite(EN_RIGHT, 0); //Disable right motor
            break;
          case 1: //posX
            //Map enable values
            xPosMapped = map(dataPacket.xPos, MAX_X+1, 1023, 0, 255); //As x increases, both en increase in positive direction
            //Use PWM to control enable
            digitalWrite(EN_LEFT, xPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;
          case 2: //negX
            //Map enable values
            xPosMapped = map(dataPacket.xPos, 0, MIN_X-1, 255, 0); //As x decreases, both en increase in negative direction
            //Use PWM to control enable
            digitalWrite(EN_LEFT, xPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;
          case 3: //posY
            //Map enable values
            yPosMapped = map(dataPacket.yPos, MAX_Y+1, 1023, 0, 255); //As y increases, left en increases in positive direction
            //Use PWM to control enable
            digitalWrite(EN_LEFT, yPosMapped);
            digitalWrite(EN_RIGHT, 0);
            break;
          case 4: //negY
            //Map enable values
            yPosMapped = map(dataPacket.yPos, 0, MIN_Y-1, 255, 0); //As y decreases, right en increases in positive direction
            //Use PWM to control enable
            digitalWrite(EN_LEFT, 0);
            digitalWrite(EN_RIGHT, yPosMapped);
            break;
          case 5: //posXposY
            //Map enable values
            xPosMapped = map(dataPacket.xPos, MAX_X+1, 1023, 0, 255); //As x increases, both en increase in positive direction
            yPosMapped = map(dataPacket.yPos, MAX_Y+1, 1023, 255, 0); //As y increases, right en decreases
            //Use PWM to control enable
            digitalWrite(EN_LEFT, xPosMapped);
            digitalWrite(EN_RIGHT, yPosMapped);
            break;
          case 6: //negXnegY
            //Map enable values
            xPosMapped = map(dataPacket.xPos, 0, MIN_X-1, 255, 0); //As x decreases, both en increase in negative direction
            yPosMapped = map(dataPacket.yPos, 0, MIN_Y-1, 0, 255); //As y decreases, left en decreases            
            //Use PWM to control enable
            digitalWrite(EN_LEFT, yPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;
          case 7: //posXnegY
            //Map enable values
            xPosMapped = map(dataPacket.xPos, MAX_X+1, 1023, 0, 255); //As x increases, both en increase in positive direction
            yPosMapped = map(dataPacket.yPos, 0, MIN_Y-1, 0, 255); //As y decreases, left en decreases
            //Use PWM to control enable
            digitalWrite(EN_LEFT, yPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;
          case 8: //negXposY
            //Map enable values
            xPosMapped = map(dataPacket.xPos, MAX_X+1, 1023, 0, 255); //As x decreases, both en increase in negative direction
            yPosMapped = map(dataPacket.yPos, MAX_Y+1, 1023, 255, 0); //As y increases, right en decreases
            //Use PWM to control enable
            digitalWrite(EN_LEFT, yPosMapped);
            digitalWrite(EN_RIGHT, xPosMapped);
            break;    
        } //End of switch statement
      } //End of btnState TRUE block
      else 
      {
        digitalWrite(EN_LEFT, 0); //Disable left motor
        digitalWrite(EN_RIGHT, 0); //Disable right motor
      } //End of btnState checking block
    } //End of ultrasonic checking block
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
    {
      joystickDir = posX; //Case 1: Joystick in positive X direction, move forward
      motorConfig = forwardConfig;
    }
    else if ( (xPos < MIN_X) && (yPos >= MIN_Y && yPos <= MAX_Y) )
    {
      joystickDir = negX; //Case 2: Joystick in negative X direction, move car backward
      motorConfig = reverseConfig;
    }
    else if ( (xPos >= MIN_X && xPos <= MAX_X) && (yPos > MAX_Y) )
    {
      joystickDir = posY; //Case 3: Joystick in positive Y direction, turn right
      motorConfig = reverseConfig;
    }
    else if ( (xPos >= MIN_X && xPos <= MAX_X) && (yPos < MIN_Y) )
    {
      joystickDir = negY; //Case 4: Joystick in negative Y direction, turn left
      motorConfig = reverseConfig;
    }
    //Diagonal Movements
    else if ( (xPos > MAX_X) && (yPos > MAX_Y) )
    {
      joystickDir = posXposY; //Case 5: Joystick in positive X and positive Y direction, move car forward and turn right
      motorConfig = forwardConfig;
    }
    else if ( (xPos < MIN_X) && (yPos < MIN_Y) )
    {
      joystickDir = negXnegY; //Case 6: Joystick in negative X and negative Y direction, move car backward and turn left
      motorConfig = reverseConfig;
    }
    else if ( (xPos > MAX_X) && (yPos < MIN_Y) )
    {
      joystickDir = posXnegY; //Case 7: Joystick in positive X direction and negative Y direction, move car forward and turn left
      motorConfig = forwardConfig;
    }
    else if ( (xPos < MIN_X) && (yPos > MAX_Y) )
    {
      joystickDir = negXposY; //Case 8: Joystick in negative X direction and positive Y direction, move car backward and turn right
      motorConfig = reverseConfig;
    }
    else
      joystickDir = deadZone; //Case 9: Joystick is in dead zone

    setHBridge(motorConfig);
    return joystickDir;
  } //End of direction calculation
  else
    return -1; //Return -1 if btnState is toggled to FALSE
} //End of directionCalc function

///////////////////////////////////////////////////////////////////////
// Function: setHBridge                                              //
// Sets motor control pins of H-Bridge based on direction            //
///////////////////////////////////////////////////////////////////////
void setHBridge(HBRIDGE_PINSET motorConfig)
{
  if (motorConfig)
  {
    digitalWrite(motor3APin, HIGH);
    digitalWrite(motor4APin, LOW);
    digitalWrite(motor1APin, LOW);
    digitalWrite(motor2APin, HIGH);
  }
  else
  {
    digitalWrite(motor3APin, LOW);
    digitalWrite(motor4APin, HIGH);
    digitalWrite(motor1APin, HIGH);
    digitalWrite(motor2APin, LOW);
  }
}

///////////////////////////////////////////////////////////////////////
// Function: reverseCar                                              //
// Accelerate car backward (When ultrasonic sensor detects objects)  //
// This will eventually be expanded to be dynamic based on speed     //
///////////////////////////////////////////////////////////////////////
void reverseCar()
{ 
  int reverseTime;
  int reverseSpeed;

  motorConfig = forwardConfig;
  setHBridge(motorConfig);

  //Reverse with increasing speed over 50 loops 
  for (reverseTime = 0; reverseTime < 50;  reverseTime++)
  {
    //Increase speed in negative direction as time increases
    reverseSpeed = map(reverseTime, 0, 50, 0, 255); //Scale counter to PWM output range
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
} //End of reverseCar function