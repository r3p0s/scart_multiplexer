// #############################################################################
//  ┏┻┻┻┻┻┻┓
//  ┫      ┣ VaDeRetro
//  ┫      ┣ SCART
//  ┫      ┣ Multiplexer
//  ┗┳┳┳┳┳┳┛
//           Encoder functions headers
//
// #############################################################################

#ifndef ENCODER_HEADERS

#define ENCODER_HEADERS

int
  iEncoderCounter = 0;
unsigned long
  ulEncoderCLK,
  ulEncoderTime = 0,
  ulLastEncoderCLK;

// Initialize encoder pins & interrupts
void encoderSetup();

#endif
