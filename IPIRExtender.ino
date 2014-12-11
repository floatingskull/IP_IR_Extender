/*
 * Connect the IR LED to pin 13 on an Arduino Yun board, pin 3 on an Uno.  This setting
 * cannot be user-defined, because the people who made this IR library are balls.  If you
 * would like status indications, connect an LED to pin 10, or whatever pin you set "led"
 * to.  Enter your Crestron controller's IP address, and the port it's been programmed to
 * listen on, where indicated.


*/

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <IRremote.h>

int serialSpd = 9600;

const int led = 10;
const int rxLen = 32;
IRsend irsend;

//The commands that the Crestron controller will send for codes 1 and 2.
#define COMMAND_1 "command1"
#define COMMAND_2 "command2"
#define COMMAND_3 "command3"


//Samsung IR Codes (On and Off)
#define CODE_LEN 67
unsigned int code1[] = { 4350, 4650, 450, 1750, 450, 1750, 450, 1800, 450, 650, 450, 650, 500, 650, 400, 700, 450, 650, 500, 1750, 400, 1800, 450, 1750, 450, 650, 500, 650, 450, 650, 500, 650, 450, 650, 450, 1750, 450, 700, 450, 650, 450, 1750, 450, 1750, 500, 650, 450, 650, 450, 1800, 450, 650, 400, 1800, 450, 1750, 450, 650, 450, 700, 450, 1750, 450, 1800, 450, 650, 450 };
unsigned int code2[] = { 4350, 4650, 450, 1800, 400, 1800, 450, 1750, 450, 650, 450, 700, 450, 650, 450, 650, 450, 700, 450, 1750, 450, 1750, 450, 1750, 500, 650, 450, 650, 450, 700, 400, 700, 450, 650, 450, 700, 400, 700, 450, 700, 450, 1750, 450, 1750, 450, 650, 450, 650, 450, 1800, 400, 1800, 450, 1750, 450, 1750, 450, 700, 450, 650, 450, 1750, 500, 1750, 450, 650, 450 };
unsigned int code3[] = { };

/*
//Vizio IR Codes (Menu and Mute)
#define CODE_LEN 70
unsigned int code1[] = { 8900, 4550, 450, 700, 400, 700, 450, 1800, 400, 700, 450, 650, 450, 650, 450, 700, 450, 650, 450, 1800, 450, 1750, 450, 700, 400, 1800, 450, 1800, 450, 1800, 400, 1800, 450, 1800, 450, 1750, 450, 1800, 450, 650, 450, 700, 450, 650, 450, 650, 450, 1800, 450, 650, 450, 700, 400, 700, 450, 1800, 400, 1800, 450, 1800, 450, 1750, 450, 700, 450, 1750, 450, 8900, 2350, 450 };
unsigned int code2[] = { 8900, 4550, 500, 650, 450, 650, 450, 1800, 400, 700, 450, 650, 450, 700, 400, 700, 450, 650, 450, 1800, 450, 1800, 400, 700, 450, 1750, 500, 1750, 450, 1800, 450, 1750, 450, 1800, 450, 1800, 450, 650, 450, 650, 450, 1800, 450, 650, 450, 650, 500, 650, 450, 650, 450, 650, 500, 1750, 450, 1800, 450, 650, 450, 1800, 400, 1800, 450, 1800, 450, 1800, 450, 8900, 2350, 450 };
*/

/*
//Vizio IR Codes (On and Off)
#define CODE_LEN 67
unsigned int code1[] = { 8800, 4550, 400, 700, 450, 650, 450, 1750, 450, 650, 450, 650, 500, 650, 450, 650, 450, 650, 400, 1800, 450, 1750, 450, 650, 450, 1800, 400, 1800, 400, 1800, 450, 1750, 450, 1750, 450, 700, 400, 700, 400, 700, 400, 1800, 450, 650, 450, 650, 450, 650, 450, 650, 500, 1750, 400, 1800, 450, 1750, 450, 650, 450, 1750, 450, 1800, 400, 1800, 450, 1750, 450 };
unsigned int code2[] = { 8800, 4550, 400, 700, 400, 700, 450, 1750, 450, 650, 450, 650, 450, 650, 450, 700, 400, 700, 400, 1800, 450, 1750, 450, 650, 450, 1750, 450, 1800, 400, 1800, 450, 1750, 450, 1750, 450, 650, 450, 1800, 400, 700, 400, 700, 450, 650, 450, 650, 450, 650, 450, 650, 450, 1800, 400, 700, 400, 1800, 450, 1750, 450, 1750, 450, 1800, 400, 1800, 450, 1750, 450 };
unsigned int code3[] = { 8800, 4550, 400, 700, 450, 650, 450, 1750, 450, 650, 450, 650, 450, 700, 400, 700, 400, 700, 400, 1800, 450, 1750, 450, 650, 450, 1800, 400, 1800, 400, 1800, 450, 1750, 450, 1750, 450, 1800, 400, 1800, 450, 650, 450, 650, 450, 650, 450, 650, 450, 700, 400, 700, 400, 700, 400, 700, 400, 1800, 450, 1750, 450, 1800, 400, 1800, 400, 1800, 450, 1750, 450 };
*/

//Store the IP address and port number for communicating to the Crestron processor here.
const byte serverIP[4] = { 0, 0, 0, 0 };
const int serverPort = ####


YunClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(led, OUTPUT);
  
  blinkLED(5);

  Bridge.begin();
  client.setTimeout(250);
  
  blinkLED(6);

  Serial.println("Startup Complete");
}



void blinkLED(int num) {
  for(int i = 0; i < num; i++) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }
}




void ParseCommand(String com) {
  blinkLED(1);
  
  // RAW CODES
  
  if(com == COMMAND_1) { 
    irsend.sendRaw(code1, CODE_LEN, 38);
  }
  else if(com == COMMAND_2) {
    irsend.sendRaw(code2, CODE_LEN, 38);
  }
  else if(com == COMMAND_3) {
    irsend.sendRaw(code3, CODE_LEN, 38);
  }
  
  
}




void loop() {
  
  blinkLED(3);
  
  Serial.println("Connecting...");
    
  if(client.connect(serverIP, serverPort)) {
    Serial.println("Connected");
  }
  else { Serial.println("Connection Failed"); }
  
  while(client.connected())
  {
    
    if(client.available() > 0) 
    {
      String iprx = "";
      char iprxChar[rxLen];
      int len = client.available();
    
      for(int i = 0; i < len; i++) 
      {
        iprxChar[i] = char(client.read());
      }
      for(int i = 0; i < len; i++) { iprx += iprxChar[i]; }
    
      Serial.print("Received: "); 
      Serial.println(iprx);
      
      
      ParseCommand(iprx);
      
    }
    
  }
  
  delay(5000);
  Serial.println("Waiting..."); 
}