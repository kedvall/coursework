/**********************************************************************
*                                                                     *
* Control Software for ECE 110 Car                                    *
* Written by Kanyon Edvall and Mason Weigand                          *
*                                                                     *
* This program drives a wireless vehicle using joystick input         *
* Main components used: RF Receiver, Ultrasonic sensor, and H-Bridge  *
*                                                                     *
* This is the RF transmitting and joystick input part of the program! *
*                                                                     *
**********************************************************************/
//Includes
#include <RH_ASK.h> //Library for RF communication
#include <SPI.h> // Not actually used but needed to compile
#include <Wire.h> //Wire framework

//Defines
#define LED_PIN 13
#define BUTTON_PIN A0
#define Y_PIN A1
#define X_PIN A2

//Initialize stuct to store our data
struct dataStruct {
  int xPos;
  int yPos;
  bool btnState;
  long int packetsSent;
} dataPacket;

byte tx_buf[sizeof(dataPacket)] = {0}; //Create buffer the size of our struct var

RH_ASK driver; //Initializes a driver to send/receive data

///////////////////////////////////////////////////////////////////////
// Initial Setup                                                     //
//    Starts serial debugging session at 9600 baud                   //
//    Checks for correct driver initialization                       //
//    Sets up pins for joystick input                                //
///////////////////////////////////////////////////////////////////////
void setup() {
  //Serial console for debugging
  Serial.begin(9600);
  Serial.println("Init setup");
  if (!driver.init())
    Serial.println("Init failed");
    
  //PinMode setup
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
} //End of setup

///////////////////////////////////////////////////////////////////////
// Main control loop                                                 //
//    Reads data from joystick and updates stuct variables           //
//    Sends data using RF Transmitter                                //
//    Tracks number of packets sent                                  //
///////////////////////////////////////////////////////////////////////
void loop() {
  //Get state of joystick
  if (analogRead(BUTTON_PIN) == 0) //Check if button is pressed
    dataPacket.btnState = !dataPacket.btnState; //Toggle bool btnState
  dataPacket.xPos = analogRead(X_PIN); //Get x axis position from joystick
  dataPacket.yPos = analogRead(Y_PIN); //Get y axis position from joystick

  memcpy(tx_buf, &dataPacket, sizeof(dataPacket));//Reserve memory for data to be transmitted (based on size of data)
  byte zize = sizeof(dataPacket); //Find size of data to be sent

  //Print to serial console for debugging (Uncomment to check raw values being transmitted)
  /*
  Serial.print("xPos: ");
  Serial.print(dataPacket.xPos);
  Serial.print(" yPos: ");
  Serial.print(dataPacket.yPos);
  Serial.print(" btnState: ");
  Serial.print(dataPacket.btnState);
  Serial.print(" packetsSent: ");
  Serial.println(dataPacket.packetsSent);
  */
  
  //Transmit data
  driver.send((uint8_t *)tx_buf, zize); //Send data using RF Transmitter
  driver.waitPacketSent(); //Wait for all data to be sent
  dataPacket.packetsSent++; //Increase packet count  
} //End of main control loop
