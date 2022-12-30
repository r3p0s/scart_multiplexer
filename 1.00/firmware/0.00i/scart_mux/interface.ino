// #############################################################################
//  ┏┻┻┻┻┻┻┓
//  ┫      ┣ VaDeRetro
//  ┫      ┣ SCART
//  ┫      ┣ Multiplexer
//  ┗┳┳┳┳┳┳┛
//           Interface functions
//
// !!! Please check .h file for required settings and libraries !!!
// #############################################################################

// Set foreground and background text colors
static void setTextColor( bool b )
{
  if ( b )
    display.setTextColor( BLACK, WHITE );
  else
    display.setTextColor( WHITE, BLACK );
}

// Clear & initialize display
static void initDisplay()
{
  display.clearDisplay();
  display.setTextSize( TEXT_SIZE );                 // Normal 1:1 pixel scale
  setTextColor( false );                            // Draw white text on black background
  display.cp437( true );                            // Use full 256 char 'Code Page 437' font
}

// Clear display effect
static void clearDisplay()
{
  for ( int i = 0; i < 128; i += CLEAR_BAR_WIDTH )
  {
    if ( i > 0 ) display.fillRect( i - CLEAR_BAR_WIDTH, 0, CLEAR_BAR_WIDTH, SCREEN_HEIGHT, BLACK );
    display.fillRect( i, 0, CLEAR_BAR_WIDTH, SCREEN_HEIGHT, WHITE );
    display.display();
  }

  // Clear the buffer
  display.clearDisplay();
  display.display();
}

// Project logo
static void logo()
{
  #define LOGO_Y (SCREEN_HEIGHT-4*LINE_HEIGHT)/2

  // Set display parameters
  initDisplay();

  // Print text
  display.setCursor( LOGO_MARGIN_LEFT, LOGO_Y );
  display.println( F( "VaDeRetro!" ) );
  display.setCursor( LOGO_MARGIN_LEFT, LOGO_Y + 1.5*LINE_HEIGHT );
  display.println( PROJECT_NAME );
  display.setCursor( LOGO_MARGIN_LEFT, LOGO_Y + 3*LINE_HEIGHT );
  display.print( PCB_VERSION );
  display.println( FIRMWARE_VERSION );

  // Draw chip logo
  display.drawRoundRect( 4, LOGO_Y + 4, 23, 23, 2, WHITE );

  for ( int i = 5; i < 28; i += 4 )
  {
    display.drawLine( i, LOGO_Y, i, LOGO_Y+2, WHITE );
    display.drawLine( i, LOGO_Y + 28, i, LOGO_Y + 30, WHITE );
    display.drawLine( 0, LOGO_Y + i, 2, LOGO_Y + i, WHITE );
    display.drawLine( 28, LOGO_Y + i, 30, LOGO_Y + i, WHITE );
  }

  // Show the display buffer on the screen
  display.display();
}

#ifdef DEBUG
// Show slot status
void showSlotStatus( bool bOutput )
{
  byte
    bI;
  char
    cBuffer[BUFFER_MAX],
    *cA;

  cA = cBuffer;
  for ( bI = 0; bI < SCART_MUX_SLOTS; bI++, cA++ )
    *cA = aboSlot[bI] + '0';
  *cA = NUL;
  if ( bOutput )
  {
    display.setCursor( 64, TEXT_ROW7 );
    display.print( cBuffer );
    display.print( F( " " ) );
  }
  else
    Serial.print( cBuffer );
}
#endif

// Update display info
static void updateInfo()
{

  // Clear & init display
  initDisplay();

  // Show project name
  setTextColor( true );
  display.setCursor( 0, TEXT_ROW0 );
  display.print( PROJECT_NAME_L );

  // Show active input number
  setTextColor( false );
  display.setCursor( 0, TEXT_ROW2 );
  display.print( F( "Active input:" ) );
  if ( bActiveSlot )
    display.print( bActiveSlot );
  else    
    display.print( F( "N/A" ) );
  
  // Show active input name
  display.setTextColor( boSelection && ( bCLK & 4 ) ? BLACK : WHITE, BLACK );
  display.setCursor( 0, TEXT_ROW3 );
  display.print( acSlotName[boSelection ? bSelectedSlot : bActiveSlot] );

  // Show output mode
  setTextColor( false );
  display.setCursor( 0, TEXT_ROW4 );
  display.print( F( "Output mode:" ) );
  if ( boOutputMode )
    display.print( F( "RGB" ) );
  else    
    display.print( F( "COMPOSITE" ) );
  
  // Show available inputs
  setTextColor( false );
  display.setCursor( 0, TEXT_ROW6 );
  display.print( F( "Available inputs:" ) );
  display.print( bSlots );

  // Show the display buffer on the screen
  display.display();
}
