#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MAX31865.h>

#define OLED_RESET 4
#define RREF 430.0

Adafruit_SSD1306 display(OLED_RESET);
Adafruit_MAX31865 max = Adafruit_MAX31865(10);

float minTemp = 1000.0;
float maxTemp = 0;
float lastTemp = maxTemp;

void setup(){
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  max.begin(MAX31865_3WIRE); 
  introText();
}

void loop(){
  if(checkFaults()){
    displayErrorText();
    delay(2000);//wait longer in this case
    display.clearDisplay();
    display.display();
    display.invertDisplay(false);
  }else{
    readTemp();
    updateDisplay();
  }

  //TODO: if pin x == HIGH = resetTemp();

  delay(200);
}

void updateDisplay(){
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.print("currently: ");
  display.println(lastTemp);
  display.println("");
  display.print("min :   ");
  display.println(minTemp);
  display.print("max :   ");
  display.println(maxTemp);


  display.display();
}

bool checkFaults(){
  // Check and print any faults
  uint8_t fault = max.readFault();
  if (fault) {
    Serial.println(fault);
    max.clearFault();
    return true;
  }
  return false;

}

void displayErrorText(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.invertDisplay(true);
  display.println("");
  display.setTextSize(2);
  display.println("ERROR");
  display.display();

}

void resetTemp(){
  minTemp = 1000.0;
  maxTemp = 0;
  lastTemp = maxTemp;
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.invertDisplay(true);
  display.println("");
  display.println("- - - - - - - - - - -");
  display.println("     RESET RANGE     ");
  display.println("- - - - - - - - - - -");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  display.invertDisplay(false);

}

void readTemp(){

  uint16_t rtd = max.readRTD();
  float ratio = rtd;
  ratio /= 32768;

  //Serial.print("RTD value: "); Serial.println(rtd);
  //Serial.print("Ratio = "); Serial.println(ratio,8);
  //Serial.print("Resistance = "); Serial.println(RREF*ratio,8);
 
  lastTemp = max.temperature(100, RREF);
  if(lastTemp > maxTemp){
    maxTemp = lastTemp;
  }
  if(lastTemp < minTemp){
    minTemp = lastTemp;
  }

}

void introText(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.invertDisplay(true);
  display.println("     WATTS RULES     ");
  display.println("    OTHERS DROOL     ");
  display.println("- - - - - - - - - - -");
  display.println("        2017         ");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  display.invertDisplay(false);
}

