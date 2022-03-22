#include <SoftwareSerial.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 //กำหนดขาที่จะเชื่อมต่อ Sensor
SoftwareSerial UnoSerial(11, 10); // RX | TX
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 20, 4);



//int ledPin = 13;  // LED connected to digital pin 13

//int esp_lineNoWaterPin = 9; // esp-02 Line NO Output Water Notify
//int esp_webPin = 8;  // esp8266-01 on off via web server
int sw1Pin = 7;   // ON OFF Switch
int woPin = 6;    // Water output Sensor
int ry1Pin = 5;    //  AC relay1 pin
int ry2Pin =4;    // AC relay2 pin

  //int esp_web = 0;
  int sw1 = 0;
  int wo = 0;
  int ry1 =0;
  int ry2 =0;
  int n =0;   // water output flag
  int i =0;
  int r =0;
  float tempCelsius;
 
void setup() {
  //pinMode(ledPin, OUTPUT);
  //pinMode(esp_lineNoWaterPin, OUTPUT);
  //pinMode(esp_webPin, INPUT);
  pinMode(11, INPUT);
  pinMode(10, OUTPUT);
  pinMode(sw1Pin, INPUT);
  pinMode(woPin, INPUT);
  pinMode(ry1Pin, OUTPUT);
  pinMode(ry2Pin, OUTPUT);
  
lcd.begin();
Serial.begin(9600);
sensors.begin();
UnoSerial.begin(9600); 
int n = 0;  // n = 0 no water output , n = 1 water output is ok, n = 2 water out put gone.
int i = 0;
int r =0; //relay flag
float tempCelsius;    // temperature in Celsius

lcd.backlight();
lcd.setBacklight(HIGH);

}

void loop() {
//==== State 1 Pump Start =======
sw1 = digitalRead(sw1Pin);
if (sw1 ==HIGH){
if (n == 0){       //  n = 0 is in the initial state to start the motor
  lcd.clear();
  lcd.setCursor (0, 0 );
  lcd.print("Switch is ON");
  lcd.setCursor (0, 1 );
  lcd.print("Checking Water");
  lcd.setCursor (0, 3);
  lcd.print("Please wait");
  digitalWrite(ry1Pin, HIGH);
  digitalWrite(ry2Pin, HIGH);
  delay(10000);      // wait 10 second for Water out
  wo = digitalRead(woPin);
    if(wo == LOW){
      Serial.println("No water out put Pump will stop 5 sec the restart automaticaly");
      digitalWrite(ry1Pin, LOW);
      digitalWrite(ry2Pin, LOW);
      delay(5000);     // wait 5 sec.for recharging water in the pump
      digitalWrite(ry1Pin, HIGH);
      digitalWrite(ry2Pin, HIGH);
      delay(3000);    // wait 3 sec. for monitoring the Water output
      wo = digitalRead(woPin); 
      if (wo == LOW) {
        Serial.println("No water out put Confirm, Pump is off");
        Serial.println("please check the water source");
        digitalWrite(ry1Pin, LOW);
        digitalWrite(ry2Pin, LOW);
        Serial.println("Motor STOP Please check Water Input");
        Serial.println("then Restart the system");
        n = 3;
        delay(3000);
      }
      else if(wo ==HIGH) {
        n = 1;
         Serial.println("n = 1 change to namal running");
        sensors.requestTemperatures(); //อ่านข้อมูลจาก library
        tempCelsius = sensors.getTempCByIndex(0);
        lcd.clear();
        lcd.setCursor (0, 0 );
        lcd.print("Water output is ok  ");
        lcd.setCursor (0, 1 );
        lcd.print("Motor is running");
        UnoSerial.print(n);
        UnoSerial.print("\n");
        lcd.setCursor(0, 2);
        lcd.print("temp:");
        lcd.setCursor(7, 2);       // start to print at the first row
        lcd.print(tempCelsius);    // print the temperature in Celsius
        lcd.print((char)223);      // print ° character
        lcd.print("C");
        delay(2000);
      }
    }
    else if(wo == HIGH) {
      n = 1;
      Serial.println("n = 1 change to namal running");
      sensors.requestTemperatures(); //อ่านข้อมูลจาก library
      tempCelsius = sensors.getTempCByIndex(0);
      lcd.clear();
      lcd.setCursor (0, 0 );
      lcd.print("Water output is ok  ");
      lcd.setCursor (0, 1 );
      lcd.print("Motor is running");
      UnoSerial.print(n);
      UnoSerial.print("\n");
      lcd.setCursor(0, 2);
      lcd.print("temp:");
      lcd.setCursor(7, 2);       // start to print at the first row
      lcd.print(tempCelsius);    // print the temperature in Celsius
      lcd.print((char)223);      // print ° character
      lcd.print("C");
      delay(2000);
    }
}
else if(n == 1) {    // water out is ok state
  wo = digitalRead(woPin);
  if (wo == LOW){
    n =2;
    Serial.println("n = 2 change to No water out detected");
    delay(2000);
  }
  else if(wo == HIGH){
  n = 1;
  digitalWrite(ry1Pin, HIGH);
  digitalWrite(ry2Pin, HIGH);
  Serial.println("Water out is ok, Motor is running normal");
  lcd.clear();
  lcd.setCursor (0, 0 );
  lcd.print("Water output is ok  ");
  lcd.setCursor (0, 1 );
  lcd.print("Motor is running");
  UnoSerial.print(n);
  UnoSerial.print("\n");
  Serial.println("Requesting temperatures...");
    
  //Print sensor results
  lcd.setCursor(0, 2);
  lcd.print("temp:");
  lcd.setCursor(7, 2);       // start to print at the first row
  lcd.print(tempCelsius);    // print the temperature in Celsius
  lcd.print((char)223);      // print ° character
  lcd.print("C");

  if (tempCelsius >= 60){
    n = 3;
  }
  //Serial.print("Temperature is: ");
  //Serial.print(sensors.getTempCByIndex(0)); // แสดงค่า อูณหภูมิ 
  //Serial.println(" *C");
  
  delay(1000);
  }
}
else if(n == 2) {   // no water out detected state
  digitalWrite(ry1Pin, LOW);
  digitalWrite(ry2Pin, LOW);
  lcd.clear();
  lcd.setCursor (0, 0 );
  lcd.print("No Water Output");
  lcd.setCursor (0, 1 );
  lcd.print("Motor is STOP");
  lcd.setCursor (0, 2 );
  lcd.print("Pls.Check Input");
  lcd.setCursor (0, 3 );
  lcd.print("Then Restart Again");
  Serial.println("Motor STOP, No water output");
  Serial.println("Pls Chk Water in put then restart again");
  n = 2;
  UnoSerial.print(n);
  UnoSerial.print("\n");
  delay(2000);
}
else if(n == 3){
  digitalWrite(ry1Pin, LOW);
  digitalWrite(ry2Pin, LOW);
  lcd.clear();
  lcd.setCursor (0, 0 );
  lcd.print("Over Temperature");
  lcd.setCursor (0, 1 );
  lcd.print("Motor is STOP");
  lcd.setCursor (0, 2 );
  lcd.print("Temp.> 60'C.");
  lcd.setCursor (0, 3 );
  lcd.print("I/P Water or Motor");
  Serial.println("Over Temperature");
  Serial.println("Motor OFF");
  Serial.println("Pls Chk Input water or Motor");
  n = 3;
  UnoSerial.print(n);
  UnoSerial.print("\n");
  delay(2000);
}
} // end of "if sw1  == HIGH "
else if(sw1 == LOW){        // else of if sw1 check " sw1 == LOW"
  n = 0;
  r = 4;
  digitalWrite(ry1Pin, LOW);
  digitalWrite(ry2Pin, LOW);
  lcd.clear();
  lcd.setCursor (0, 0 );
  lcd.print("Switch is OFF ");
  Serial.println("Switch if off");
  UnoSerial.print(r);
  UnoSerial.print("\n");
  delay(2000);
}
} // return of void loop