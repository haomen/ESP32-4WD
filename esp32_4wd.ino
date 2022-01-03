#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <Adafruit_MotorShield.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "xiaobaita";
const char* password = "starcraftforever";
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *fr = AFMS.getMotor(1); // Front right
Adafruit_DCMotor *fl = AFMS.getMotor(2); // Front Left
Adafruit_DCMotor *rl = AFMS.getMotor(3); // Rear Left
Adafruit_DCMotor *rr = AFMS.getMotor(4); // Rear Right

String translateEncryptionType(wifi_auth_mode_t encryptionType) {

  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
  // return "";
}

void scanNetworks() {

  int numberOfNetworks = WiFi.scanNetworks();

  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);

  for (int i = 0; i < numberOfNetworks; i++) {

    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));

    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));

    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));

    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");

  }
}

void connectToNetwork() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }

  Serial.println("Connected to network");
}

void printOled(char* line1, char* line2, char* line3, char* line4) {
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println(F(line1));
  display.setCursor(0, 8);
  display.println(F(line2));
  display.setCursor(0, 16);
  display.println(F(line3));
  display.setCursor(0, 24);
  display.println(F(line4));
  display.display();
  delay(2000);
}

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Initializing motors"));
  display.display();
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    display.setCursor(0, 8);
    display.println(F("Motors not detected"));
    display.display();
    while (1);
  }
  display.setCursor(0, 8);
  display.println(F("Motors initialized"));
  display.display();

  // Set the speed to start, from 0 (off) to 255 (max speed)
  fl->setSpeed(150);
  fl->run(FORWARD);
  fr->setSpeed(150);
  fr->run(FORWARD);
  rl->setSpeed(150);
  rl->run(FORWARD);
  rr->setSpeed(150);
  rr->run(FORWARD);

  // turn on motor
  fl->run(RELEASE);
  fr->run(RELEASE);
  rl->run(RELEASE);
  rr->run(RELEASE);
  display.setCursor(0, 16);
  display.println(F("Motors ready!"));
  display.display();
  delay(2000);
  
  // scanNetworks();
  // connectToNetwork();
  // Serial.println(WiFi.macAddress());
  // Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println("Move forward");
  printOled("Motor running", "Moving Forward", "Running 3 secs", "-");
  fl->run(FORWARD);
  fr->run(FORWARD);
  rl->run(FORWARD);
  rr->run(FORWARD);
  fl->setSpeed(100);
  fr->setSpeed(100);
  rl->setSpeed(100);
  rr->setSpeed(100);
  delay(3000);

  Serial.println("Move backward");
  printOled("Motor running", "Moving Backward", "Running 3 secs", "-");
  fl->run(BACKWARD);
  fr->run(BACKWARD);
  rl->run(BACKWARD);
  rr->run(BACKWARD);
  fl->setSpeed(100);
  fr->setSpeed(100);
  rl->setSpeed(100);
  rr->setSpeed(100);
  delay(3000);

  Serial.println("Move Left");
  printOled("Motor running", "Moving Left", "Running 3 secs", "-");
  fl->run(FORWARD);
  fr->run(BACKWARD);
  rl->run(FORWARD);
  rr->run(BACKWARD);
  fl->setSpeed(100);
  fr->setSpeed(100);
  rl->setSpeed(100);
  rr->setSpeed(100);
  delay(3000);

  Serial.println("Move Right");
  printOled("Motor running", "Moving Right", "Running 3 secs", "-");
  fl->run(BACKWARD);
  fr->run(FORWARD);
  rl->run(BACKWARD);
  rr->run(FORWARD);
  fl->setSpeed(100);
  fr->setSpeed(100);
  rl->setSpeed(100);
  rr->setSpeed(100);
  delay(3000);


  Serial.println("Now realease");
  printOled("Motor stop", "Release", "1 sec", "-");
  fl->run(RELEASE);
  fr->run(RELEASE);
  rl->run(RELEASE);
  rr->run(RELEASE);
  delay(1000);

}
