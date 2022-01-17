//Board:  Heltec Wifi Kit32
//https://www.w3schools.com/bootstrap/ to create index.h

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Tone32.h>  //https://github.com/lbernstone/Tone
#include "UserSettings.h"

// WiFi and AP
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include "index.h"

// Display Settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels  128
#define SCREEN_HEIGHT 64 // OLED display height, in pixels  64
#define OLED_RESET     16 // For built in OLED wifikit8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
U8G2_FOR_ADAFRUIT_GFX u8display;

//  EEPROM configuration - 24 bytes
#define EEPROM_SIZE 24  /* new */

//  Settings for AP
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 10, 2); // hard code AP IP address (optional - default is 192.168.4.1)
DNSServer dnsServer;
WebServer server(80); //Server on port 80

// Intitialize variables
unsigned long previousTime = 0; // store the last time the led variable was updated
int led = 0;  //used as a counting variable and doubles as the number of LEDs to be lit
double cum_led = 0;  //cumulative count
bool walktime = true;
bool useEEPROMvalues = false;
bool useEEPROMnextboot = false;
bool toneOn = true;
int x_offset = 12;  //offset for led

// 600000 = 10 minutes in milliseconds (10 * 60 * 1000)
// 60000 = 1 minute in milliseconds
// 6000 = 6 sec in milliseconds
long interval = 10000; // 10sec interval - increments LED count
long setupInterval = 10000; // interval before moving out of setup automatically
int selectmode = 0;  // variable to store display function mode

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}
//===============================================================
// This routine is executed when you press submit
//===============================================================
void handleForm() {
  String run__duration = server.arg("runTime");
  String walk__duration = server.arg("walkTime");
  String tone__on = server.arg("tone");

  // this is a cool way to convert string from the web page to an integer
  run_duration = atoi(run__duration.c_str());
  walk_duration = atoi(walk__duration.c_str());

  run_duration = run_duration * 1000 / interval;
  walk_duration = walk_duration * 1000 / interval;

  Serial.print("Run Duration:");
  Serial.println(run_duration);

  Serial.print("Walk Duration:");
  Serial.println(walk_duration);

  toneOn = (tone__on == "on") ? true : false;
  Serial.print("Tone is:  "); Serial.println(toneOn);

  selectmode = 1;  //set to one to start the run / walk cycle
  Serial.print("Selectmode = "); Serial.println(selectmode);

  led = 0; //set led count back to zero on new settings
  cum_led = 0;  //reset cumulative time back to zero

  useEEPROMnextboot = 1;  //flag to read from eeprom

  //Update web page with input values
  server.send(200, "text/html", run__duration + " " + walk__duration); //Send web page

  //Write values to EEPROM for next boot
  EEPROM.write(0, run_duration);
  EEPROM.write(1, walk_duration);
  EEPROM.write(2, useEEPROMnextboot);
  EEPROM.commit();


  // reset first display
  display.clearDisplay();
  u8display.setCursor(x_offset, 10);
  u8display.print("Walking: ");
  u8display.print(led * 10);
  u8display.println(" sec");
  display.drawBitmap(
    ((display.width() - 32 ) / 2) + 45,
    ((display.height() - 32) / 2) + 10,
    myWalker, 32, 32, 1);
  u8display.setCursor(x_offset, 25);
  u8display.print("Total: ");
  u8display.print(cum_led * interval / (60 * 1000));  u8display.println(" min");
  display.fillRect(0, 60, (10 * (walk_duration - led) * 127) / (10 * walk_duration), 4, WHITE);
  display.display();
}


void setup() {
  unsigned long currentTime = millis();

  // Set up EEPROM storage
  //===============================================================
  EEPROM.begin(EEPROM_SIZE);

  // initialize display
  Wire.begin(4, 15);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  u8display.begin(display);  //connect u8g2 to adafruit gfx
  u8display.setFont(u8g2_font_ordinarybasis_tr); //u8g2_font_ordinarybasis_t_all
  //u8display.setFont(u8g2_font_seraphimb1_tr);
  //u8display.setFont(u8g2_font_koleeko_tf);
  //u8display.setFont(u8g2_font_DigitalDiscoThin_te);
  u8display.setFontMode(1);
  u8display.setFontDirection(0);
  u8display.setForegroundColor(WHITE);
  u8display.setCursor(0, 10);
  display.setRotation(0);  //0 is normal, 1 is 90, 2 is 180 and 3 is 270deg

  Serial.begin(115200);

  //Settings for webserver
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP("runwalk", "12345678");    //Password length minimum 8 char
  //WiFi.softAPConfig(local_ip, gateway, subnet);  //Optional - only if you don't want the default
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  dnsServer.setTTL(300); // modify TTL with the domain name from 60 to 300seconds
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, "www.walkrun.com", apIP);  // set website name

  //Map server actions to functions
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here
  server.begin();                  //Start server
  Serial.println("HTTP server started");
  display.clearDisplay();
  u8display.setCursor(x_offset, 30);
  u8display.println("Connect Wifi to");
  u8display.setCursor(x_offset, 45);
  u8display.print("runwalk");
  display.display();
  display.setCursor(0, 10);

  delay(3000);  // delay to allow connection

  display.clearDisplay();
  u8display.setCursor(x_offset, 30);
  u8display.print("go to: ");
  u8display.print("192.168.10.2");
  display.display();

delay(1000);  //delay to read

  // convert walk_duration to number of loop cycles based on interval
  walk_duration = walk_duration * 1000 / interval; // convert to ms then divide by interval which is also in ms
  run_duration = run_duration * 1000 / interval;
  Serial.print("walk_duration:  ");  Serial.println(walk_duration);

  //Read values from eeprom if flag is set
  useEEPROMnextboot = EEPROM.read(2);
  // test to see if values were stored in EEPROM and load them
  Serial.print("useEEPROMnextboot from EEPROM: "); Serial.println(useEEPROMnextboot);
  if (useEEPROMnextboot = 1) {
    run_duration = EEPROM.read(0);
    walk_duration = EEPROM.read(1);
    Serial.println("*****From EEPROM**********");
    Serial.print("walk_duration: "); Serial.println(walk_duration);
    Serial.print("run_duration: "); Serial.println(run_duration);
  }

  Serial.println("Setup Complete");
  Serial.print("Selectmode = "); Serial.println(selectmode);

}

void loop() {

  // store the time since the Arduino started running in a variable
  unsigned long currentTime = millis();

  switch (selectmode) {

    case 0:  // walking

      server.handleClient();          //Handle client requests


      if (currentTime - previousTime > setupInterval) {
        previousTime = currentTime;

        // change selectmode to start run/walk cycle
        selectmode = 1;
        Serial.print("Selectmode: "); Serial.println(selectmode);

        led = 0;
        // first display
        display.clearDisplay();
        u8display.setCursor(x_offset, 10);
        u8display.print("Walking: ");
        u8display.print(led * 10);
        u8display.println(" sec");
        display.drawBitmap(
          ((display.width() - 32 ) / 2) + 45,
          ((display.height() - 32) / 2) + 10,
          myWalker, 32, 32, 1);
        u8display.setCursor(x_offset, 25);
        u8display.print("Total: ");
        u8display.print(cum_led * interval / (60 * 1000));  u8display.println(" min");
        display.fillRect(0, 60, (10 * (walk_duration - led) * 127) / (10 * walk_duration), 4, WHITE);
        display.display();
      }
      break;

    case 1:  //running

      server.handleClient();          //Handle client requests

      // begin walk / run alternate timing
      if (walktime == false) {
        if (currentTime - previousTime > interval) {
          previousTime = currentTime;
          // increment the led variable
          led++;
          cum_led++;

          // update Display
          display.clearDisplay();
          u8display.setCursor(x_offset, 10);
          u8display.print("Running: ");
          u8display.print(led * 10);
          u8display.println(" sec");
          display.drawBitmap(
            ((display.width() - 32 ) / 2) + 45,
            ((display.height() - 32) / 2) + 10,
            myRunner, 32, 32, 1);
          u8display.setCursor(x_offset, 25);
          u8display.print("Total: ");
          u8display.print(cum_led * interval / (60 * 1000));  u8display.println(" min");
          display.fillRect(0, 60, (10 * (run_duration - led) * 127) / (10 * run_duration), 4, WHITE); // display.fillrect(xi,yi,width,height,color)
          display.display();
          if (led == 2) {
            u8display.setCursor(x_offset, 45);
            u8display.println("Doing great!");
            display.display();
          }
          if (led == 4) {
            u8display.setCursor(x_offset, 45);
            u8display.println("Awesome!");
            display.display();
          }


          // check if run duration is complete
          if (led >= run_duration) {  //if interval is set to 10 sec, then this loop triggers at the end of 60 seconds
            // time is up
            led = 0;
            display.clearDisplay();
            u8display.setCursor(x_offset, 10);
            //u8display.print("Running: ");
            u8display.print("Walking: ");
            u8display.print(led * 10);
            u8display.println(" sec");
            display.drawBitmap(
              ((display.width() - 32 ) / 2) + 45,
              ((display.height() - 32) / 2) + 10,
              myWalker, 32, 32, 1);
            display.display();
            u8display.setCursor(x_offset, 25);
            u8display.print("Total: ");
            u8display.print(cum_led * interval / (60 * 1000));  u8display.println(" min");
            u8display.setCursor(x_offset, 45);
            u8display.println("Time to Walk!");
            display.fillRect(0, 60, (10 * (run_duration - (led)) * 127) / (10 * run_duration), 4, WHITE);
            display.display();
            //led = 0;
            walktime = true;
            if (toneOn == true) {
              playdowntone();
            }
          }
        }
      }

      // repeat countup for Walking

      if (walktime == true) {
        if (currentTime - previousTime > interval) {
          previousTime = currentTime;
          // increment led variable
          led++;
          cum_led++;

          // update display
          display.clearDisplay();
          u8display.setCursor(x_offset, 10);
          u8display.print("Walking:  ");
          u8display.print(led * 10);
          u8display.println(" sec");
          display.drawBitmap(
            ((display.width() - 32 ) / 2) + 45,
            ((display.height() - 32) / 2) + 10,
            myWalker, 32, 32, 1);
          u8display.setCursor(x_offset, 25);
          u8display.print("Total: ");
          u8display.print(cum_led * interval / (60 * 1000));  u8display.println(" min");
          display.fillRect(0, 60, (10 * (walk_duration - led) * 127) / (10 * walk_duration), 4, WHITE);
          display.display();

          // check to see if walk duration is complete
          if (led >= walk_duration) {
            // time is up
            led = 0;
            display.clearDisplay();
            u8display.setCursor(x_offset, 10);
            //u8display.print("Walking: ");
            u8display.print("Running: ");
            u8display.print(led * 10);
            u8display.println(" sec");
            display.drawBitmap(
              ((display.width() - 32 ) / 2) + 45,
              ((display.height() - 32) / 2) + 10,
              myRunner, 32, 32, 1);
            display.display();
            u8display.setCursor(x_offset, 25);
            u8display.print("Total: ");
            u8display.print(cum_led * interval / (60 * 1000));  u8display.println(" min");
            u8display.setCursor(x_offset, 45);
            u8display.println("Time to Run!");
            display.fillRect(0, 60, (10 * (walk_duration - (led)) * 127) / (10 * walk_duration), 4, WHITE);
            display.display();
            //led = 0;
            walktime = false;
            if (toneOn == true) {
              playuptone();
            }
          }
        }
      }
  }
}
