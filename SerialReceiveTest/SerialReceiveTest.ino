 //#include <SoftwareSerial.h>
 //SoftwareSerial mySerial(10, 11); // RX, TX

//declace a String to hold what we're inputting
String incomingString = "";

void setup() {
  //initialise Serial communication on 9600 baud
  Serial.begin(115200);
  while(!Serial);
  Serial.println("HW Ready!");

  //mySerial.begin(115200);
  //mySerial.println("SW Ready!");
}

void loop () {
  // Check if there's incoming serial data.

  if (Serial.available() > 0) {
    char ch = Serial.read();
     
    // show the byte on serial monitor
    Serial.print(ch);
  }

  /*
  if (Serial.available() > 0) {
    Serial.println("Avail");

    Serial.println( Serial.read(), DEC );
    Serial.flush();
    */
    /*
    // Read a byte from the serial buffer.
    char incomingByte = (char)Serial.read();
    incomingString += incomingByte;

    // Checks for null termination of the string.
    if (incomingByte == '\0') {
      // ...do something with String...
      Serial.println(incomingString);
      
      if(incomingString == "hello") {
        Serial.println("Hello World!"); 
      }

      incomingString = "";
    }
    */
  //}
}
