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
