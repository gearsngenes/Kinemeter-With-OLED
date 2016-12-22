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
    timeArray[2 * pinArrayindex + 1] = millis();

    brokenTime[pinArrayindex] = timeArray[2 * pinArrayindex + 1] - timeArray[2 * pinArrayindex];
    instTime[pinArrayindex] = (timeArray[2 * pinArrayindex + 1] + timeArray[2 * pinArrayindex]) / 2000.0;
    if (pinArrayindex > 0)
    {

      deltaArray[pinArrayindex] = instTime[pinArrayindex] - instTime[pinArrayindex - 1];
      // Serial.print("Time between Midpoints: "); Serial.println (deltaArray[pinArrayindex]);
      speedArray[pinArrayindex] = disp[pinArrayindex] / deltaArray[pinArrayindex] ;
      AvgTime[pinArrayindex] = (instTime[pinArrayindex] + instTime[pinArrayindex - 1]) / 2;
    }

    passArray[pinArrayindex] = true;
  }
  if  (!sensStateArray[pinArrayindex] && lastStateArray[pinArrayindex])

  {
    timeArray[2 * pinArrayindex] = millis();
  }
  lastStateArray[pinArrayindex] = sensStateArray[pinArrayindex];
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
