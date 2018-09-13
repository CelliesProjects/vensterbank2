#include <WiFi.h>
#include "SH1106.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>

const char* ssid = "yourSSID";
const char* password = "yourPSK";

const char* host = "192.168.2.3";

float kamerTemp = -1000;

const uint8_t I2C_SDA_PIN  = 21;
const uint8_t I2C_SCL_PIN  = 22;
const uint8_t OLED_ADDRESS = 0x3c;

const uint8_t redPin       = 26;
const uint8_t greenPin     = 18;
const uint8_t bluePin      = 19;

const uint8_t numberOfBits = 16;
const uint64_t frequency   = 1300;

enum channel { redChannel, greenChannel, blueChannel };

const float coldTemp = 15; //only blue light at this temp
const float niceTemp = 20; //only green light at this temp
const float warmTemp = 25; //only red light at this temp

uint32_t PWMdepth;

SH1106 oled( OLED_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN );

Preferences preferences;

void setup( void )
{
  Serial.begin(115200);
  Serial.println();
  preferences.begin( "vensterbank", false );

  oled.init();
  oled.setTextAlignment( TEXT_ALIGN_CENTER );
  oled.drawString( 64, 0, "Connecting WiFi" );
  oled.display();

  //setup pwm
  ledcSetup( redChannel, frequency, numberOfBits );
  ledcSetup( greenChannel, frequency, numberOfBits );
  ledcSetup( blueChannel, frequency, numberOfBits );

  ledcAttachPin( redPin, redChannel);
  ledcAttachPin( greenPin, greenChannel);
  ledcAttachPin( bluePin, blueChannel);

  PWMdepth = ( 0x00000001 << numberOfBits ) - 1;

  if ( preferences.getString( "currentMode", "undefined" ).equals( "undefined" ) )
    preferences.putString( "currentMode", "temp" );

  Serial.printf( "Connecting to %s", ssid );
  WiFi.begin( ssid, password );
  while ( WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println( "connected" );
  Serial.println( WiFi.localIP().toString() );


  if ( preferences.getString( "currentMode", "undefined" ).equals( "color" ) )
  {
    setManualColor( preferences.getString( "color" ) );
  }

  xTaskCreatePinnedToCore(
    tempClient,                     /* Function to implement the task */
    "tempClient",                   /* Name of the task */
    2000,                           /* Stack size in words */
    NULL,                           /* Task input parameter */
    5,                              /* Priority of the task */
    NULL,                           /* Task handle. */
    1);                             /* Core where the task should run */

  xTaskCreatePinnedToCore(
    webServer,                      /* Function to implement the task */
    "webServer",                    /* Name of the task */
    2000,                           /* Stack size in words */
    NULL,                           /* Task input parameter */
    1,                              /* Priority of the task */
    NULL,                           /* Task handle. */
    1);                             /* Core where the task should run */
}

void loop( void )
{
 vTaskDelete( NULL );
}

