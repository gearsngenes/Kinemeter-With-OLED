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
