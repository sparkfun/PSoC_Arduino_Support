#include "Arduino.h"
const int ledPin = 13; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  delay(5000);
  Serial.println("Hello, world");
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // see if there's incoming serial data:
  if (Serial.available() > 0) 
  {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'H') {
      digitalWrite(ledPin, HIGH);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'L') {
      digitalWrite(ledPin, LOW);
    }
  }
}

#if 0
#include <Servo.h>

Servo servoMotor0;
/*
Servo servoMotor1;
Servo servoMotor2;
Servo servoMotor3;
Servo servoMotor4;
Servo servoMotor5;
Servo servoMotor6;
Servo servoMotor7;
Servo servoMotor8;
Servo servoMotor9;
Servo servoMotor10;
Servo servoMotor11;*/

void setup()
{
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  digitalWrite(D5, LOW);
  digitalWrite(D5, HIGH);
  digitalWrite(D5, LOW);
  Serial1.begin(57600);
  servoMotor0.attach(P3_D0, 700, 2200);
  /*
  servoMotor1.attach(P0_D1, 750, 2300);
  servoMotor2.attach(P0_D2, 750, 2300);
  servoMotor3.attach(P0_D3, 750, 2300);
  servoMotor4.attach(P0_D4, 750, 2300);
  servoMotor5.attach(P0_D5, 750, 2300);
  servoMotor6.attach(P0_D6, 750, 2300);
  servoMotor7.attach(P0_D7, 750, 2300);
  servoMotor8.attach(P3_D0, 750, 2300);
  servoMotor9.attach(P3_D1, 750, 2300);
  servoMotor10.attach(P3_D2, 750, 2300);
  servoMotor11.attach(P3_D3, 750, 2300);
  */
}
const int tickDelay = 500;
const int stepSize = 5;
void loop()
{
  //servoMotor0.write(15);
  //delay(2000);
  servoMotor0.write(95);
  //delay(2000);
  //servoMotor0.write(175);
  //delay(2000);
  /*static int angle = 0;
  static int lastTick = millis();
  int thisTick = millis();
  if (thisTick - tickDelay >= lastTick)
  {
    lastTick = thisTick;
    servoMotor0.write(angle);
    angle += stepSize;
  }
  if (Serial1.available())
  {
    Serial1.read();
    Serial1.println(angle);
  }
  if (angle >= 180)
  {
    angle = 0;
  }*/
  
}
#endif

#if 0
#include <SPI.h>
#include <WiFi.h>

void printMacAddress();
void listNetworks();
void printEncryptionType(int thisType);

void setup() {
  //Initialize serial and wait for port to open:
  Serial1.begin(9600);
  SPI.begin();
  while (!Serial1) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  delay(1200);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial1.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial1.println("Please upgrade the firmware");

  // Print WiFi MAC address:
  printMacAddress();

  // scan for existing networks:
  Serial1.println("Scanning available networks...");
  listNetworks();
}

void loop() {
  delay(10000);
  // scan for existing networks:
  Serial1.println("Scanning available networks...");
  listNetworks();
}

void printMacAddress() {
  // the MAC address of your Wifi shield
  byte mac[6];

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial1.print("MAC: ");
  Serial1.print(mac[5], HEX);
  Serial1.print(":");
  Serial1.print(mac[4], HEX);
  Serial1.print(":");
  Serial1.print(mac[3], HEX);
  Serial1.print(":");
  Serial1.print(mac[2], HEX);
  Serial1.print(":");
  Serial1.print(mac[1], HEX);
  Serial1.print(":");
  Serial1.println(mac[0], HEX);
}

void listNetworks() {
  // scan for nearby networks:
  Serial1.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    Serial1.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial1.print("number of available networks:");
  Serial1.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial1.print(thisNet);
    Serial1.print(") ");
    Serial1.print(WiFi.SSID(thisNet));
    Serial1.print("\tSignal: ");
    Serial1.print(WiFi.RSSI(thisNet));
    Serial1.print(" dBm");
    Serial1.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial1.println("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial1.println("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial1.println("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial1.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial1.println("Auto");
      break;
  }
}
#endif

#if 0
#include <SPI.h>
#include <SFE_MicroOLED.h>
#include <math.h>
//////////////////////////
// MicroOLED Definition //
//////////////////////////
#define PIN_RESET 9  // Connect RST to pin 9
#define PIN_DC    8  // Connect DC to pin 8
#define PIN_CS    10 // Connect CS to pin 10
#define DC_JUMPER 0

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // SPI 

int SCREEN_WIDTH = oled.getLCDWidth();
int SCREEN_HEIGHT = oled.getLCDHeight();

float d = 3;
float px[] = { 
  -d,  d,  d, -d, -d,  d,  d, -d };
float py[] = { 
  -d, -d,  d,  d, -d, -d,  d,  d };
float pz[] = { 
  -d, -d, -d, -d,  d,  d,  d,  d };

float p2x[] = {
  0,0,0,0,0,0,0,0};
float p2y[] = {
  0,0,0,0,0,0,0,0};

float r[] = {
  0,0,0};

#define SHAPE_SIZE 600
// Define how fast the cube rotates. Smaller numbers are faster.
// This is the number of ms between draws.
#define ROTATION_SPEED 0
void drawCube();
void setup()
{
  oled.begin();
  oled.clear(ALL);
  oled.display();  
}

void loop()
{
  drawCube();
  delay(ROTATION_SPEED);
}

void drawCube()
{
  r[0]=r[0]+PI/180.0; // Add a degree
  r[1]=r[1]+PI/180.0; // Add a degree
  r[2]=r[2]+PI/180.0; // Add a degree
  if (r[0] >= 360.0*PI/180.0) r[0] = 0;
  if (r[1] >= 360.0*PI/180.0) r[1] = 0;
  if (r[2] >= 360.0*PI/180.0) r[2] = 0;

  for (int i=0;i<8;i++)
  {
    float px2 = px[i];
    float py2 = cos(r[0])*py[i] - sin(r[0])*pz[i];
    float pz2 = sin(r[0])*py[i] + cos(r[0])*pz[i];

    float px3 = cos(r[1])*px2 + sin(r[1])*pz2;
    float py3 = py2;
    float pz3 = -sin(r[1])*px2 + cos(r[1])*pz2;

    float ax = cos(r[2])*px3 - sin(r[2])*py3;
    float ay = sin(r[2])*px3 + cos(r[2])*py3;
    float az = pz3-150;

    p2x[i] = SCREEN_WIDTH/2+ax*SHAPE_SIZE/az;
    p2y[i] = SCREEN_HEIGHT/2+ay*SHAPE_SIZE/az;
  }
  oled.clear(PAGE);
  for (int i=0;i<3;i++) 
  {
    oled.line(p2x[i],p2y[i],p2x[i+1],p2y[i+1]);
    oled.line(p2x[i+4],p2y[i+4],p2x[i+5],p2y[i+5]);
    oled.line(p2x[i],p2y[i],p2x[i+4],p2y[i+4]);
  }    
  oled.line(p2x[3],p2y[3],p2x[0],p2y[0]);
  oled.line(p2x[7],p2y[7],p2x[4],p2y[4]);
  oled.line(p2x[3],p2y[3],p2x[7],p2y[7]);
  oled.display();
}
#endif

#if 0
#include <Wire.h>
#include "APDS9960.h"

// Pins
#define APDS9960_INT    2 // Needs to be an interrupt pin

// Constants

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void interruptRoutine();
void handleGesture();

void setup() {

  // Set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);

  // Initialize Serial1 port
  Serial1.begin(57600);
  Serial1.println();
  Serial1.println(F("--------------------------------"));
  Serial1.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial1.println(F("--------------------------------"));
  
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial1.println(F("APDS-9960 initialization complete"));
  } else {
    Serial1.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial1.println(F("Gesture sensor is now running"));
  } else {
    Serial1.println(F("Something went wrong during gesture sensor init!"));
  }
}

void loop() {
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial1.println("UP");
        break;
      case DIR_DOWN:
        Serial1.println("DOWN");
        break;
      case DIR_LEFT:
        Serial1.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial1.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial1.println("NEAR");
        break;
      case DIR_FAR:
        Serial1.println("FAR");
        break;
      default:
        Serial1.println("NONE");
    }
  }
}

#endif
