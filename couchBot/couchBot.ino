#include <Servo.h>
#include <EEPROM.h> // for saving values
#include "couchBotDefs.h"

const uint8_t  LEFT_SERVO_PIN  = 5;
const uint8_t  RIGHT_SERVO_PIN = 6;
const uint8_t  RC_CH1          = A0;
const uint8_t  RC_CH2          = A1;
const uint8_t  RC_CH3          = A2;
const uint8_t  RC_CH8          = A3; // the knob, to be used for either turn specific sensitivity or absolute maximum lockout
const uint16_t CH1_MAX         = 1924; // right stick right/left TURN
const uint16_t CH1_MIN         = 963;
const uint16_t CH2_MAX         = 1989; // right stick up/down MAIN THROTTLE
const uint16_t CH2_MIN         = 984;
const uint16_t CH3_MAX         = 1800; // left stick up/down LIMITTER
const uint16_t CH3_MIN         = 1100;
const uint16_t CH8_MIN         = 989; // secondary limiter
const uint16_t CH8_MAX         = 1984;
const uint16_t veloDeadZone    = 6;
const uint16_t diffDeadZone    = 6;
const uint8_t  reverseRight    = 1;
const uint8_t  reverseLeft     = 0;


//working variables
uint16_t CH1_last_value = 0;
uint16_t CH2_last_value = 0;
uint16_t CH3_last_value = 0;
uint16_t CH8_last_value = 0;
uint32_t timeout = 100;
uint32_t lastTime = 0;
int16_t velocity = 0;
int16_t difference = 0;
uint16_t maxDifference = 20;
int16_t maxSpeed = 0;
int16_t leftMotorSpeed = 0;
int16_t rightMotorSpeed = 0;
uint8_t maxPWM = 180;
uint8_t minPWM = 0;
uint8_t maxLeftPWM = 180;
uint8_t minLeftPWM = 0;
uint8_t maxRightPWM = 180;
uint8_t minRightPWM = 0;
boolean deadZoneEnable = true;
boolean outputEnable = false;
boolean serialDebugPrintEnable = false;
boolean newCommand = false;
boolean elevatedPermissions = false;
String commandStr = "";


Servo leftMotor;
Servo rightMotor;


void setup() {
  Serial.begin(115200);
  leftMotor.attach(LEFT_SERVO_PIN); leftMotor.write(90);
  rightMotor.attach(RIGHT_SERVO_PIN); rightMotor.write(90);
  pinMode(RC_CH1, INPUT);
  pinMode(RC_CH2, INPUT);
  pinMode(RC_CH3, INPUT);
  pinMode(RC_CH8, INPUT);
  if (reverseLeft) {
    maxLeftPWM = 0;
    minLeftPWM = 180;
  }
  if (reverseRight) {
    maxRightPWM = 0;
    minRightPWM = 180;
  }
}


void loop() {
  if (newCommand) {
    handleCommand(); // do this first, because if anything changed we want to implement it immediately.
  }
  int32_t ch1Average = 0;
  int32_t ch2Average = 0;
  int32_t ch3Average = 0;
  int32_t ch8Average = 0;
  for (uint8_t i = 0; i < 3; i++) {
    getReceiver();
    ch1Average += CH1_last_value;
    ch2Average += CH2_last_value;
    ch3Average += CH3_last_value;
    ch8Average += CH8_last_value;
  }
  CH1_last_value = ch1Average / 3.0;
  CH2_last_value = ch2Average / 3.0;
  CH3_last_value = ch3Average / 3.0;
  CH8_last_value = ch8Average / 3.0;
  scaleNumbers();
  controlMotor();
  if (serialDebugPrintEnable) {
    serialDebugPrint();
  }
}


void getReceiver() {
  CH1_last_value = 0;
  CH2_last_value = 0;
  CH3_last_value = 0;
  CH8_last_value = 0;

  lastTime = millis();
  while (CH1_last_value == 0 && millis() - lastTime < timeout) {
    CH1_last_value = pulseIn(RC_CH1, HIGH, 20000);
  }
  lastTime = millis();
  while (CH2_last_value == 0 && millis() - lastTime < timeout) {
    CH2_last_value = pulseIn(RC_CH2, HIGH, 20000);
  }
  lastTime = millis();
  while (CH3_last_value == 0 && millis() - lastTime < timeout) {
    CH3_last_value = pulseIn(RC_CH3, HIGH, 20000);
  }
  lastTime = millis();
  while (CH8_last_value == 0 && millis() - lastTime < timeout) {
    CH8_last_value = pulseIn(RC_CH8, HIGH, 20000);
  }
}


void scaleNumbers() {


// set current speed value
  if (CH2_last_value > CH2_MAX) velocity = CH2_MAX;
  else if (CH2_last_value < CH2_MIN) velocity = CH2_MIN;
  else velocity = CH2_last_value;
  velocity = map(velocity, CH2_MIN, CH2_MAX, -99, 99);

// set maximum speed value
  if (CH3_last_value > CH3_MAX) maxSpeed = CH3_MAX;
  else if (CH3_last_value < CH3_MIN) maxSpeed = CH3_MIN;
  else maxSpeed = CH3_last_value;
  maxSpeed = map(maxSpeed, CH3_MIN, CH3_MAX, 1, 99);

// set current turn value
  if (CH1_last_value > CH1_MAX) difference = CH1_MAX;
  else if (CH1_last_value < CH1_MIN) difference = CH1_MIN;
  else difference = CH1_last_value;
  difference = map(difference, CH1_MIN, CH1_MAX, (-1*maxDifference), maxDifference);

  // set everything to 0 if transmitter is off
  if (CH1_last_value == 0) difference = 0;  //CH1 returns 0 if radio is off
  if (CH2_last_value == 0) velocity = 0;    //CH2 returns 0 if radio is off
  if (CH3_last_value == 0) velocity = 0;    //CH3 returns 0 if radio is off
  if (CH8_last_value == 0) velocity = 0;    //CH8 returns 0 if radio is off

}


void controlMotor() {
  if (deadZoneEnable) {
    if (abs(velocity) <= veloDeadZone) velocity = 0;
    if (abs(difference) <= diffDeadZone) difference = 0;
  }
  leftMotorSpeed = constrain((maxSpeed/99.0)*(velocity + difference),-99,99);
  rightMotorSpeed = constrain((maxSpeed/99.0)*(velocity - difference),-99,99);

  leftMotor.write(map(leftMotorSpeed, -99, 99, minLeftPWM, maxLeftPWM));
  rightMotor.write(map(rightMotorSpeed, -99, 99, minRightPWM, maxRightPWM));
}

void handleCommand() {
    if (commandStr.length() < 1) {
        invalidCommand();
    } else if (commandStr.equalsIgnoreCase("enable")) {
        outputEnable = true;
    } else if (commandStr.equalsIgnoreCase("disable")) {
        outputEnable = false;
    } else if (commandStr.equalsIgnoreCase("help")) {
        printHelp();
    } else if (commandStr.equalsIgnoreCase("lpv")) {
        printParameters();
    } else if (commandStr.equalsIgnoreCase("dbg")) {
        serialDebugPrintEnable = true;
    } else if (commandStr.equalsIgnoreCase("lcp") && elevatedPermissions) {
        listParameters();
    } else if (commandStr.substring(0,2).equalsIgnoreCase("cp") && elevatedPermissions) {
        changeParameters();
    } else {

    }
}

void invalidCommand() {
    Serial.println("Invalid Command!");
}

void serialDebugPrint() {
  Serial.print("CH1: "); Serial.print(CH1_last_value);
  Serial.print("  CH2: "); Serial.print(CH2_last_value);
  Serial.print("  CH3: "); Serial.print(CH3_last_value);
  Serial.print("  CH8: "); Serial.print(CH8_last_value);
  Serial.print("  Velocity: "); Serial.print(velocity);
  Serial.print("  Diff: "); Serial.print(difference);
  Serial.print("  Max Speed: "); Serial.print(maxSpeed);
  Serial.print("  L: "); Serial.print(leftMotorSpeed);
  Serial.print("  R: "); Serial.println(rightMotorSpeed);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    commandStr += inChar;
    if (inChar == '\n') {
      newCommand = true;
    }
  }
}

void printHelp() {
    Serial.println("******Caddy Couch Control V0.2******");
    Serial.println("Available Commands:");
    Serial.println("help - print this message");
    Serial.println("lpv - list all parameter values");
    Serial.println("dbg - print debug values (q to quit)");
    Serial.println("enable - enable motor output");
    Serial.println("disable - disable motor output");
    if (elevatedPermissions) {
        Serial.println("You have elevated permissions! You have extra access:");
        Serial.println("lcp - list changeable parameters");
        Serial.println("cp <parameter> <value> - change a parameter value. Motors must be disabled");
        Serial.println("save - save parameters to eeprom. Will overwrite any values stored there, and cannot be undone");
        Serial.println("reload - reload parameters from eeprom");
    }
}

void printParameters() {
    Serial.println("***Current Parameters***");
    Serial.println("---Motor Settings---");
    Serial.print("outputEnable");Serial.println(outputEnable);
    Serial.print("reverseRight");Serial.println(reverseRight);
    Serial.print("reverseLeft");Serial.println(reverseLeft);
    Serial.println("---Control Parameters---");
    Serial.print("maxDifference");Serial.println(maxDifference);
    Serial.print("maxPWM");Serial.println(maxPWM);
    Serial.print("minPWM");Serial.println(minPWM);
    Serial.print("veloDeadZone");Serial.println(veloDeadZone);
    Serial.print("diffDeadZone");Serial.println(diffDeadZone);
    Serial.print("deadZoneEnable");Serial.println(deadZoneEnable);
    Serial.print("ch1max");Serial.println(CH1_MAX);
    Serial.print("ch1min");Serial.println(CH1_MIN);
    Serial.print("ch2max");Serial.println(CH2_MAX);
    Serial.print("ch2min");Serial.println(CH2_MIN);
    Serial.print("ch3max");Serial.println(CH3_MAX);
    Serial.print("ch3min");Serial.println(CH3_MIN);
    Serial.print("ch8max");Serial.println(CH8_MAX);
    Serial.print("ch8min");Serial.println(CH8_MIN);
}

void listParameters(void) {
    Serial.println("---Motor Settings---");
    Serial.println("outputEnable");
    Serial.println("reverseRight");
    Serial.println("reverseLeft");
    Serial.println("---Control Loop Parameters---");
    Serial.println("maxDifference");
    Serial.println("maxPWM");
    Serial.println("minPWM");
    Serial.println("veloDeadZone");
    Serial.println("diffDeadZone");
    Serial.println("deadZoneEnable");
    Serial.println("---Controller Parameters---");
    Serial.println("ch1max");
    Serial.println("ch1min");
    Serial.println("ch2max");
    Serial.println("ch2min");
    Serial.println("ch3max");
    Serial.println("ch3min");
    Serial.println("ch8max");
    Serial.println("ch8min");
}

void changeParameters() {
    // first check the function for form
    String paramVal = commandStr.substring(4).toLowerCase();
    // check for first index of space
    int spaceIndex = paramVal.indexOf(" ");
    if (spaceIndex <=0) {
        Serial.println("Invalid Command");
        return;
    }
    String param = paramVal.substring(0,spaceIndex+1).toLowerCase();
    // check the parameter agaiinst the list of parameters
}