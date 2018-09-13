void setTempColor( const float kamerTemp )
{
  ledcWrite( redChannel, redValue( kamerTemp ) * preferences.getFloat( "brightness", 0.5 ) );
  ledcWrite( greenChannel, greenValue( kamerTemp ) * preferences.getFloat( "brightness", 0.5 ) );
  ledcWrite( blueChannel, blueValue( kamerTemp ) * preferences.getFloat( "brightness", 0.5 ) );
}

static inline __attribute__((always_inline)) int blueValue( const float temp ) {
  if ( temp <= coldTemp ) return PWMdepth;
  if ( temp >= niceTemp ) return 0;
  return mapFloat( temp, coldTemp, niceTemp, PWMdepth, 0 );
}

static inline __attribute__((always_inline)) int greenValue( const float temp ) {
  if ( temp <= coldTemp || temp >= warmTemp ) return 0;
  if ( temp > coldTemp && temp < niceTemp ) return mapFloat( temp, coldTemp, niceTemp, 0, PWMdepth );
  return mapFloat( temp, niceTemp, warmTemp, PWMdepth , 0 );
}

static inline __attribute__((always_inline)) int redValue( const float temp ) {
  if ( temp <= niceTemp ) return 0;
  if ( temp >= warmTemp ) return PWMdepth;
  return mapFloat( temp, niceTemp, warmTemp, 0, PWMdepth );
}

static inline __attribute__((always_inline)) float mapFloat( const float x, const float in_min, const float in_max, const float out_min, const float out_max)
{
  return ( x - in_min ) * ( out_max - out_min ) / ( in_max - in_min ) + out_min;
}

