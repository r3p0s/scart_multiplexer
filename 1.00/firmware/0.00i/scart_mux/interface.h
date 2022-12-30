// #############################################################################
//  ┏┻┻┻┻┻┻┓
//  ┫      ┣ VaDeRetro
//  ┫      ┣ SCART
//  ┫      ┣ Multiplexer
//  ┗┳┳┳┳┳┳┛
//           Interface functions headers
//
// !!! Please check .h file for required settings and libraries !!!
// #############################################################################

#ifndef INTERFACE_HEADERS

#define INTERFACE_HEADERS

// Set foreground and background text colors
void setTextColor( bool b );

// Clear & initialize display
void initDisplay();

// Clear display effect
void clearDisplay();

// Project logo
void logo();

// Update display info
void updateInfo();

#endif
