/**********************************************************************
*                                                                     *
* Header file for CarControlReceive.ino                               *
* Written by Kanyon Edvall					                          *
*                                                                     *
* This header file was created to satisfy Arduino's requirement for   *
* enums to be declared seperately from the main sketch as noted here: *
* - http://www.arduino.cc/en/Hacking/BuildProcess					  *
* - http://stackoverflow.com/questions/18158136/					  *
*		why-cant-i-pass-typedef-or-enum-in-arduino     				  *
*                                                                     *
**********************************************************************/
//Defines
//Pins (Pin 11 is reserved for RF Receiver)
#define LED_PIN           13    //Internal LED pin to flash
#define TRIGGER_PIN       9     //Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN          10    //Arduino pin tied to echo pin on the ultrasonic sensor.
#define R_INTERRUPT_PIN   2     //Arduino pin for input from right encoder
#define L_INTERRUPT_PIN   3     //Arduino pin for input from left encoder
//Parameters
#define MAX_DISTANCE      100   //Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm
#define DIST_THRESHOLD    6     //Threshold distance for ultrasonic sensor
#define ENCODER_DELAY     2     //Number (in milliseconds) to wait before reading new encoder value. Default: 2ms = 500Hz
#define MIN_X             481   //Min x value to escape deadzone (for decreasing x) Defult: 485
#define MAX_X             497   //Max x value to escape deadzone (for increasing x) Defult: 502
#define MIN_Y             509   //M\in y value to escape deadzone (for decreasing y) Defult: 509
#define MAX_Y             525   //Max y value to escape deadzone (for increasing y) Defult: 525
#define ENABLE_DEBUGGING  0  //Enable or disable USB serial debugging (set 1:TRUE or 0:FALSE)

//Initialize stuct to store our data
struct dataStruct {
  int xPos;
  int yPos;
  bool btnState;
  unsigned long int packetsSent;
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
} joystickDir; //Declare var joystickDir of type DIRECTION

//Enum for H-Bridge motor control pin setup
enum HBRIDGE_PINSET
{
  reverseConfig,
  forwardConfig
} motorConfig;