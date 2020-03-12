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
#define ENABLE_DEBUGGING  TRUE  //Enable or disable USB serial debugging (set TRUE or FALSE)

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

//Other Variables
int yPosMapped;
int xPosMapped;
int reverseTime;
int reverseSpeed;

RH_ASK driver; //Initializes a driver to send/receive data
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //NewPing setup of pins and maximum distance.

///////////////////////////////////////////////////////////////////////
// Initial Setup                                                     //
// Starts serial debugging session at 9600 baud                      //
// Checks for correct driver initialization                          //
// Sets up pins for H-Brdige                                         //
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
// Main control loop                                                 //
//    Reads data from RF Receiver and updates struct variables       //
//    Reads data from Ultrasonic sensors for obstacle avoidance      //
//    Drives H-Bridge using PWM to control motors and drive car      //
//    Made assuming: (Verify to be correct)                          //
//        enable12Pin = left motor enable pin (EN_LEFT)              //
//        enable34Pin = right motor enable pin (EN_RIGHT)            //
//        motor3APin and motor4APin = left motor control             //
//        motor1APin and motor2APin = right motor control            //
///////////////////////////////////////////////////////////////////////
void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN]; //Buffer for receiving data
  uint8_t buflen = sizeof(buf); //Length of buffer

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

      }
      //////////////////////
      // Standard Driving //
      //////////////////////
      else //Car is under ultrasonic threshold (far enough away from wall)
      {
        //Nine possible cases:
        //Xinc, Xdec, Yinc, Ydec, Xinc & Yinc, Xdec & Ydec, Xinc & Ydec, Xdec & Yinc, Dead zone
        //+X = Forward, -X = Reverse, +Y = Turn right, -Y = Turn left
        if ( (dataPacket.xPos > MAX_X) && (dataPacket.yPos >= MIN_Y && dataPacket.yPos <= MAX_Y) )
          //Case 1: Xinc, move car forward
        {
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
        } else if ( (dataPacket.xPos < MIN_X) && (dataPacket.yPos >= MIN_Y && dataPacket.yPos <= MAX_Y) )
          //Case 2: Xdec, move car backward
        {
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
        } else if ( (dataPacket.xPos >= MIN_X && dataPacket.xPos <= MAX_X) && (dataPacket.yPos < MIN_Y) )
          //Case 3: Yinc, turn right
        {
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
        } else if ( (dataPacket.xPos >= MIN_X && dataPacket.xPos <= MAX_X) && (dataPacket.yPos > MAX_Y) )
          //Case 4: Ydec, turn left
        {
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

          //Diagonal Movements - Temporarily disabled due to sporadic movement
          /* Uncomment to enable
          } else if ( (dataPacket.xPos > MAX_X) && (dataPacket.yPos > MAX_Y) )
            //Case 5: Xinc & Yinc, move car forward and turn right
          {
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
          } else if ( (dataPacket.xPos < MIN_X) && (dataPacket.yPos < MIN_Y) )
            //Case 6: Xdec & Ydec, move car backward and turn left
          {
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
          } else if ( (dataPacket.xPos > MAX_X) && (dataPacket.yPos < MIN_Y) )
            //Case 7: Xinc & Ydec
          {
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
          } else if ( (dataPacket.xPos < MIN_X) && (dataPacket.yPos > 520) )
            //Case 8: Xdec & Yinc
          {
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
          */
        } else
          //Case 9: Dead zone, stop car
          //Range (for reference) = ( (dataPacket.xPos >= 490 && dataPacket.xPos <= 497) && (dataPacket.yPos >= 514 && dataPacket.yPos <= 520) )
        {
          digitalWrite(EN_LEFT, 0);
          digitalWrite(EN_RIGHT, 0);
        }
      } //End of ultrasonic checking block
    } else
    {
      digitalWrite(EN_LEFT, 0); //Disable left motor
      digitalWrite(EN_RIGHT, 0); //Disable right motor
      digitalWrite(LED_PIN, HIGH); //Turn "safe mode" LED on
    } //End of btnState checking block
  } //End of RF checking block
} //End of main control loop
