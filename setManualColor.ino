static inline __attribute__((always_inline)) void setManualColor( const String hexColorStr )
{
  uint32_t color = strtol( hexColorStr.c_str(), NULL, 16 );
  uint8_t redVal   = ( color & 0xFF0000 ) >> 16;
  uint8_t greenVal = ( color & 0x00FF00 ) >> 8;
  uint8_t blueVal  =   color & 0x0000FF ;
  ledcWrite( redChannel,   map( redVal, 0, 255, 0, PWMdepth ) );
  ledcWrite( greenChannel, map( greenVal, 0, 255, 0, PWMdepth ) );
  ledcWrite( blueChannel,  map( blueVal, 0, 255, 0, PWMdepth ) );

  oled.clear();
  oled.setFont( ArialMT_Plain_10 );
  oled.drawString( 64, 0, WiFi.localIP().toString() );
  oled.drawString( 64, 10, "Manual color mode" );
  char content[50];
  snprintf( content, sizeof( content ), "Red: %i", redVal );
  oled.drawString( 64, 20, content );
  snprintf( content, sizeof( content ), "Green: %i", greenVal );
  oled.drawString( 64, 30, content );
  snprintf( content, sizeof( content ), "Blue: %i", blueVal );
  oled.drawString( 64, 40, content );
  oled.display();
}
