#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display = Adafruit_SSD1306();

void oledSetup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(BUTTON_B, INPUT_PULLUP);
}


void oledPrintPos()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (int pinArrayindex = 0; pinArrayindex < sensors; pinArrayindex++)
  {



      display.setTextSize(1);
      display.print("["); display.print(pinArrayindex+1); display.print("]");
      display.setTextSize(2);    display.print(instTime[pinArrayindex], 2);
      display.setTextSize(1);
      display.println("sec ");

    display.display();
    delay(1000);
    display.clearDisplay();
    display.setCursor(0, 0);
  }
}

void oledPrintAcc()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print("A:");  display.println(avgAcc, 1);
  display.println (F("  cm/sec^2"));


  display.setCursor(0, 0);
  display.display();
}
void oledPrintVel()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (int pinArrayindex = 0; pinArrayindex < sensors; pinArrayindex++)
  {

    if (pinArrayindex > 0)
    {

      display.setTextSize(1);
      display.print("["); display.print(pinArrayindex); display.print("-"); display.print(pinArrayindex + 1); display.print("]");
      display.setTextSize(2);    display.print(speedArray[pinArrayindex], 1);
      display.setTextSize(1);
      display.println("cm/sec ");

    }
    display.display();
    delay(1000);
    display.clearDisplay();
    display.setCursor(0, 0);
  }
}
