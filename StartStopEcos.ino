/*
Start and stop of ECoS central station.
Copyright (C) <2020>  <Louis Grüninger>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <SPI.h>
#include <Ethernet.h>

//Define pin which command each function
#define START_PIN 2
#define STOP_PIN 3
#define DISCONNECT_PIN 13

//Addresses of the Arduino
byte mac[] = {0x00, 0x00, 0x00, 0x12, 0x34, 0x56};
byte ip[] = {192, 168, 1, 223};
byte gateway[] = {192, 168, 1, 1};
byte subnet[] = {255, 255, 255, 0};

//Address and port on the centrale
byte ipCentrale[] = {192, 168, 1, 210};
uint16_t portCentrale = 15471;

/*****************DO NOT MODIFY AFTER THIS LINE*****************/

//Data to send for each function
char goBuffer[] = "set(1,go)";
char stopBuffer[] = "set(1,stop)";

//Buffer for received data
char receiveBuffer[20];

uint8_t oldStartPin, oldStopPin;

//Define the EthernetClient
EthernetClient centrale;

void setup() {
  //Start Ethernet 
  Ethernet.begin(mac, ip, gateway, subnet);

  //Init inputs pins
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(STOP_PIN, INPUT_PULLUP);
  //Init outputs pins
  pinMode(DISCONNECT_PIN, OUTPUT);

  //Toggle disconnect output to 0
  digitalWrite(DISCONNECT_PIN, 0);

  //Connecting to the centrale
  delay(1000);
  while(!centrale.connected())
  {
    centrale.connect(ipCentrale, portCentrale);
    delay(100); 
  }
}

void loop() {  
  //Check if the central is connected
  if(centrale.connected())
  {
    //Start centrale
    if (digitalRead(START_PIN) == 0 && oldStartPin == 1)
    {
      centrale.println(goBuffer);
    }
    //Stop centrale
    if (digitalRead(STOP_PIN) == 0 && oldStopPin == 1)
    {
      centrale.println(stopBuffer);
    } 
  }
  else
  {
    //If client not connect, stop client and set disconnect pin at 1
    centrale.stop();
    digitalWrite(DISCONNECT_PIN,1);
    while(1);
  }

  //Keep pin state
  oldStartPin = digitalRead(START_PIN);
  oldStopPin = digitalRead(STOP_PIN);
}
