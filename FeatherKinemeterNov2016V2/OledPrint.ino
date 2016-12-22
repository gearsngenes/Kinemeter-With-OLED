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


void oledPrint()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (int pinArrayindex = 0; pinArrayindex < sensors; pinArrayindex++)
  {
    display.print("["); display.print (pinArrayindex + 1);
    display.print("]"); display.println(instTime[pinArrayindex], 2);

  }
 
  display.setCursor(0, 0);
  display.display();
}

void oledPrintAcc()
{
    display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

   display.print("A:");  display.println(avgAcc,1);
  display.println (F("  cm/sec^2"));

 
  display.setCursor(0, 0);
  display.display();
}

