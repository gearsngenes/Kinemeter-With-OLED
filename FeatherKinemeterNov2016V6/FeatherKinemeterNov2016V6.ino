#define BUTTON_B 6
#define LEDPIN 13

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX | TX
String model = "Model 1";

int const sensors = 4;
int const slots = 2 * sensors;
int pinArray [sensors] = {10, 9, 11, 5}; //, 5,4,3};

int buttonCount = 0;
unsigned long brokenTime[sensors];
float AvgTime[sensors];
float instTime[sensors];
float accl[sensors];

boolean printVel = true;
boolean printAcc = true;
int expnumber = 0;

boolean passArray[sensors];
int sensStateArray[sensors];
int lastStateArray[sensors];
unsigned long timeArray[slots];
float deltaArray[sensors];
//const float DIST = 5.0;
float disp[sensors] = {0, 5, 5, 5};
float speedArray[sensors];
float accArray[sensors];

unsigned long curTime;
unsigned long startTime;
//float speed1;

float accSum = 0.0;
float avgAcc = 0.0;



void BTprintPinArray()
{
  BTSerial.print("Sensor Pins:");
  for (int j = 0; j < sensors; j++)
  {
    BTSerial.print(pinArray[j]);
    if (j < sensors - 1)BTSerial.print(",");
  }
  BTSerial.println();
}
void BTprintdispArray()
{
  BTSerial.print("Displacements (cm):");
  for (int j = 0; j < sensors; j++)
  {
    BTSerial.print(disp[j], 0);
    if (j < sensors - 1)BTSerial.print(", ");
  }
  BTSerial.println();
}
void BTprintSelfConfig()
{
  delay(20);
  BTSerial.print("Number of Sensors ="); BTSerial.println(sensors);
  BTprintPinArray(); delay(20);
  BTprintdispArray(); BTSerial.println();
}

void BTSetup ()

{
  BTSerial.begin(9600);
  BTSerial.println("BT Demo");
  startTime = millis();
  curTime = startTime;
}

void setup() {
  // initialize the LED pin as an output:
  pinMode(LEDPIN, OUTPUT);

  // initialize the sensor pin as an input:
  for (int j1 = 0; j1 < sensors; j1++)
  {
    pinMode(pinArray[j1], INPUT);
    digitalWrite(pinArray[j1], HIGH); // turn on the pullup
    passArray[j1] = false;
  }
  Serial.begin(9600);

  Serial.println(F("Set up Complete."));
  BTSetup();
  oledSetup();
}




boolean allDone() //all sensors have passed through
{
  boolean output = true;

  for (int j2 = 0; j2 < sensors; j2++)
  {
    if (passArray[j2] == false) {
      output = false; // if even one is false, the total result is false.
    }
  }
  return output;
}



void BTPrintAccelerations()
{
  BTSerial.println();
  BTSerial.print("----"); BTSerial.print("\t"); BTSerial.print("Accelerations ");  BTSerial.print("\t"); BTSerial.println("----");
  for (int j = 1; j < sensors - 1; j++)
  {

    BTSerial.print(F(" Acc[")); BTSerial.print(j); BTSerial.print("]=");
    BTSerial.print(accl[j], 4); BTSerial.println (" cm/s^2");
  }
  delay(20);
  BTSerial.println();
  BTSerial.print(F(" Avg Acc=  ")); BTSerial.print(avgAcc, 4); BTSerial.println (" cm/s^2");
}

void BTPrintPositions()
{
  BTSerial.println();
  BTSerial.print("----"); BTSerial.print("\t"); BTSerial.print("POSITIONS Crossing Sensors");  BTSerial.print("\t"); BTSerial.println("----");
  for (int pinArrayindex = 0; pinArrayindex < sensors; pinArrayindex++)
  {
    BTSerial.print("Crossed Sensor S"); BTSerial.print (pinArrayindex + 1);
    BTSerial.print("(Pin #"); BTSerial.print(pinArray[pinArrayindex]); BTSerial.print(")"); BTSerial.print("  at T= ");
    BTSerial.println(instTime[pinArrayindex], 4);

  }

}

void BTPrintVelocities()
{
  BTSerial.println();
  BTSerial.print("----"); BTSerial.print("\t"); BTSerial.print("Instantaneous Velocities At MID POINTS");  BTSerial.print("\t"); BTSerial.println("----");
  for (int pinArrayindex = 0; pinArrayindex < sensors; pinArrayindex++)
  {

    if (pinArrayindex > 0)
    {

      BTSerial.print(F(" Inst Vel Betw S"));
      BTSerial.print(pinArrayindex); BTSerial.print(" & S"); BTSerial.print(pinArrayindex + 1);

      BTSerial.print(": ");
      BTSerial.print(speedArray[pinArrayindex], 2); BTSerial.print("cm/sec ");
      BTSerial.print(" at T="); BTSerial.print(AvgTime[pinArrayindex], 2); BTSerial.println("sec");
    }

  }
}
void BTPrint()
{
  BTSerial.println();
  BTSerial.print("\t"); BTSerial.print("\t"); BTSerial.print("Exp number ="); BTSerial.println(expnumber);
  if (expnumber == 1) BTprintSelfConfig();

  BTPrintPositions();
  if (  printVel) BTPrintVelocities();
  if (printAcc)  BTPrintAccelerations();


}

void resetAllDone()
{
  for (int j1 = 0; j1 < sensors; j1++)
  {
    passArray[j1] = false;
  }
}



void loop() {
  for (int x = 0; x < sensors; x++)
  {
    processSensorPinat (x);
  }

  if  (allDone())

  {
    Serial.println("----");
    expnumber = expnumber + 1; Serial.print("Exp number ="); Serial.println(expnumber);
    if (expnumber == 2) {
      printSelfConfig();
    };
    processAcc();
    SerialPrint();
    BTPrint();
    oledPrintAcc();

    resetAllDone();

  }
  if (!digitalRead(BUTTON_B)) {
    buttonCount++;
    if (buttonCount % 3 == 1) {
      oledPrintPos();
    }
    else if (buttonCount % 3 == 2) {
      oledPrintVel();
    }
    else if (buttonCount % 3 == 0) {
      oledPrintAcc();
    }
    delay(1000);
    Serial.println("==="); Serial.print("button Count:"); Serial.println(buttonCount);

  }

}
