void tempClient( void * pvParameters  )
{
  while ( true )
  {
    WiFiClient client;
    if ( client.connect( host, 80 ) )
    {
      client.print(String("GET /api/getdevice?status=") + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n" +
                   "\r\n"
                  );

      while ( client.connected() )
      {
        if ( client.available() )
        {
          String line = client.readStringUntil( '\n' );
          if ( line.startsWith( "kamer" ) )
          {
            String temp = line.substring( line.indexOf( "," ) + 1 );
            kamerTemp = temp.toFloat();
            client.stop();            
          }
        }
      }
      if ( client ) client.stop();
    }
    else
    {
      Serial.println( "connection failed!]" );
      client.stop();
    }
    static float lastTemp;
    if ( preferences.getString( "currentMode", "undefined" ).equals( "temp" ) && lastTemp != kamerTemp )
    {
      setTempColor( kamerTemp );
      lastTemp = kamerTemp;
      UpdateTempOnOled();
    }
    delay( 5000 );
  }
}

void UpdateTempOnOled()
{
  oled.clear();
  oled.drawString( 64, 0, WiFi.localIP().toString() );
  if ( kamerTemp != -1000 )
  {
    char buff[10];
    snprintf( buff, sizeof( buff ), "%.1f°", kamerTemp );
    oled.setFont( ArialMT_Plain_24 );
    oled.drawString( 64, 20, buff );
    oled.setFont( ArialMT_Plain_10 );
    oled.drawString( 64, 50, (String)preferences.getFloat( "brightness", 0.5 ) );
  }      
  oled.display();
}

