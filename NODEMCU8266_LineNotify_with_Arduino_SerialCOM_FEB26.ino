#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <TridentTD_LineNotify.h>
#define SSID        "Manida"      // บรรทัดที่ 11 ให้ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
#define PASSWORD    "10042490"     // บรรทัดที่ 12 ใส่ รหัส Wifi
#define LINE_TOKEN  "Q5PrYbFxsvuGPdIKq4qe4FK8SOZK5Nkl0ZHq7MyxkTr"   // บรรทัดที่ 13 ใส่ รหัส TOKEN ที่ได้มาจากข้างบน
SoftwareSerial NodeSerial(D2, D3); // RX | TX
int x =0;

void setup() {
  int n =0;
  int x =0;
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  Serial.begin(9600);
  NodeSerial.begin(9600);
 
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);  
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
}

void loop() {  
  
  while (NodeSerial.available() > 0)
  {
    int n = NodeSerial.parseInt();

    if (NodeSerial.read() == '\n')
    {
      if (x == n) {

      }
      else if(x != n){
        if(n == 1){
          LINE.setToken(LINE_TOKEN);
          LINE.notify("ปั๊มทำงานแล้วนะค่ะ");
          LINE.notify("น้ำออกปกติค่ะ");
          LINE.notifySticker(3, 240);
          Serial.println("n = 1");
          x = 1;         
        }
        else if(n == 2) {
           LINE.setToken(LINE_TOKEN);
          LINE.notify("น้ำไม่เข้าปั๊ม ปิดปั๊มแล้วค่ะ");
          LINE.notify("ตรวจสอบน้ำเข้า แล้วปิดเปิดใหม่นะคะ");
          LINE.notifySticker(3, 240);
          Serial.println("n = 2");
          x = 2; 
        }
        else if( n == 3){
          LINE.setToken(LINE_TOKEN);
          LINE.notify("มอเตอร์ร้อนเกิน 70 องศาค่ะ ปิดปั๊มแล้วนะค่ะ");
          LINE.notify("ตรวจสอบน้ำเข้า ตรวจสอบมอเตอร์ แล้วปิดเปิดใหม่นะคะ");
          LINE.notifySticker(3, 240);
          Serial.println("n = 3");
          x = 3;          
        }
        else if(n == 4){
          LINE.setToken(LINE_TOKEN);
          LINE.notify("ปิดปั๊มแล้วค่ะ");
          LINE.notifySticker(3, 240);
          Serial.println("n = 4");
          x = 4;          
        }  
      }
    }
    delay(1000);
  }

}