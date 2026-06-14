#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

/*
ESP8266 SMART ROOM COMMUNICATION MODULE

Communication flow:

MIT App Inventor -> WiFi -> ESP8266 -> SoftwareSerial -> STM32
STM32 -> SoftwareSerial -> ESP8266 -> WiFi -> MIT App Inventor

ESP8266 pins:

D5 = RX from STM32
D6 = TX to STM32

STM32 USART1 pins:

PA9  = STM32 TX
PA10 = STM32 RX

Connections:

ESP D6  -> STM32 PA10
ESP D5  -> STM32 PA9
ESP GND -> STM32 GND

Status format expected from STM32:

MOTION=1,TEMP=25,HUM=60,LIGHT=ON,FAN=45,ALARM=0\n
*/

// SoftwareSerial(RX, TX)
SoftwareSerial stmSerial(D5, D6);

const char* ssid = "SMART_ROOM";
const char* password = "12345678";

ESP8266WebServer server(80);

const unsigned long STM32_TIMEOUT = 2000;

// LED blink variables
unsigned long previousBlinkTime = 0;
const unsigned long blinkInterval = 500;
bool ledState = false;

// SEND COMMAND TO STM32
void sendToSTM32(String cmd)
{
  stmSerial.println(cmd);
}

// READ RESPONSE FROM STM32
String readFromSTM32()
{
  String response = "";
  unsigned long startTime = millis();

  while (millis() - startTime < STM32_TIMEOUT)
  {
    yield();

    while (stmSerial.available())
    {
      char c = stmSerial.read();

      if (c == '\n')
      {
        response.trim();
        return response;
      }

      response += c;
    }
  }

  response.trim();
  return response;
}

// HANDLE COMMAND FROM APP
void handleCommand()
{
  if (!server.hasArg("cmd"))
  {
    server.send(400, "text/plain", "ERROR:NO_COMMAND");
    return;
  }

  String cmd = server.arg("cmd");

  while (stmSerial.available())
  {
    stmSerial.read();
  }

  sendToSTM32(cmd);

  String stm32Ack = readFromSTM32();

  if (stm32Ack.length() == 0)
  {
    server.send(504, "text/plain", "ERROR:NO_ACK_FROM_STM32");
  }
  else
  {
    server.send(200, "text/plain", stm32Ack);
  }
}

// HANDLE STATUS REQUEST FROM APP
void handleStatus()
{
  while (stmSerial.available())
  {
    stmSerial.read();
  }

  sendToSTM32("STATUS?");

  String status = readFromSTM32();

  if (status.length() == 0)
  {
    server.send(504, "text/plain", "ERROR:NO_STATUS_FROM_STM32");
  }
  else
  {
    server.send(200, "text/plain", status);
  }
}

// BLINK ESP8266 INTERNAL LED
void blinkLed()
{
  unsigned long currentTime = millis();

  if (currentTime - previousBlinkTime >= blinkInterval)
  {
    previousBlinkTime = currentTime;
    ledState = !ledState;

    digitalWrite(LED_BUILTIN, ledState ? LOW : HIGH);
  }
}

//SETUP
void setup()
{
  Serial.begin(115200);      // Debug through USB only
  stmSerial.begin(9600);     // Communication with STM32

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  delay(500);

  server.on("/cmd", handleCommand);
  server.on("/status", handleStatus);

  server.begin();

  Serial.println();
  Serial.println("ESP SERVER STARTED");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

// LOOP
void loop()
{
  server.handleClient();
  blinkLed();
}