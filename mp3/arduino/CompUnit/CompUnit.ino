#include <SoftwareSerial.h>
#include <SPI.h>
#include "RF24.h"

#define PLAYERMODE 2    // should be defined as either 1, 2, 3, or 4 depending on which unit this is

// the Radio object
RF24 radio(7,8);

unsigned long currTime;  // holds the current time, so we can determine when we want to send radio info
unsigned long sentTime;   // holds the time we last sent a radio signal

uint8_t serialIn;   // creating character variable to hold input signals
// SERIAL INPUT VALUES>250 ARE SPECIAL TRIGGER VALUES
// 255 = end of message
// 254 = start multiplayer mode
// 253 = start controller mode

int arduinoMode;    // holds the current arduino mode we're in. 1 = MULTIPLAYER MODE, 2 = CONTROLLER MODE

uint8_t transmitBuf[18];   // holds the information we will transmit to the computer
uint8_t inputBuf[16];     // holds information we will transmit to other arduinos

int bufPointer;   // holds the location in the inputBuf we read into

uint8_t pipeNum;

// our struct to send/receieve multiplayer game data
struct multiPackage{
  uint8_t playerX;
  uint8_t playerY;
  uint8_t pitchX1;
  uint8_t pitchY1;
  uint8_t pitchX2;
  uint8_t pitchY2;
  uint8_t pitchX3;
  uint8_t pitchY3;
  uint8_t pitchX4;
  uint8_t pitchY4;
  uint8_t pitchX5;
  uint8_t pitchY5; 
  uint8_t pitchX6;
  uint8_t pitchY6; 
  uint8_t playerOrientation;
  uint8_t playerHealth;
};
struct multiPackage currPlayer, player1, player2, player3, player4;

// our struct to recieve data from the controller
struct controlPackage{
  int8_t xCoord;
  int8_t yCoord;
  int8_t buttonPressed;
};
struct controlPackage controlInfo;

// holds the pipe addresses of each of our units
byte addresses[][6] = {"1unit","2unit","3unit","4unit","5unit"};
    // controller is 1unit, player 1 is 2unit, player 2 is 3unit, player 3 is 4unit, player 4 is 5unit

void setup() 
{
  // initialize all our information to 0
  currPlayer.playerX = 0;
  currPlayer.playerY = 0;
  currPlayer.pitchX1 = 0;
  currPlayer.pitchY1 = 0;
  currPlayer.pitchX2 = 0;
  currPlayer.pitchY2 = 0;
  currPlayer.pitchX3 = 0;
  currPlayer.pitchY3 = 0;
  currPlayer.pitchX4 = 0;
  currPlayer.pitchY4 = 0;
  currPlayer.pitchX5 = 0;
  currPlayer.pitchY5 = 0;
  currPlayer.pitchX6 = 0;
  currPlayer.pitchY6 = 0;
  currPlayer.playerOrientation = 0;
  currPlayer.playerHealth = 0;

  controlInfo.xCoord = 0;
  controlInfo.yCoord = 0;
  controlInfo.buttonPressed = 0;

  // set our radio stuff
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_LOW); // we might need to do this or not

  // set up our timing stuff
  currTime = millis();
  sentTime = currTime;

  // set up our end of message signal
  transmitBuf[17] = 255;
  
  // set up our serial transmitting stuff
  Serial.begin(9600);
  /*
  Serial.println(F("Welcome to the multiTest Arduino unit"));
  Serial.print(F("We are unit"));
  Serial.println(PLAYERMODE);
*/

  arduinoMode = 0;    // initialize this
  while(arduinoMode == 0)
  {
    if(Serial.available() > 0)   // keep looping over! we're waiting for our special signal.
    {
      serialIn = Serial.read();
      if(serialIn == 254)   // this means we're going to multiplayer mode
        arduinoMode = 1;
      else if(serialIn == 253)   // this means we're going to controller mode
        arduinoMode = 2;
    }
  }

  // open up our reading pipes, but don't open up the reading pipe for our own player
    if(PLAYERMODE != 1)
      radio.openReadingPipe(1,addresses[1]);
    if(PLAYERMODE != 2)
      radio.openReadingPipe(2,addresses[2]);
    if(PLAYERMODE != 3)
      radio.openReadingPipe(3,addresses[3]);
    if(PLAYERMODE != 4)
      radio.openReadingPipe(4,addresses[4]);
  // and also we start reading from the controller
    radio.openReadingPipe(5,addresses[0]);      // THE CONTROLLER IS PIPE 5, BUT ADDRESS 0 

  // also open up our writing pipe to the address we use
  radio.openWritingPipe(addresses[PLAYERMODE]);
  
  // now we 
  radio.startListening();
}

void loop() 
{
   if(arduinoMode == 1)   // this means we're in MULTIPLAYER MODE!
   {
     currTime = millis();
     if((currTime - sentTime) > 100)   // we send a ping every 200 milliseconds
     {
        radio.stopListening(); 
        if(radio.write(&currPlayer,32))   // send our struct!
        {
  //        Serial.println("successfully sent");
        }
        else
        {
  //        Serial.println("not sent");
        }
        sentTime = currTime;
        radio.startListening();   // now we have to restart
     }
  
     if(radio.available(&pipeNum))
     {/*
      Serial.println(" ");
      Serial.print(F("Our messages are"));
      Serial.print(player1.playerX);
      Serial.print(F(" "));
      Serial.print(player2.playerX);
      Serial.print(F(" "));
      Serial.print(player3.playerX);
      Serial.print(F(" "));
      Serial.print(player4.playerX);*/
      if(pipeNum == 1)
      {
        radio.read(&player1, 16);
        // if we received a signal, package this information from the struct into a 16 byte array
        transmitBuf[0] = 1;
        transmitBuf[1] = player1.playerX;
        transmitBuf[2] = player1.playerY;
        transmitBuf[3] = player1.pitchX1;
        transmitBuf[4] = player1.pitchY1;
        transmitBuf[5] = player1.pitchX2;
        transmitBuf[6] = player1.pitchY2;
        transmitBuf[7] = player1.pitchX3;
        transmitBuf[8] = player1.pitchY3;
        transmitBuf[9] = player1.pitchX4;
        transmitBuf[10] = player1.pitchY4;
        transmitBuf[11] = player1.pitchX5;
        transmitBuf[12] = player1.pitchY5;
        transmitBuf[13] = player1.pitchX6;
        transmitBuf[14] = player1.pitchY6;
        transmitBuf[15] = player1.playerOrientation;
        transmitBuf[16] = player1.playerHealth;
      }
      else if(pipeNum == 2)
      {
        radio.read(&player2, 16);
        transmitBuf[0] = 2;
        transmitBuf[1] = player2.playerX;
        transmitBuf[2] = player2.playerY;
        transmitBuf[3] = player2.pitchX1;
        transmitBuf[4] = player2.pitchY1;
        transmitBuf[5] = player2.pitchX2;
        transmitBuf[6] = player2.pitchY2;
        transmitBuf[7] = player2.pitchX3;
        transmitBuf[8] = player2.pitchY3;
        transmitBuf[9] = player2.pitchX4;
        transmitBuf[10] = player2.pitchY4;
        transmitBuf[11] = player2.pitchX5;
        transmitBuf[12] = player2.pitchY5;
        transmitBuf[13] = player2.pitchX6;
        transmitBuf[14] = player2.pitchY6;
        transmitBuf[15] = player2.playerOrientation;
        transmitBuf[16] = player2.playerHealth;
      }
      else if(pipeNum == 3)
      {
        radio.read(&player3, 16);
        transmitBuf[0] = 3;
        transmitBuf[1] = player3.playerX;
        transmitBuf[2] = player3.playerY;
        transmitBuf[3] = player3.pitchX1;
        transmitBuf[4] = player3.pitchY1;
        transmitBuf[5] = player3.pitchX2;
        transmitBuf[6] = player3.pitchY2;
        transmitBuf[7] = player3.pitchX3;
        transmitBuf[8] = player3.pitchY3;
        transmitBuf[9] = player3.pitchX4;
        transmitBuf[10] = player3.pitchY4;
        transmitBuf[11] = player3.pitchX5;
        transmitBuf[12] = player3.pitchY5;
        transmitBuf[13] = player3.pitchX6;
        transmitBuf[14] = player3.pitchY6;
        transmitBuf[15] = player3.playerOrientation;
        transmitBuf[16] = player3.playerHealth;
      }
      else if(pipeNum == 4)
      {
        radio.read(&player4, 16);
        transmitBuf[0] = 4;
        transmitBuf[1] = player4.playerX;
        transmitBuf[2] = player4.playerY;
        transmitBuf[3] = player4.pitchX1;
        transmitBuf[4] = player4.pitchY1;
        transmitBuf[5] = player4.pitchX2;
        transmitBuf[6] = player4.pitchY2;
        transmitBuf[7] = player4.pitchX3;
        transmitBuf[8] = player4.pitchY3;
        transmitBuf[9] = player4.pitchX4;
        transmitBuf[10] = player4.pitchY4;
        transmitBuf[11] = player4.pitchX5;
        transmitBuf[12] = player4.pitchY5;
        transmitBuf[13] = player4.pitchX6;
        transmitBuf[14] = player4.pitchY6;
        transmitBuf[15] = player4.playerOrientation;
        transmitBuf[16] = player4.playerHealth;
      }

      for(int i=0; i<17; i++)
      {
        Serial.write(transmitBuf[i]);
      }
    }
  }
  else if(arduinoMode == 2)   // ELSE if we're in controller mode!
  {
    if(radio.available())
    {
      radio.read(&controlInfo, 5);    // read our stuff into the control info struct
      transmitBuf[0] = controlInfo.xCoord;
      transmitBuf[1] = controlInfo.yCoord;
      transmitBuf[2] = controlInfo.buttonPressed;
      
      for(int i=0; i<3; i++)
      {
        Serial.write(transmitBuf[i]);
      }
      Serial.write(255);
    }
    
  }

  while(Serial.available() > 0)   // loop over until no more serial input remains
  {
    serialIn = Serial.read();
    if(serialIn == 255)   // this means we've received an end of line signal
    {
      bufPointer = 0;
      inputBuf[0] = currPlayer.playerX;
      inputBuf[1] = currPlayer.playerY;
      inputBuf[2] = currPlayer.pitchX1;
      inputBuf[3] = currPlayer.pitchY1;
      inputBuf[4] = currPlayer.pitchX2;
      inputBuf[5] = currPlayer.pitchY2;
      inputBuf[6] = currPlayer.pitchX3;
      inputBuf[7] = currPlayer.pitchY3;
      inputBuf[8] = currPlayer.pitchX4;
      inputBuf[9] = currPlayer.pitchY4;
      inputBuf[10] = currPlayer.pitchX5;
      inputBuf[11] = currPlayer.pitchY5;
      inputBuf[12] = currPlayer.pitchX6;
      inputBuf[13] = currPlayer.pitchY6;
      inputBuf[14] = currPlayer.playerOrientation;
      inputBuf[15] = currPlayer.playerHealth;
    }
    else if(serialIn == 254)   // this means we're going to multiplayer mode
    {
      arduinoMode = 1;
      bufPointer = 0;
      Serial.write(PLAYERMODE);
      Serial.write(255);
      break;
    }
    else if(serialIn == 253)   // this means we're going to controller mode
    {
      arduinoMode = 2;
      bufPointer = 0;
      break;
    }
    else
    {
      transmitBuf[bufPointer] = serialIn;   // put it in our buffer
      bufPointer++;
    }
  }
}
