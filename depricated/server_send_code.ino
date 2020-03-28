#include <Arduino.h>

/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "...";
const char* password = "...";

ESP8266WebServer server ( 80 );

const int led = 13;

/////////////////////////////////////////////////////////////////////
#define transmitPin 5
#define shortPuls 255
#define longPuls 1300
#define dly 20

int send_1_on[]  =  { 169, 106, 170, 149, 154, 166, 105, 170 };
int send_1_off[]  =  { 169, 106, 170, 149, 154, 166, 106, 170 };

int send_2_on[]  =  { 169, 106, 170, 149, 154, 166, 105, 169 };
int send_2_off[]  =  { 169, 106, 170, 149, 154, 166, 106, 169 };

int send_3_on[]  =  { 169, 106, 170, 149, 154, 166, 105, 166 };
int send_3_off[]  =  { 169, 106, 170, 149, 154, 166, 106, 166 };

int send_4_on[]  =  { 153, 150, 154, 106, 165, 170, 105, 170 };
int send_4_off[]  =  { 153, 150, 154, 106, 165, 170, 106, 170 };

int send_5_on[]  =  { 153, 150, 154, 106, 165, 170, 105, 169 };
int send_5_off[]  =  { 153, 150, 154, 106, 165, 170, 106, 169 };

int send_6_on[]  =  { 153, 150, 154, 106, 165, 170, 105, 166 };
int send_6_off[]  =  { 153, 150, 154, 106, 165, 170, 106, 166 };

int send_all1_on[]  =  { 169, 106, 170, 149, 154, 166, 102, 170 };
int send_all1_off[]  =  { 169, 106, 170, 149, 154, 166, 103, 170 };

int send_all2_on[]  =  { 153, 150, 154, 106, 165, 170, 102, 170 };
int send_all2_off[]  =  { 153, 150, 154, 106, 165, 170, 103, 170 };


void sendLogic_1()
{
  digitalWrite(transmitPin, 1);
  delayMicroseconds(shortPuls);
  digitalWrite(transmitPin, 0);
  delayMicroseconds(shortPuls);
}
void sendLogic_0()
{
  digitalWrite(transmitPin, 1);
  delayMicroseconds(shortPuls);
  digitalWrite(transmitPin, 0);
  delayMicroseconds(longPuls);
}

void blinker() {
 for(int i = 1; i <= 10; i++) 
 {
   sendSignal(send_1_on);
   delay(20); 
   sendSignal(send_2_on);
   delay(20); 
   sendSignal(send_3_on);
   delay(20); 
   sendSignal(send_4_on);
   delay(20);
   sendSignal(send_5_on);
   delay(20); 
   sendSignal(send_6_on);
   delay(20); 
   sendSignal(send_1_off);
   delay(20); 
   sendSignal(send_2_off);
   delay(20); 
   sendSignal(send_3_off);
   delay(20); 
   sendSignal(send_4_off);
   delay(20); 
   sendSignal(send_5_off);
   delay(20); 
   sendSignal(send_6_off);
   delay(20); 
 }
}

void sendSignal(int sendBytes[8])
{
  int bufferval = 0;
  noInterrupts();
  for(int a = 0; a < 2; a++)
  {
    ///send start
    sendLogic_0();
    delayMicroseconds(1250);
    Serial.print("\n::");
    for (int b = 0; b < 64; b++)
    {
      bufferval = sendBytes[b/8];

       if ((1 & (bufferval >> (7 - b%8))) == 1) {
          Serial.print("1");
          sendLogic_1();
      }
      else if ((1 & (bufferval >> (7 - b%8))) == 0) {
          Serial.print("0");
          sendLogic_0();
      }
    }
     Serial.print(":\\\n");
    ///send end
    sendLogic_0();
    delayMicroseconds(10000);
  }
  interrupts();
  //Send OK but no data
  server.send(204);
}
/////////////////////////////////////////////////////////////////////

void handleRoot() {
	digitalWrite ( led, 1 );
	char temp[2056];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	snprintf ( temp, 2056,

//<meta http-equiv='refresh' content='15'/>\

"<html>\
  <head>\
    <meta http-equiv='content-type' content='text/html; charset=UTF-8'/>\
    <title>ESP8266-Funke</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1 align='center'/>433Mhz ESP8266 Funke</h1>\
    <p align='center'/>Uptime: %02d:%02d:%02d</p>\
    <p align='center'/>1::  <a href='send_1_on'/><button>ON</button></a><a href='send_1_off'/><button>OFF</button></a></p>\
    <p align='center'/>2::  <a href='send_2_on'/><button>ON</button></a><a href='send_2_off'/><button>OFF</button></a></p>\
    <p align='center'/>3::  <a href='send_3_on'/><button>ON</button></a><a href='send_3_off'/><button>OFF</button></a></p>\
    <p align='center'/>4::  <a href='send_4_on'/><button>ON</button></a><a href='send_4_off'/><button>OFF</button></a></p>\
    <p align='center'/>5::  <a href='send_5_on'/><button>ON</button></a><a href='send_5_off'/><button>OFF</button></a></p>\
    <p align='center'/>6::  <a href='send_6_on'/><button>ON</button></a><a href='send_6_off'/><button>OFF</button></a></p>\
    </body>\
</html>",

		hr, min % 60, sec % 60
	);
	server.send ( 200, "text/html", temp );
	digitalWrite ( led, 0 );
}

void handleNotFound() {
	digitalWrite ( led, 1 );
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );
	digitalWrite ( led, 0 );
}

void setup ( void ) {
	pinMode ( led, OUTPUT );
  pinMode(transmitPin, OUTPUT);   //send transmitPin to OUTPUT
	digitalWrite ( led, 0 );
	Serial.begin ( 9600 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

	if ( MDNS.begin ( "esp8266" ) ) {
		Serial.println ( "MDNS responder started" );
	}

	server.on ( "/", handleRoot );
  server.on ( "/send_1_on",  []() { sendSignal(send_1_on); }   );
  server.on ( "/send_1_off", []() { sendSignal(send_1_off); }  );
  server.on ( "/send_2_on",  []() { sendSignal(send_2_on); }   );
  server.on ( "/send_2_off", []() { sendSignal(send_2_off); }  );
  server.on ( "/send_3_on",  []() { sendSignal(send_3_on); }   );
  server.on ( "/send_3_off", []() { sendSignal(send_3_off); }  );
  server.on ( "/send_4_on",  []() { sendSignal(send_4_on); }   );
  server.on ( "/send_4_off", []() { sendSignal(send_4_off); }  );
  server.on ( "/send_5_on",  []() { sendSignal(send_5_on); }   );
  server.on ( "/send_5_off", []() { sendSignal(send_5_off); }  );
  server.on ( "/send_6_on",  []() { sendSignal(send_6_on); }   );
  server.on ( "/send_6_off", []() { sendSignal(send_6_off); }  );
  server.on ( "/random",     []() { blinker(); 
                                    server.send ( 200, "text/plain", "...happy?" );  }  );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
}

void loop ( void ) {
	server.handleClient();
}
