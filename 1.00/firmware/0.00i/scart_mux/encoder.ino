// #############################################################################
//  ┏┻┻┻┻┻┻┓
//  ┫      ┣ VaDeRetro
//  ┫      ┣ SCART
//  ┫      ┣ Multiplexer
//  ┗┳┳┳┳┳┳┛
//           Encoder functions
//
// !!! Please check .h file for required settings and libraries !!!
// #############################################################################

static void updateEncoderCounter(){
  
  // Read CLK
  ulEncoderCLK = digitalRead( ENCODER_CLK_PIN );

  if ( ulEncoderCLK != ulLastEncoderCLK)
  {

    #ifdef ENCODER_REVERSE
    if ( digitalRead( ENCODER_DT_PIN ) == ulEncoderCLK )
    #else
    if ( digitalRead( ENCODER_DT_PIN ) != ulEncoderCLK )
    #endif
      // Rotating counter-clockwise
      iEncoderCounter--;
    else
      // Rotating clockwise
      iEncoderCounter++;

  }

  // Save CLK
  ulLastEncoderCLK = ulEncoderCLK;
}

// Initialize encoder pins & interrupts
static void encoderSetup()
{
  // Set encoder pins as inputs
  pinMode( ENCODER_CLK_PIN, INPUT );
  pinMode( ENCODER_DT_PIN, INPUT );
  pinMode( ENCODER_SW_PIN, INPUT_PULLUP );

  // Save CLK
  ulLastEncoderCLK = digitalRead( ENCODER_CLK_PIN );

  // Attach update counter function to interrupts 0 & interrupt 1
  attachInterrupt(0, updateEncoderCounter, CHANGE);
  attachInterrupt(1, updateEncoderCounter, CHANGE);  
}
