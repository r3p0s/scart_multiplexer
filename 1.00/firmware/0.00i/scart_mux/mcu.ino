// #############################################################################
//  ┏┻┻┻┻┻┻┓
//  ┫      ┣ VaDeRetro
//  ┫      ┣ SCART
//  ┫      ┣ Multiplexer
//  ┗┳┳┳┳┳┳┛
//           MCU functions
//
// !!! Please check .h file for required settings and libraries !!!
// #############################################################################

// Set all pins as INPUTS
static void setAllPinsAsInputs()
{

  // Set all pin as INPUTs
  DDRB = DDRC = DDRD = 0;

}

// Normalyze slot number
static byte normalyzeSlot( byte bSlot )
{
  if ( !bSlot )
    bSlot = 1;
  else if ( bSlot > SCART_MUX_SLOTS )
    bSlot = SCART_MUX_SLOTS;
  return bSlot;
}

// Activate slot
static void enableSlot( byte bSlot )
{
   if ( bSlot && aboSlot[bSlot-1] )
    digitalWrite( abSlotEnablePorts[bSlot-1], HIGH );
}

// Deactivate slot
static void disableSlot( byte bSlot )
{
  if ( bSlot )
    digitalWrite( abSlotEnablePorts[bSlot-1], LOW );
}

// Find previous available slot
static byte findPreviousSlot()
{
  byte
    bSelSlot, bLastSlot, bSlot;

  bSlot = bLastSlot = normalyzeSlot( bSelectedSlot ? bSelectedSlot : bActiveSlot );

  bSelSlot = 0;
  while ( !bSelSlot && bSlot > 1 )
  {

    #ifdef DEBUG
    Serial.print( F( "FPS: " ) );
    Serial.print( bSelSlot );
    Serial.print( F( " " ) );
    Serial.print( bSlot );
    Serial.println( F( " " ) );
    #endif

    bSlot--;
    if ( aboSlot[bSlot-1] )
      bSelSlot = bSlot;
  }

  if ( !bSelSlot ) bSelSlot =  bLastSlot;

  #ifdef DEBUG
  Serial.print( F( "FPSR: " ) );
  Serial.println( bSelSlot );
  #endif

  return bSelSlot;
}

// Find next available slot
static byte findNextSlot( bool boAbsoluteFirst )
{
  byte
    bSelSlot, bLastSlot, bSlot;

  bSlot = bLastSlot = normalyzeSlot( bSelectedSlot ? bSelectedSlot : bActiveSlot );
  if ( boAbsoluteFirst ) bSlot = 0;

  bSelSlot = 0;
  while ( !bSelSlot && bSlot < SCART_MUX_SLOTS )
  {

    #ifdef DEBUG
    Serial.print( F( "FNS: " ) );
    Serial.print( bSelSlot );
    Serial.print( F( " " ) );
    Serial.print( bSlot );
    Serial.println( F( " " ) );
    #endif

    bSlot++;
    if ( aboSlot[bSlot-1] )
      bSelSlot = bSlot;
  }

  if ( !bSelSlot ) bSelSlot =  bLastSlot;

  #ifdef DEBUG
  Serial.print( F( "FNSR: " ) );
  Serial.println( bSelSlot );
  #endif

  return bSelSlot;
}

// Activate first available slot
static void enableFirstAvailableSlot()
{
  byte
    bSlot;

  // Disable previous enabled slot
  if ( bActiveSlot )
    disableSlot( bActiveSlot );

  bActiveSlot = bSelectedSlot = 0;
  bSlot = findNextSlot( true );

  #ifdef DEBUG
  Serial.print( F( "AFAS: " ) );
  Serial.println( bSlot );
  #endif

  if ( bSlot )
  {
    bActiveSlot = bSelectedSlot = bSlot;
    enableSlot( bActiveSlot );
  }

}

// Update slot status
static void updateSlots()
{
  byte
    bBit,
    bDAC = 0,
    bDACport,
    bLastSlots = bSlots,
    bResult,
    bSlot = 1;
  int
    iDACread, iDACvalue;
  word
    wP;

  // Reset number of available slots
  bSlots = 0;

  // Check RGB mode
  boOutputMode = digitalRead( RGB_PIN );

  // Reset last normalyzed DAC reads
  wDACnorm = boOutputMode;

  while ( bDAC < DACS && bSlot <= SCART_MUX_SLOTS )
  {

    wP = awPow2[DAC_SHIFT+DAC_REVERSE*bDAC];

    // DAC sampling
    iDACvalue = 1024;
    for ( bBit = 0; bBit < DAC_SAMPLES; bBit++ )
    {
      iDACread = analogRead( abDACPorts[bDAC] );

      // Equalize DAC read
      if ( iDACread < wP - DAC_THRESHOLD )
        iDACread = 0;
      else
      {
        iDACread += DAC_THRESHOLD;
        if ( iDACread > 1023 ) iDACread = 1023;
      }

      // Get minimum
      if ( iDACread < iDACvalue ) iDACvalue = iDACread;
      delay( DAC_SAMPLING_DELAY );
    }

    #ifdef DEBUG
    Serial.print( " MCU0: " );
    Serial.print( abDACPorts[bDAC] );
    Serial.print( " " );
    Serial.print( iDACvalue );
    Serial.print( " " );
    #endif

    // Update slot status
    bBit = 0;
    while ( bBit < DAC_BITS && bSlot <= SCART_MUX_SLOTS )
    {
      bResult = aboSlot[abDACTranslationTable[bSlot-1]] = ( iDACvalue & wP ) == wP;

      #ifdef DEBUG
      Serial.print( bResult );
      #endif

      if ( bResult )
      {
        // Acknowledge slot change
        wDACnorm |= wP;

        // Increase available slots number
        bSlots++;
      }

      wP <<= 1;
      bSlot++;
      bBit++;
    }

    // Next DAC
    bDAC++;

  }

  #ifdef DEBUG
  Serial.print( " MCU1: " );
  Serial.print( bActiveSlot );
  Serial.print( " " );
  Serial.print( bSlots );
  Serial.print( " " );
  #endif

  // No available input
  if ( !bSlots )
  {
    disableSlot( bActiveSlot );
    boSelection = false;
    bActiveSlot = bSelectedSlot = 0;
  }
  else
    if ( bSlots != bLastSlots )
      if (
                ( bActiveSlot && !aboSlot[bActiveSlot-1] )
                ||
                ( !bActiveSlot && bSlots >= 1 )
              )
      {
        #ifdef DEBUG
        Serial.print( aboSlot[bActiveSlot-1] );
        Serial.print( " CAFAS-" );
        #endif
        enableFirstAvailableSlot();
      }

  #ifdef DEBUG
  Serial.println();
  #endif

}
