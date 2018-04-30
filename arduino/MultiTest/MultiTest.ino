#include <SoftwareSerial.h>
#include <SPI.h>
#include "RF24.h"

#define PLAYERMODE 2    // should be defined as either 1, 2, 3, or 4 depending on which unit this is

// the Radio object
RF24 radio(7,8);

unsigned long currTime;  // holds the current time, so we can determine when we want to send radio info
unsigned long sentTime;   // holds the time we last sent a radio signal

char serialIn;   // creating character variable to hold input signals

uint8_t transmitBuf[48];   // holds the information we will transmit to the computer
// it will look like [PLAYER2INFO-PLAYER3INFO-PLAYER4INFO] we don't give current player info

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
    // gun is 1unit, player 1 is 2unit, player 2 is 3unit, player 3 is 4unit, player 4 is 5unit

void setup() 
{
  // initialize all our information to -1 to show that it doesn't exist yet
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
  currPlayer.playerHealth = -1;

  // set our radio stuff
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_LOW); // we might need to do this or not

  controlInfo.xCoord = 0;
  controlInfo.yCoord = 0;
  controlInfo.buttonPressed = 0;

  // set up our timing stuff
  currTime = millis();
  sentTime = currTime;
  
  // set up our serial transmitting stuff
  Serial.begin(9600);
  Serial.println(F("Welcome to the multiTest Arduino unit"));
  Serial.print(F("We are unit"));
  Serial.println(PLAYERMODE);

  // open up our reading pipes, but don't open up the reading pipe for our own player
  if(PLAYERMODE != 1)
  {
    radio.openReadingPipe(1,addresses[1]);
  }
  if(PLAYERMODE != 2)
  {
    radio.openReadingPipe(2,addresses[2]);
  }
  if(PLAYERMODE != 3)
  {
    radio.openReadingPipe(3,addresses[3]);
  } 
  if(PLAYERMODE != 4)
  {
    radio.openReadingPipe(4,addresses[4]);
  }
  // and also we start reading from the gun
  radio.openReadingPipe(5,addresses[0]);      // THE GUN IS PIPE 5, BUT ADDRESS 0 

  // also open up our writing pipe to the address we use
  radio.openWritingPipe(addresses[PLAYERMODE]);
  
  // now we 
  radio.startListening();
}

void loop() 
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
   {
    Serial.println(" ");
    Serial.print(F("Our messages are"));
/*    Serial.print(player1.playerX);
    Serial.print(F(" "));
    Serial.print(player2.playerX);
    Serial.print(F(" "));
    Serial.print(player3.playerX);
    Serial.print(F(" "));
    Serial.print(player4.playerX);
*/    if(pipeNum == 1)
    {
      radio.read(&player1, 16);
//      Serial.println(player1.playerX);
    }
    else if(pipeNum == 2)
    {
      radio.read(&player2, 16);
//      Serial.println(player2.playerX);
    }
    else if(pipeNum == 3)
    {
      radio.read(&player3, 16);
//      Serial.println(player3.playerX);
    }
    else if(pipeNum == 4)
    {
      radio.read(&player4, 16);
//      Serial.println(player4.playerX);
    }
    else if(pipeNum == 5)
    {
      radio.read(&controlInfo, 3);    // TO DO THIS STUFF
      Serial.print(controlInfo.xCoord);
      Serial.print(" ");
      Serial.print(controlInfo.yCoord);
      Serial.print(" ");
      Serial.println(controlInfo.buttonPressed);
    }
   }

  while(Serial.available() > 0)   // loop over until no more serial input remains
  {
    serialIn = Serial.read();
    currPlayer.playerX = serialIn;
  }
}
