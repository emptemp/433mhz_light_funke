#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "index.h"

const char* ssid = "...";
const char* password = "...";

ESP8266WebServer server(80);

#define LED_PIN             13
#define TX_PIN              5

//=== SET HIGH/LOW PULSE LENGTH =====
#define INTER_SHORT         255
#define INTER_LONG          1300
#define INTER_START_SYNC    1250
#define INTER_STOP_SYNC     10000

#define MUMBI_1_SHORT       300
#define MUMBI_1_LONG        810
#define MUMBI_0_SHORT       300
#define MUMBI_0_LONG        830
#define MUMBI_SYNC          9500
//===================================

#define MOOD_ON             1
#define MOOD_OFF            2
#define WORK_ON             3
#define WORK_OFF            4
#define ALL_ON              5
#define ALL_OFF             6

//=== SET INTERTECHNO CODES =========
int inter_1_on[] =  { 169, 106, 170, 149, 154, 166, 105, 170 };
int inter_1_off[] = { 169, 106, 170, 149, 154, 166, 106, 170 };

int inter_2_on[] =  { 169, 106, 170, 149, 154, 166, 105, 169 };
int inter_2_off[] = { 169, 106, 170, 149, 154, 166, 106, 169 };

int inter_3_on[] =  { 169, 106, 170, 149, 154, 166, 105, 166 };
int inter_3_off[] = { 169, 106, 170, 149, 154, 166, 106, 166 };

int inter_4_on[] =  { 153, 150, 154, 106, 165, 170, 105, 170 };
int inter_4_off[] = { 153, 150, 154, 106, 165, 170, 106, 170 };

int inter_5_on[] =  { 153, 150, 154, 106, 165, 170, 105, 169 };
int inter_5_off[] = { 153, 150, 154, 106, 165, 170, 106, 169 };

int inter_6_on[] =  { 153, 150, 154, 106, 165, 170, 105, 166 };
int inter_6_off[] = { 153, 150, 154, 106, 165, 170, 106, 166 };

int inter_all1_on[] =  { 169, 106, 170, 149, 154, 166, 102, 170 };
int inter_all1_off[] = { 169, 106, 170, 149, 154, 166, 103, 170 };

int inter_all2_on[] =  { 153, 150, 154, 106, 165, 170, 102, 170 };
int inter_all2_off[] = { 153, 150, 154, 106, 165, 170, 103, 170 };

//=== SET MUMBI CODES ===============
unsigned int mumbi_1_a_on =  2472021963;
unsigned int mumbi_1_a_off = 2472021706;
unsigned int mumbi_1_b_on =  2472021449;
unsigned int mumbi_1_b_off = 2472021192;
unsigned int mumbi_1_c_on =  2472020941;
unsigned int mumbi_1_c_off = 2472020684;
unsigned int mumbi_1_d_on =  2472019907;
unsigned int mumbi_1_d_off = 2472019650;
unsigned int mumbi_1_all_on = 2472019136;
unsigned int mumbi_1_all_off = 2472020175;

unsigned int mumbi_2_a_on =  4082372503;
unsigned int mumbi_2_a_off = 4082372246;
unsigned int mumbi_2_b_on =  4082371989;
unsigned int mumbi_2_b_off = 4082371732;
unsigned int mumbi_2_c_on =  4082371475;
unsigned int mumbi_2_c_off = 4082371218;
unsigned int mumbi_2_d_on =  4082370459;
unsigned int mumbi_2_d_off = 4082370202;
unsigned int mumbi_2_all_on = 4082369688;
unsigned int mumbi_2_all_off = 4082370704;
//=====================================

void sendMumbiLogic_1() {
  digitalWrite(TX_PIN, 1);
  delayMicroseconds(MUMBI_1_LONG);
  digitalWrite(TX_PIN, 0);
  delayMicroseconds(MUMBI_1_SHORT);
}

void sendMumbiLogic_0() {
  digitalWrite(TX_PIN, 1);
  delayMicroseconds(MUMBI_0_SHORT);
  digitalWrite(TX_PIN, 0);
  delayMicroseconds(MUMBI_0_LONG);
}

void sendInterLogic_1() {
  digitalWrite(TX_PIN, 1);
  delayMicroseconds(INTER_SHORT);
  digitalWrite(TX_PIN, 0);
  delayMicroseconds(INTER_SHORT);
}

void sendInterLogic_0() {
  digitalWrite(TX_PIN, 1);
  delayMicroseconds(INTER_SHORT);
  digitalWrite(TX_PIN, 0);
  delayMicroseconds(INTER_LONG);
}

void sendMumbiSignal(unsigned int code, bool ack)
{
  noInterrupts();
  for(int repeat = 0; repeat < 4; repeat++)
  {
    Serial.print("::");
    for (int i = 31; i >= 0; i--)
    {
      Serial.print((code >> i) & 0x01);
      (code >> i) & 0x01 == 1 ? sendMumbiLogic_1() : sendMumbiLogic_0();
    }
    sendMumbiLogic_0();
    sendMumbiLogic_0();
    delayMicroseconds(MUMBI_SYNC);
    Serial.print(":\\\n");
  }
  interrupts();
  //Send OK but no data
  if(ack) {
    server.send(204);
  }
}

void sendInterSignal(int sendBytes[8], bool ack)
{
  int bufferval = 0;
  noInterrupts();
  for (int repeat = 0; repeat < 2; repeat++)
  {
    ///send start
    sendInterLogic_0();
    delayMicroseconds(INTER_START_SYNC);
    Serial.print("::");
    for (int i = 0; i < 64; i++)
    {
      bufferval = sendBytes[i / 8];
      if ((1 & (bufferval >> (7 - i % 8))) == 1) {
        Serial.print("1");
        sendInterLogic_1();
      }
      else if ((1 & (bufferval >> (7 - i % 8))) == 0) {
        Serial.print("0");
        sendInterLogic_0();
      }
    }
    Serial.print(":\\\n");
    ///send end
    sendInterLogic_0();
    delayMicroseconds(INTER_STOP_SYNC);
  }
  interrupts();
  //Send OK but no data
  if(ack) {
    server.send(204);
  }
}

void sendMood(int mood_code)
{
  switch (mood_code) {
    case MOOD_ON: 
      sendInterSignal(inter_1_on, false);
      sendInterSignal(inter_2_on, false);
      sendInterSignal(inter_3_on, false);
      sendInterSignal(inter_4_on, false);
      sendMumbiSignal(mumbi_1_a_on, false);
      break;
    case MOOD_OFF: 
      sendInterSignal(inter_1_off, false);
      sendInterSignal(inter_2_off, false);
      sendInterSignal(inter_3_off, false);
      sendInterSignal(inter_4_off, false);
      sendMumbiSignal(mumbi_1_a_off, false);
      break;
    case WORK_ON: 
      sendInterSignal(inter_5_on, false);
      sendMumbiSignal(mumbi_2_a_on, false);
      sendMumbiSignal(mumbi_2_b_on, false);
      break;
    case WORK_OFF: 
      sendInterSignal(inter_5_off, false);
      sendMumbiSignal(mumbi_2_a_off, false);
      sendMumbiSignal(mumbi_2_b_off, false);
      break;
    case ALL_ON: 
      sendInterSignal(inter_1_on, false);
      sendInterSignal(inter_2_on, false);
      sendInterSignal(inter_3_on, false);
      sendInterSignal(inter_4_on, false);
      sendInterSignal(inter_5_on, false);
      sendInterSignal(inter_6_on, false);
      sendMumbiSignal(mumbi_1_all_on, false);
      sendMumbiSignal(mumbi_2_all_on, false);
      break;
    case ALL_OFF: 
      sendInterSignal(inter_1_off, false);
      sendInterSignal(inter_2_off, false);
      sendInterSignal(inter_3_off, false);
      sendInterSignal(inter_4_off, false);
      sendInterSignal(inter_5_off, false);
      sendInterSignal(inter_6_off, false);
      sendMumbiSignal(mumbi_1_all_off, false);
      sendMumbiSignal(mumbi_2_all_off, false);
      break;
    default: break;
    }
  //Send OK but no data
  server.send(204);
}
/////////////////////////////////////////////////////////////////////

void handleRoot() {
  digitalWrite (LED_PIN, 1);
  char temp[2056];
  int sec = millis()/1000;
  int min = sec/60;
  int hr = min/60;
  String s = MAIN_page;
  server.send(200, "text/html", s);
  digitalWrite(LED_PIN, 0);
}

void handleNotFound()
{
  digitalWrite (LED_PIN, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send (404, "text/plain", message);
  digitalWrite (LED_PIN, 0 );
}

void setup(void)
{
  pinMode(LED_PIN, OUTPUT );  // set LED_PIN to OUTPUZT
  pinMode(TX_PIN, OUTPUT);    // set TX_PIN to OUTPUT
  digitalWrite(LED_PIN, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ("");
  Serial.print ("Connected to ");
  Serial.println (ssid);
  Serial.print ("IP address: ");
  Serial.println (WiFi.localIP());

  if (MDNS.begin ("esp8266" )) {
    Serial.println ( "MDNS responder started" );
  }

  server.on("/", handleRoot);
  server.on("/send_i1_on",  []() {
    sendInterSignal(inter_1_on, true);
  }   );
  server.on("/send_i1_off", []() {
    sendInterSignal(inter_1_off, true);
  }  );
  server.on("/send_i2_on",  []() {
    sendInterSignal(inter_2_on, true);
  }   );
  server.on("/send_i2_off", []() {
    sendInterSignal(inter_2_off, true);
  }  );
  server.on("/send_i3_on",  []() {
    sendInterSignal(inter_3_on, true);
  }   );
  server.on("/send_i3_off", []() {
    sendInterSignal(inter_3_off, true);
  }  );
  server.on("/send_i4_on",  []() {
    sendInterSignal(inter_4_on, true);
  }   );
  server.on("/send_i4_off", []() {
    sendInterSignal(inter_4_off, true);
  }  );
  server.on("/send_i5_on",  []() {
    sendInterSignal(inter_5_on, true);
  }   );
  server.on("/send_i5_off", []() {
    sendInterSignal(inter_5_off, true);
  }  );
  server.on("/send_i6_on",  []() {
    sendInterSignal(inter_6_on, true);
  }   );
  server.on("/send_i6_off", []() {
    sendInterSignal(inter_6_off, true);
  }  );
  server.on("/send_m1a_on",  []() {
    sendMumbiSignal(mumbi_1_a_on, true);
  }   );
  server.on("/send_m1a_off", []() {
    sendMumbiSignal(mumbi_1_a_off, true);
  }  );
  server.on("/send_m1b_on",  []() {
    sendMumbiSignal(mumbi_1_b_on, true);
  }   );
  server.on("/send_m1b_off", []() {
    sendMumbiSignal(mumbi_1_b_off, true);
  }  );
  server.on("/send_m1c_on",  []() {
    sendMumbiSignal(mumbi_1_c_on, true);
  }   );
  server.on("/send_m1c_off", []() {
    sendMumbiSignal(mumbi_1_c_off, true);
  }  );
  server.on("/send_m2a_on",  []() {
    sendMumbiSignal(mumbi_2_a_on, true);
  }   );
  server.on("/send_m2a_off", []() {
    sendMumbiSignal(mumbi_2_a_off, true);
  }  );
  server.on("/send_m2b_on",  []() {
    sendMumbiSignal(mumbi_2_b_on, true);
  }   );
  server.on("/send_m2b_off", []() {
    sendMumbiSignal(mumbi_2_b_off, true);
  }  );
  server.on("/send_m2c_on",  []() {
    sendMumbiSignal(mumbi_2_c_on, true);
  }   );
  server.on("/send_m2c_off", []() {
    sendMumbiSignal(mumbi_2_c_off, true);
  }  );
  server.on("/send_mood_on", []() {
    sendMood(MOOD_ON);
  }  );
  server.on("/send_mood_off", []() {
    sendMood(MOOD_OFF);
  }  );
  server.on("/send_work_on", []() {
    sendMood(WORK_ON);
  }  );
  server.on("/send_work_off", []() {
    sendMood(WORK_OFF);
  }  );
  server.on("/send_all_on", []() {
    sendMood(ALL_ON);
  }  );
  server.on("/send_all_off", []() {
    sendMood(ALL_OFF);
  }  );
  server.onNotFound (handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop (void) {
  server.handleClient();
}
