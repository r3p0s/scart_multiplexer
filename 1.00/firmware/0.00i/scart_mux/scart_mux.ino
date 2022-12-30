// #############################################################################
//  ┏┻┻┻┻┻┻┓
//  ┫      ┣ VaDeRetro
//  ┫      ┣ SCART
//  ┫      ┣ Multiplexer
//  ┗┳┳┳┳┳┳┛
//           Firmware
//
// !!! Please check .h file for required settings and libraries !!!
// #############################################################################

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "scart_mux.h"
#include "mcu.h"
#include "encoder.h"
#include "strings.h"

// Initialization
void setup()
{
  byte
    bI;

  #ifdef DEBUG
  // Set serial port speed
  Serial.begin( SERIAL_BAUD_RATE );
  #endif

  // We all love our MCUs so do not forget to
  // set all pins as INPUTs (High impedance)
  // before doing anything else
  setAllPinsAsInputs();

  // Initialize output ports
  bI = 0;
  while (  bI < SCART_MUX_SLOTS )
  {
    pinMode( abSlotEnablePorts[bI], OUTPUT );
    bI++;
    disableSlot( bI );
  }

  // Set analog reference
  analogReference( EXTERNAL );

  // Get a random seed  
  randomSeed( analogRead( 0 ) );

  // Initialize display voltage from 3.3V internally with the proper screen I2C address
  display.begin( SH1106_SWITCHCAPVCC, SCREEN_ADDRESS );

  // Initialize encoder
  encoderSetup();

}

// Main loop
void loop()
{
  word
    wLastDACnorm = -1;

  // Clear the display buffer
  display.clearDisplay();

  ///////////////////////////////////////////////////////
  // LOGO
  ///////////////////////////////////////////////////////

  // Show logo
  #ifdef SHOW_LOGO
  
  logo();
  delay( LOGO_DELAY );

  // Clear display effect
  clearDisplay();
  #endif

  ///////////////////////////////////////////////////////
  // SCART MUX
  ///////////////////////////////////////////////////////

  while ( true )
  {

    // Update slot status
    updateSlots();

    // Check new selection
    if ( iEncoderCounter != 0 )
      if ( bSlots > 1 && millis() - ulEncoderTime > ENCODER_BOUNCE )
      {
  
        ulEncoderTime = millis();
  
        if ( iEncoderCounter > ENCODER_THRESHOLD )
          bSelectedSlot = findNextSlot( false );
        else if ( iEncoderCounter < ENCODER_THRESHOLD )
          bSelectedSlot = findPreviousSlot();
  
        iEncoderCounter = 0;
        wLastDACnorm = -1;
        boSelection = bSelectedSlot;
  
      }
      else
        iEncoderCounter = 0;

    // Update info on display if there were any changes
    if ( wDACnorm != wLastDACnorm || boSelection )
    {

      #ifdef DEBUG
      Serial.print( F( "ML: " ) );
      Serial.print( wDACnorm );
      Serial.print( F( " " ) );
      Serial.print( wLastDACnorm );
      Serial.print( F( " " ) );
      Serial.print( bActiveSlot );
      Serial.print( F( " " ) );
      Serial.print( bSelectedSlot );
      Serial.print( F( " " ) );
      Serial.print( boSelection );
      Serial.print( F( " " ) );
      showSlotStatus( false );
      Serial.println();    
      #endif   

      updateInfo();
      wLastDACnorm = wDACnorm;
    }

    // Check encoder switch
    if ( boSelection && !digitalRead( ENCODER_SW_PIN ) )
    {
      boSelection = false;
      wLastDACnorm = -1;
      disableSlot( bActiveSlot );
      bActiveSlot = bSelectedSlot;
      enableSlot( bActiveSlot );
    }

    #ifdef DEBUG

    display.setCursor( 0, TEXT_ROW7 );
    display.print( F( "A" ) );
    display.print( bActiveSlot );
    display.print( F( " " ) );

    display.setCursor( 16, TEXT_ROW7 );
    display.print( F( "S" ) );
    display.print( bSelectedSlot );
    display.print( F( " " ) );

    display.setCursor( 32, TEXT_ROW7 );
    display.print( F( "M" ) );
    display.print( boSelection );
    display.print( F( " " ) );

    display.setCursor( 48, TEXT_ROW7 );
    display.print( F( "C" ) );
    display.print( iEncoderCounter );
    display.print( F( " " ) );

    showSlotStatus( true );

    display.display();

    #endif

    bCLK++;
    
    delay( LOOP_DELAY );

  }

}
