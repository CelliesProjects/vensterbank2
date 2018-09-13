#include "index_htm.h"

const char* HTML_HEADER = "text/html";

void webServer( void * pvParameters )
{
  static AsyncWebServer server(80);

  server.on( "/", HTTP_GET, [] ( AsyncWebServerRequest * request )
  {
    AsyncWebServerResponse *response = request->beginResponse_P( 200, HTML_HEADER, index_htm, index_htm_len );
    request->send( response );
  });

  server.on( "/api/getbrightness", HTTP_GET, []( AsyncWebServerRequest * request )
  {
    request->send( 200, "text/plain", (String)preferences.getFloat( "brightness", 0.5 ) );
  });

  server.on( "/api/setbrightness", HTTP_GET, []( AsyncWebServerRequest * request )
  {
    if ( !request->hasArg( "brightness" ) )
    {
      return request->send( 400, "text/plain", "Invalid request" );
    }
    float newBrightness = request->arg("brightness").toFloat();
    if ( newBrightness < 0 || newBrightness > 1 )
    {
      return request->send( 400, "text/plain", "Invalid brightness" );
    }
    preferences.putFloat( "brightness", newBrightness );
    if ( preferences.getString( "currentMode" ).equals( "temp" ) ) 
    {
      setTempColor( kamerTemp );
      UpdateTempOnOled();
    }
    request->send( 200, "text/plain", "brightness set" );
  });

  server.on( "/api/setcolor", HTTP_GET, []( AsyncWebServerRequest * request )
  {
    if ( !request->hasArg( "color" ) )
    {
      return request->send( 400, "text/plain", "Invalid request" );
    }
    preferences.putString( "color", request->arg( "color" ) );
    preferences.putString( "currentMode", "color" );
    setManualColor( request->arg( "color") );
    request->send( 200, "text/plain", "color set" );
  });

  server.on( "/api/getcolor", HTTP_GET, []( AsyncWebServerRequest * request )
  {
    request->send( 200, "text/plain", preferences.getString( "color", "undefined" ) );
  });
  
  server.on( "/api/tempmode", HTTP_GET, []( AsyncWebServerRequest * request )
  {
    preferences.putString( "currentMode", "temp" );
    setTempColor( kamerTemp );
    UpdateTempOnOled();
    request->send( 200, "text/plain", (String)kamerTemp );
  });

  server.onNotFound( []( AsyncWebServerRequest * request )
  {
    request->send( 404 , "text/plain", "Not found" );
  });

  DefaultHeaders::Instance().addHeader( "Access-Control-Allow-Origin", "*" );

  server.begin();

  vTaskDelete( NULL );
}


