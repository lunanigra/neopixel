/*
 * Toilet_Lights.ino
 * Example showing 8x8 NeoPixel matrix display
 *
 * This sketch uses the ESP8266WiFi library
 */

#include <FS.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <Ticker.h>

#define PIN D8

struct RGB {
  int8_t r;
  int8_t g;
  int8_t b;
};

struct IMAGE {
  uint64_t image;
  RGB color;
};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800);

WiFiClient wifiClient;

const char *AP_SSID = "tbd.";
const char *AP_PASS = "tbd.";

Ticker ticker;
boolean tickState = LOW;

void tick() {
  digitalWrite(BUILTIN_LED, tickState);
  tickState = (tickState == LOW) ? HIGH : LOW;
}

// flag for saving data
bool shouldSaveConfig = false;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);

  Serial.begin(115200);

  pixels.begin(); // This initializes the NeoPixel library.

  ticker.attach(0.75, tick);

  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  Serial.print("Connecting to WiFi...");
  // begin WiFi joining with provided Access Point name and password
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());

  Serial.println("\r\n");
  Serial.print("Chip ID: 0x");
  Serial.println(ESP.getChipId(), HEX);

  // set hostname
  String hostname("ESP8266-OTA-");
  hostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostname);

  // starting OTA
  ArduinoOTA.setHostname((const char *) hostname.c_str());
  ArduinoOTA.begin();
  Serial.println("OTA started...");

  ticker.detach();
}

const uint64_t IMAGES_DIGITIS[] = {
  0x003f0c0c0c0c0e0c, 0x003f33061c30331e, 0x001e33301c30331e, 0x0078307f33363c38, 0x001e3330301f033f, 0x001e33331f03061c, 0x000c0c0c1830333f, 0x001e33331e33331e,
  0x000e18303e33331e, 0x003e676f7b73633e
};

const uint64_t IMAGES_LETTERS[] = {
  0x0033333f33331e0c, 0x003f66663e66663f, 0x003c66030303663c, 0x001f36666666361f, 0x007f46161e16467f, 0x000f06161e16467f, 0x007c66730303663c, 0x003333333f333333,
  0x001e0c0c0c0c0c1e, 0x001e333330303078, 0x006766361e366667, 0x007f66460606060f, 0x0063636b7f7f7763, 0x006363737b6f6763, 0x001c36636363361c, 0x000f06063e66663f,
  0x00381e3b3333331e, 0x006766363e66663f, 0x001e33380e07331e, 0x001e0c0c0c0c2d3f, 0x003f333333333333, 0x000c1e3333333333, 0x0063777f6b636363, 0x0063361c1c366363,
  0x001e0c0c1e333333, 0x007f664c1831637f, 0x0000000000000000, 0x006e333e301e0000, 0x003b66663e060607, 0x001e3303331e0000, 0x006e33333e303038, 0x001e033f331e0000,
  0x000f06060f06361c, 0x1f303e33336e0000, 0x006766666e360607, 0x001e0c0c0c0e000c, 0x1e33333030300030, 0x0067361e36660607, 0x001e0c0c0c0c0c0e, 0x00636b7f7f330000,
  0x00333333331f0000, 0x001e3333331e0000, 0x0f063e66663b0000, 0x78303e33336e0000, 0x000f06666e3b0000, 0x001f301e033e0000, 0x00182c0c0c3e0c08, 0x006e333333330000,
  0x000c1e3333330000, 0x00367f7f6b630000, 0x0063361c36630000, 0x1f303e3333330000, 0x003f260c193f0000
};

const uint64_t IMAGES_SIGNS[] = {
  0x00000c0c3f0c0c00, 0x000000003f000000, 0x0000663cff3c6600, 0x000103060c183060, 0x0063660c18336300, 0x00003f00003f0000, 0x0000000000003b6e, 0x0000000063361c08,
  0x00180c0603060c18, 0x00060c1830180c06, 0x00180c0606060c18, 0x00060c1818180c06, 0x001e06060606061e, 0x001e18181818181e, 0x00380c0c070c0c38, 0x00070c0c380c0c07,
  0x000c0c0000000000, 0x000c0c00000c0c00, 0x060c0c00000c0c00, 0x060c0c0000000000, 0x00180018183c3c18, 0x000c000c1830331e, 0x001e037b7b7b633e, 0x006e333b6e1c361c,
  0x000c1f301e033e0c, 0x0036367f367f3636, 0x00406030180c0603, 0x0000000000180c0c, 0x0000000000030606
};

const uint64_t IMAGES_OTHERS[] = {
  0x18187e03037e1818, 0x003f67060f26361c, 0x0c0c3f0c3f1e3333, 0x1e331c36361cc67c, 0x3c4299858599423c, 0x0000cc663366cc00, 0x3c42a59da59d423c, 0x007e0018187e1818,
  0x000000001c36361c, 0x000000001c080c08, 0x0000003c0c18301c, 0x0000001c3018301c, 0x03063e6666660000, 0x00d8d8d8dedbdbfe, 0x00003366cc663300, 0x03f3f6ecbd3363c3,
  0xf03366cc7b3363c3, 0x80e6acdbb463c403, 0x001e3303060c000c, 0x00637f63361c0007, 0x00637f63361c0070, 0x00637f633e00361c, 0x00637f633e003b6e, 0x0063637f63361c63,
  0x00333f331e000c0c, 0x007333337f33367c, 0x1e30181e3303331e, 0x003f061e063f0007, 0x003f061e063f0038, 0x003f061e063f120c, 0x003f061e063f0036, 0x001e0c0c0c1e0007,
  0x001e0c0c0c1e0038, 0x001e0c0c1e00120c, 0x001e0c0c0c1e0033, 0x003f66666f6f663f, 0x00333b3f3733003f, 0x00183c663c18000e, 0x00183c663c180070, 0x00183c663c18663c,
  0x003e63633e003b6e, 0x00183c66663c18c3, 0x0000361c081c3600, 0x001d366f7b73365c, 0x003c66666666000e, 0x003c666666660070, 0x003c66666600663c, 0x001e333333330033,
  0x0018183c66660070, 0x0f063e66663e060f, 0x03031f331f331e00, 0x007e333e301e0007, 0x007e333e301e0038, 0x00fc667c603cc37e, 0x007e333e301e3b6e, 0x007e333e301e0033,
  0x007e333e301e0c0c, 0x00fe33fe30fe0000, 0x1c301e03031e0000, 0x001e033f331e0007, 0x001e033f331e0038, 0x003c067e663cc37e, 0x001e033f331e0033, 0x001e0c0c0c0e0007,
  0x001e0c0c0c0e001c, 0x003c1818181c633e, 0x001e0c0c0c0e0033, 0x001e333e301b0e1b, 0x003333331f001f00, 0x001e33331e000700, 0x001e33331e003800, 0x001e33331e00331e,
  0x001e33331e003b6e, 0x001e33331e003300, 0x063c6e7e763c6000, 0x007e333333000700, 0x007e333333003800, 0x007e33333300331e, 0x007e333333003300, 0x1f303e3333003800,
  0x00063e663e060000, 0x1f303e3333003300
};

const uint64_t IMAGE_HEART = 0x00183c7effffff66;

const uint64_t IMAGES_TEXT[] = {
  0x00, IMAGES_LETTERS[12], IMAGES_OTHERS[23], IMAGES_LETTERS[3], IMAGES_LETTERS[4], IMAGES_LETTERS[11], IMAGES_LETTERS[18], 0x00, IMAGE_HEART, 0x00, 0x00
};

const IMAGE IMAGE_TICKER[] = {
  {0x00, {0x00, 0x00, 0x00}},
  {IMAGES_LETTERS[12], {0xff, 0x08, 0x7f}},
  {IMAGES_OTHERS[23], {0xff, 0x08, 0x7f}},
  {IMAGES_LETTERS[3], {0xff, 0x08, 0x7f}},
  {IMAGES_LETTERS[4], {0xff, 0x08, 0x7f}},
  {IMAGES_LETTERS[11], {0xff, 0x08, 0x7f}},
  {IMAGES_LETTERS[18], {0xff, 0x08, 0x7f}},
  {0x00, {0x00, 0x00, 0x00}},
  {IMAGE_HEART, {0xff, 0x00, 0x00}},
  {0x00, {0x00, 0x00, 0x00}},
  {0x00, {0x00, 0x00, 0x00}}
};

unsigned long statusTime = 0;

void showImage(IMAGE image, int8_t offset) {
  for (uint8_t y = 0; y < 8; y++) {
    unsigned long row = image.image >> y * 8 & 0xff;
    for (uint8_t x = 0; x < 8; x++) {
      uint64_t pixel = row >> x & 0x01;
      if (x - offset >= 0 && x - offset < 8) {
        pixels.setPixelColor(y * 8 + x - offset, pixel * image.color.r, pixel * image.color.g, pixel * image.color.b);
      }
    }
  }
}

void loop() {
  for (uint8_t i = 0; i < (sizeof(IMAGE_TICKER) / sizeof(IMAGE) - 1) * 8; i++) {
    IMAGE image1 = IMAGE_TICKER[i / 8];
    IMAGE image2 = IMAGE_TICKER[i / 8 + 1];
    int8_t offset = i % 8;

    showImage(image1, offset);
    showImage(image2, -(8 - offset));
    pixels.setBrightness(0x80);
    pixels.show();

    for (uint8_t y = 0; y < 20; y++) {
      // handle OTA server.
      ArduinoOTA.handle();
      yield();

      delay(10);
    }
  }
}

