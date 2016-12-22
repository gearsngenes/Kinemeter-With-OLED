  #define BUTTON_B 6
#define LEDPIN 13

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX | TX
String model = "Model 1";

int const sensors = 4;
int const slots = 2 * sensors;
int pinArray [sensors] = {10, 9,11,5};//, 5,4,3};


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
float disp[sensors] = {0, 5,5,5};
float speedArray[sensors];
float accArray[sensors];

unsigned long curTime;
unsigned long startTime;
//float speed1;

float accSum = 0.0;
float avgAcc = 0.0;

void printPinArray()
{
  Serial.print("Sensor Pins:");
  for (int j=0;j<sensors;j++)
  {Serial.print(pinArray[j]); if (j<sensors-1)Serial.print(",");}
  Serial.println();
}
void printdispArray()
{
  Serial.print("Displacements (cm):");
  for (int j=0;j<sensors;j++)
  {Serial.print(disp[j],0); if (j<sensors-1)Serial.print(", ");}
  Serial.println();
}
void BTprintPinArray()
{
  BTSerial.print("Sensor Pins:");
  for (int j=0;j<sensors;j++)
  {BTSerial.print(pinArray[j]); if (j<sensors-1)BTSerial.print(",");}
  BTSerial.println();
}
void BTprintdispArray()
{
  BTSerial.print("Displacements (cm):");
  for (int j=0;j<sensors;j++)
  { BTSerial.print(disp[j],0); if (j<sensors-1)BTSerial.print(", ");}
  BTSerial.println();
}
void printSelfConfig()
{
   Serial.println (model);
  Serial.print("Number of Sensors =");Serial.println(sensors);
  printPinArray(); 
  printdispArray();Serial.println(); 
}

void BTprintSelfConfig()
{
    delay(20);
  BTSerial.print("Number of Sensors =");BTSerial.println(sensors);
  BTprintPinArray(); delay(20);
  BTprintdispArray();BTSerial.println(); 
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


void processSensorPinat (int pinArrayindex)
{

  sensStateArray[pinArrayindex] = digitalRead(pinArray[pinArrayindex]);
  if  (sensStateArray[pinArrayindex] == LOW)

  {
    // turn LED on:
    digitalWrite(LEDPIN, HIGH);
  }
  else {
    // turn LED off:
    digitalWrite(LEDPIN, LOW);
  }

  if
  (sensStateArray[pinArrayindex] && !lastStateArray[pinArrayindex])

  {
    // Serial.print("At sensor # ");
    timeArray[2 * pinArrayindex + 1] = millis();

    brokenTime[pinArrayindex] = timeArray[2 * pinArrayindex + 1] - timeArray[2 * pinArrayindex];
    instTime[pinArrayindex] = (timeArray[2 * pinArrayindex + 1] + timeArray[2 * pinArrayindex]) / 2000.0;
    //Serial.print("Mid Break:  "); Serial.println(instTime[pinArrayindex],4);
    /*     Serial.print("At sensor # ");
         Serial.print(pinArray[pinArrayindex]); Serial.print("  at T= ");
        Serial.println(instTime[pinArrayindex], 4);
        */
    if (pinArrayindex > 0)
    {

      deltaArray[pinArrayindex] = instTime[pinArrayindex] - instTime[pinArrayindex - 1];
      // Serial.print("Time between Midpoints: "); Serial.println (deltaArray[pinArrayindex]);
      speedArray[pinArrayindex] = disp[pinArrayindex] / deltaArray[pinArrayindex] ;
      AvgTime[pinArrayindex] = (instTime[pinArrayindex] + instTime[pinArrayindex - 1]) / 2;

      /*
      Serial.print(F("           Inst Velocity (avg.) betw. sensors  "));
            Serial.print(pinArray[pinArrayindex - 1]); Serial.print(" and "); Serial.print(pinArray[pinArrayindex]);
            Serial.print(" is ");
            Serial.print(speedArray[pinArrayindex], 4); Serial.print("cm/sec ");
            Serial.print(" at T=  "); Serial.print(AvgTime[pinArrayindex], 4); Serial.println("sec");
      */
    }

    passArray[pinArrayindex] = true;
  }
  if  (!sensStateArray[pinArrayindex] && lastStateArray[pinArrayindex])

  {

    //Serial.print("Sensor["); Serial.print(pinArray[pinArrayindex]); Serial.print("] broke @:");
    timeArray[2 * pinArrayindex] = millis();
    // Serial.print (timeArray[2 * pinArrayindex]);

  }

  lastStateArray[pinArrayindex] = sensStateArray[pinArrayindex];


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

void processAcc()
{
  accSum = 0.0;
  for (int j = 1; j < sensors - 1; j++)
  {


    accl[j] = (speedArray[j + 1] - speedArray[j]) / (AvgTime[j + 1] - AvgTime[j]);
    accSum = accSum + accl[j];

  }
  avgAcc = accSum / (sensors - 2);
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
  if (expnumber ==1) BTprintSelfConfig();

  BTPrintPositions();
 if (  printVel) BTPrintVelocities();
  if (printAcc)  BTPrintAccelerations();


}



void serialPrintVelocities()
{
  Serial.println();
  Serial.print("----"); Serial.print("\t"); Serial.print("Instantaneous Velocities At MID POINTS");  Serial.print("\t"); Serial.println("----");
  for (int pinArrayindex = 0; pinArrayindex < sensors; pinArrayindex++)
  {

    if (pinArrayindex > 0)
    {

      Serial.print(F("           Inst Velocity at midpoint (aka avg. betw.) sensors  S"));
      Serial.print(pinArrayindex); Serial.print(" and S"); Serial.print(pinArrayindex + 1);
      Serial.print(" (Pins ");
      Serial.print(pinArray[pinArrayindex - 1]); Serial.print(" and "); Serial.print(pinArray[pinArrayindex]);
      Serial.print(" )");
      Serial.print(" is ");
      Serial.print(speedArray[pinArrayindex], 4); Serial.print("cm/sec ");
      Serial.print(" at T=  "); Serial.print(AvgTime[pinArrayindex], 4); Serial.println("sec");
    }

  }
}

void serialPrintPositions()
{
  Serial.println();
  Serial.print("----"); Serial.print("\t"); Serial.print("POSITIONS Crossing Sensors");  Serial.print("\t"); Serial.println("----");
  for (int pinArrayindex = 0; pinArrayindex < sensors; pinArrayindex++)
  {
    Serial.print("Crossed Sensor S"); Serial.print (pinArrayindex + 1);
    Serial.print("(Pin #"); Serial.print(pinArray[pinArrayindex]); Serial.print(")"); Serial.print("  at T= ");
    Serial.println(instTime[pinArrayindex], 4);

  }

}

void SerialPrintAccelerations()

{

  Serial.println();
  Serial.print("----"); Serial.print("\t"); Serial.print("ACCELERATIONS between Mid points ");  Serial.print("\t"); Serial.println("----");
  for (int j = 1; j < sensors - 1; j++)
  {

    Serial.print(F(" Acc.betw mid-pt of Sensors: S"));
    Serial.print(j); Serial.print("&"); Serial.print(" S"); Serial.print(j + 1); Serial.print(" (Pins "); Serial.print(pinArray[j - 1]); Serial.print("&"); Serial.print(pinArray[j]); Serial.print(") ");
    Serial.print (" and mid-pt-of "); Serial.print(F("  Sensors: S"));
    Serial.print(j + 1); Serial.print("&"); Serial.print(" S"); Serial.print(j + 2); Serial.print(" (Pins "); Serial.print(pinArray[j]); Serial.print("&"); Serial.print(pinArray[j + 1]); Serial.print(") ");
    Serial.print(" is:   "); Serial.print(accl[j], 4); Serial.println (" cm/sec^2");
  }

  Serial.println();
  Serial.print("----"); Serial.print("\t"); Serial.print("AVERAGE ACC");  Serial.print("\t"); Serial.println("----");
  Serial.print (F("                      Average Acc =  "));
  //Serial.print(accSum / (sensors - 2), 4);
  Serial.print(avgAcc, 4);
  Serial.println (F(" cm/sec^2"));

}



void SerialPrint()

{

  serialPrintPositions();
 if ( printVel)  serialPrintVelocities();
 if (printAcc)  SerialPrintAccelerations();
}

void resetAllDone()
{
  for (int j1 = 0; j1 < sensors; j1++)
  {
    passArray[j1] = false;
  }
}



void loop() {
  // read the state of the pushbutton value:



  for (int x = 0; x < sensors; x++)
  {
    processSensorPinat (x);
  }

  if  (allDone())

  {
    Serial.println("----");
    expnumber = expnumber + 1; Serial.print("Exp number ="); Serial.println(expnumber);
    if (expnumber ==2) {printSelfConfig();};
    processAcc();
    SerialPrint();
    BTPrint();
    oledPrintAcc();
   // delay(1000);
   Serial.println(digitalRead(BUTTON_B));

   // oledPrint();
    resetAllDone();

  }
        if (!digitalRead(BUTTON_B)) {oledPrint(); delay(2000); oledPrintAcc();}

}
