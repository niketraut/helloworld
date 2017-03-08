//#include <Adafruit_Sensor.h>
//#include <DHT.h>
//#include <DHT_U.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include <EEPROM.h>



//#define DHT11PIN 2
//#define DHTTYPE  DHT11
//DHT_Unified dht(DHT11PIN, DHTTYPE);

#define IN1  14  //D5 - GPIO14 ----Final      (IN1 14 = inPina 5) //IN1 = Relay pin/Button, 14= Node MCU pin/LED 
#define IN2  12  //D6 - GPIO12 ----Final      (IN2 12 = inPinb 4)
#define IN3  13  //D7 - GPIO13 ----Final      (IN3 13 = inPinc 15)
#define IN4  2  //D4 - GPIO2 ----Final        (IN4 2 = inPind 0)

int inPina = 5;  //D1 - GPIO5 ----Final      // the number of the input pin of Node MCU
int inPinb = 4;  //D2 - GPIO4 ----Final
int inPinc = 15; //D8 - GPIO15 ----Final
int inPind = 0;  //D3 - GPIO0 ----Final




const char* ssid = "careerraiser";
const char* password = "nike1980";
//String tempdata;
//String humiddata;
ESP8266WebServer server(80);

const int led = 0;  //D3 - GPI0

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void getSettings() {
  String pin = readeeprom(0);
  String ssid = readeeprom(32);
  String pass = readeeprom(64);
  Serial.print("newmethod read pin status : __" + pin + "__");
  Serial.print("newmethod read userid : __" + ssid + "__");
  Serial.print("newmethod read pass : __" + pass + "__");
  server.send(200, "text/plain", pin + "::" + ssid + "::" + pass);
}

String readeeprom(int start) {
  String output = "";
  int endpoint = start + 32;
  for (int i = start; i < endpoint; ++i)
  {
    if (EEPROM.read(i) == 0) {
      break;
    } else {
      output = output + char(EEPROM.read(i));
      // Serial.print("read: "+output);
    }
  }
  Serial.print("read: __" + output + "__");
  return output;
}

void writeeeprom(int startpoint, String data) {
  int endpoint = startpoint + 32;
  Serial.println("clearing eeprom");
  for (int i = startpoint; i < endpoint; ++i) {
    EEPROM.write(i, 0);
  }
  Serial.println("writing to eeprom -- " + data);
  for (int i = startpoint, j = 0; j < data.length(); ++i, ++j)
  {
    EEPROM.write(i, data[j]);
  }

}

void writeeepromchar(int startpoint, char data)
{
   Serial.println("clearing eeprom");
   EEPROM.write(startpoint, 0);
   Serial.println("writing to eeprom -- " + data);
   EEPROM.write(startpoint, data);
}

void setSettings() {
  String content = "";
  int statusCode = 200;
  String qpin = server.arg("pin");
  String qsid = server.arg("ssid");
  String qpass = server.arg("password");
  if (qpin.length() > 0 && qsid.length() > 0 && qpass.length() > 0) {
    Serial.println("writing data   " + qpin);
    writeeeprom(0, qpin);
    Serial.println("writing data   " + qsid);
    writeeeprom(32, qsid);
    Serial.println("writing data   " + qpass);
    writeeeprom(64, qpass);
    EEPROM.commit();
    server.send(200, "text/plain", "Success");
  } else {
    server.send(200, "application/json", "Failure. missing mandatory parameters");
  }

}



void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void operateSwitch() 
{
  String switchnumber = server.arg("swt");
  String action = server.arg("act");
  if(switchnumber=="1"){
    if(action=="on"){
      digitalWrite(IN1, HIGH);
      writeeepromchar(0, 49);
      server.send(200, "text/plain", "1");
    }else{
      digitalWrite(IN1, LOW);
      writeeepromchar(0, 48);
      server.send(200, "text/plain", "0");
    }    
  }else if (switchnumber=="2"){
    if(action=="on"){
      digitalWrite(IN2, HIGH);
      writeeepromchar(1, 49);
      server.send(200, "text/plain", "1");
    }else{
      digitalWrite(IN2, LOW);
      writeeepromchar(1, 48);
      server.send(200, "text/plain", "0");
    }    
  }else if (switchnumber=="3"){
    if(action=="on"){
      digitalWrite(IN3, HIGH);
      writeeepromchar(2, 49);
      server.send(200, "text/plain", "1");
    }else{
      digitalWrite(IN3, LOW);
      writeeepromchar(2, 48);
      server.send(200, "text/plain", "0");
    }    
  }else if (switchnumber=="4"){
    if(action=="on"){
      digitalWrite(IN4, HIGH);
      writeeepromchar(3, 49);
      server.send(200, "text/plain", "1");
    }else{
      digitalWrite(IN4, LOW);
      writeeepromchar(3, 48);
      server.send(200, "text/plain", "0");
    }
      
  }else{
      server.send(200, "text/plain", "1");
    }
}
void setup(void) {

  //Serial.begin(9600);
  pinMode(inPina, INPUT);
  attachInterrupt(inPina, switchIN1, RISING); //syntax : attachInterrupt (digitalPinToInterrupt (BUTTON), switchPressed, CHANGE);

  pinMode(inPinb, INPUT);
  attachInterrupt(inPinb, switchIN2, RISING); //syntax(external interrupted pin, which method you want to call, change value);

  pinMode(inPinc, INPUT);
  attachInterrupt(inPinc, switchIN3, RISING);

  pinMode(inPind, INPUT);
  attachInterrupt(inPind, switchIN4, RISING);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);
  EEPROM.begin(512);
  
  
  connectwifi();
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //  if (MDNS.begin("esp8266")) {
  //    Serial.println("MDNS responder started");
  //  }

  server.on("/", handleRoot);  
  server.on("/setsettings", setSettings);
  server.on("/getsettings", getSettings);
  server.on("/powerbackup", powerbackup);


  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");  
  powerbackup();
  
}

void loop(void)
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    setup();
  }
  server.handleClient();
}


void connectwifi()
{
  // const char* ssid=readeeprom(0).c_str();
  // const char* password=readeeprom(32).c_str();
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    operateSwitch();
    Serial.print(".");
  }
}

void powerbackup()  // Restore previous pin status from EPROM during power on/off.
{

  if (EEPROM.read(0) == 49)
  {
    Serial.println("Pin1 is On");
    digitalWrite(IN1, HIGH);
  }
  if (EEPROM.read(1) == 49)
  {
    Serial.println("Pin2 is On");
    digitalWrite(IN2, HIGH);
  }
  if (EEPROM.read(2) == 49)
  {
    Serial.println("Pin3 is On");
    digitalWrite(IN3, HIGH);
  }

  if (EEPROM.read(3) == 49)
  {
    Serial.println("Pin4 is On");
    digitalWrite(IN4, HIGH);
  }
   server.send(200, "text/plain", "done restore from memory");
}

void switchIN1 () /// Relay switch : 1
{
  
  if (EEPROM.read(0) == 49) 
  {
    Serial.println("Pin1 is On");
    digitalWrite(IN1, LOW);
    EEPROM.write(0, 48);
  }
  else {
    Serial.println("Pin1 is Off");
    digitalWrite(IN1, HIGH);
    EEPROM.write(0, 49);
  }
}


void switchIN2 () /// Relay switch : 2
{
  
  if (EEPROM.read(1) == 49)
  {
    Serial.println("Pin2 is On");
    digitalWrite(IN2, LOW);
    EEPROM.write(1, 48);
  }
  else {
    Serial.println("Pin2 is Off");
    digitalWrite(IN2, HIGH);
    EEPROM.write(1, 49);
  }
}

void switchIN3 () /// Relay switch : 3
{
  
  if (EEPROM.read(2) == 49)
  {
    Serial.println("Pin3 is On");
    digitalWrite(IN3, LOW);
    EEPROM.write(2, 48);
  }
  else {
    Serial.println("Pin3 is Off");
    digitalWrite(IN3, HIGH);
    EEPROM.write(2, 49);
  }
}

void switchIN4 () /// Relay switch : 4
{

  if (EEPROM.read(3) == 49)
  {
    Serial.println("Pin4 is On");
    digitalWrite(IN4, LOW);
    EEPROM.write(3, 48);
  }
  else {
    Serial.println("Pin4 is Off");
    digitalWrite(IN4, HIGH);
    EEPROM.write(3, 49);
  }
}



