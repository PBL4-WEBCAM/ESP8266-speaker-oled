/*
 * ESP8266 WIFI OLED PLAYER (UDP) - Improved
 */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// --- 1. CẤU HÌNH WIFI ---
// LƯU Ý: Hãy đổi tên Hotspot điện thoại thành KHÔNG DẤU để tránh lỗi
// VÍ DỤ: Đổi "Ai pỏn" thành "Ai pon"
const char* ssid = "iphone cua tau";      // <--- Đổi tên WiFi thành không dấu
const char* password = "123123123";     
unsigned int localPort = 4210;      

// --- CẤU HÌNH OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA D2   
#define OLED_SCL D1   
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- CẤU HÌNH DFPLAYER ---
SoftwareSerial mySoftwareSerial(D6, D7); 
DFRobotDFPlayerMini myDFPlayer;

WiFiUDP Udp;
char packetBuffer[255]; 

void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  // 1. Khởi động OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED failed"));
    while(1) delay(100);
    Serial.println("Testing Sound...");
  myDFPlayer.volume(30); // Max volume
  myDFPlayer.play(1);
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setFont(NULL);
  display.setCursor(0, 10);
  display.println(F("Connecting WiFi..."));
  display.println(ssid); // Hiện tên WiFi đang thử kết nối
  display.display();

  // 2. Kết nối WiFi
  WiFi.begin(ssid, password);
  
  int retry_count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retry_count++;
    
    // Nếu chờ quá 20 lần (10 giây) mà chưa được
    if (retry_count > 20) {
       display.clearDisplay();
       display.setCursor(0,0);
       display.println(F("Ket noi that bai!"));
       display.println(F("Kiem tra:"));
       display.println(F("1. Ten WiFi (bo dau)"));
       display.println(F("2. Bat 2.4GHz (iPhone)"));
       display.display();
       retry_count = 0; // Reset bộ đếm để thử tiếp (hoặc treo máy tùy bạn)
       delay(2000);
       display.clearDisplay();
       display.setCursor(0,0);
       display.println(F("Retrying..."));
       display.display();
    }
  }

  // 3. Khởi động UDP Server
  Udp.begin(localPort);

  // 4. Hiển thị IP
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("WiFi Connected!"));
  display.println(F("IP Address:"));
  display.setTextSize(2); 
  display.println(WiFi.localIP()); 
  display.display();

  // 5. Khởi động DFPlayer
  delay(2000); 
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("DFPlayer Error"));
  } else {
    myDFPlayer.volume(25);
  }
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0; 
    }
    
    String textFromPC = String(packetBuffer);
    
    int firstPipe = textFromPC.indexOf('|');
    int secondPipe = textFromPC.lastIndexOf('|');

    if (firstPipe != -1 && secondPipe != -1) {
      String eng = textFromPC.substring(0, firstPipe);
      String vie = textFromPC.substring(firstPipe + 1, secondPipe);
      String idStr = textFromPC.substring(secondPipe + 1);
      int id = idStr.toInt();

      display.clearDisplay();
      
      display.setFont(NULL); 
      display.setTextSize(2);
      int16_t x1, y1; uint16_t w, h;
      display.getTextBounds(eng, 0, 0, &x1, &y1, &w, &h);
      display.setCursor((SCREEN_WIDTH - w) / 2, 10);
      display.print(eng);

      display.setFont(&FreeSans9pt7b);
      display.setTextSize(1);
      display.getTextBounds(vie, 0, 0, &x1, &y1, &w, &h);
      display.setCursor((SCREEN_WIDTH - w) / 2, 45);
      display.print(vie);
      
      display.display();

      myDFPlayer.play(id + 1);
    }
  }
  yield();
}