// #############################################################################
//  ┏┻┻┻┻┻┻┓
//  ┫      ┣ VaDeRetro
//  ┫      ┣ SCART
//  ┫      ┣ Multiplexer
//  ┗┳┳┳┳┳┳┛
//           Firmware headers
//
// Requirements:
// - New bootloader (optiboot)
// - Download, add & install Adafruit GFX and Adafruit SH1106 libraries
// - Copy provided gldcfont.c to @ arduino_libraries_directory/Adafruit-GFX-library
// - Create and/or edit platform.local.txt (@ arduino_installation_directory/hardware/arduino/avr/)
// - Add build.extra_flags=-DALT_CHARSET to platform.local.txt
// #############################################################################

#ifndef SCART_MUX_HEADERS

#define SCART_MUX_HEADERS                           // Mark .h file as already loaded

// #############################################################################
// Main constants
// #############################################################################

#define SHOW_LOGO                                   // Show logo
#undef  DEBUG                                       // Debug mode (serial output)
#define SERIAL_BAUD_RATE                     115200 // Serial baud rate (default 115200)

// #############################################################################
// Misc. constants
// #############################################################################

#define FIRMWARE_VERSION            F( " fw0.00i" ) // Firmware version
#define PCB_VERSION                     F( "1.00" ) // PCB version
#define PROJECT_NAME           F( "6ch SCART MUX" ) // Project name
#define PROJECT_NAME_L F( " 6-channel SCART MUX " ) // Project name (long)

#define CHARACTERS_PER_LINE                      21 // Characters per display line
#define BUFFER_MAX            CHARACTERS_PER_LINE+1 // Characters per display line + 1

#define NUL                                   '\x0' // NULL char

#define LOOP_DELAY                                2 // Main loop delay (ms)

// #############################################################################
// Display settings
// #############################################################################
#define LINE_HEIGHT                               8 // OLED display line height, in pixels
#define SCREEN_WIDTH                            128 // OLED display width, in pixels
#define SCREEN_HEIGHT                            64 // OLED display height, in pixels
#define SCREEN_LINES      SCREEN_HEIGHT/LINE_HEIGHT // OLED display height, in lines

#define SCREEN_ROW(r)       r*TEXT_SIZE*LINE_HEIGHT // Row top pixel calculator

#define CHARACTER_WIDTH                           6 // Character width
#define CHARACTER_HEIGHT              SCREEN_ROW(1) // Character height
#define CLEAR_BAR_WIDTH             CHARACTER_WIDTH // Clear bar width
#define TEXT_SIZE                                 1 // Text size (1:1)
#define TEXT_ROW0                     SCREEN_ROW(0) // Text row #0
#define TEXT_ROW1                     SCREEN_ROW(1) // Text row #1
#define TEXT_ROW2                     SCREEN_ROW(2) // Text row #2
#define TEXT_ROW3                     SCREEN_ROW(3) // Text row #3
#define TEXT_ROW4                     SCREEN_ROW(4) // Text row #4
#define TEXT_ROW5                     SCREEN_ROW(5) // Text row #5
#define TEXT_ROW6                     SCREEN_ROW(6) // Text row #6
#define TEXT_ROW7                     SCREEN_ROW(7) // Text row #7

#define MARGIN_LEFT                              15 // Default left margin for IC names
#define LOGO_DELAY                             2000 // Logo screen delay (ms)
#define LOGO_MARGIN_LEFT                         48 // PCB + Firmware version = 8 chars

#define OLED_RESET                               -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS                         0x3C // See datasheet for address; 0x3D for 128x64, 0x3C for 128x32

#ifdef SH1106_I2C_ADDRESS
Adafruit_SH1106 display(OLED_RESET);
#endif

// #############################################################################
// Encoder constants
// #############################################################################

#define ENCODER_CLK_PIN                           2 // Encoder CLK pin
#define ENCODER_DT_PIN                            3 // Encoder DT pin
#define ENCODER_SW_PIN                            4 // Encoder SW pin
#undef  ENCODER_REVERSE                             // Reverse encoder rotation
#define ENCODER_THRESHOLD                         0 // Encoder sensitivity [0-...] (0=Max.sensitivity)
#define ENCODER_BOUNCE                          200 // Encoder anti-bounce (ms)

// #############################################################################
// MUX constants & variables
// #############################################################################

#define SCART_MUX_SLOTS                           6 // Number of input slots
#define DACS                                      2 // Number of DACs
#define DAC_BITS                                  4 // Bits per DAC
#define DAC_REVERSE                               2 // DAC D0-D3 are in reverse order (default 2 no reverse 0)
#define DAC_SAMPLES                               5 // Number of DAC samples
#define DAC_SAMPLING_DELAY                        5 // DAC sampling delay (ms)
#define DAC_SHIFT                                 6 // DAC bit shift
#define DAC_THRESHOLD                            24 // DAC threshold
#define RGB_PIN                                  17 // RGB mode digital pin

bool
  boSelection = false,                              // Selection mode
  boOutputMode = false;                             // SCART-OUT default mode

byte
  bActiveSlot = 0,                                  // Active slot number
  bSelectedSlot = 0,                                // Selected slot number
  bSlots = 0,                                       // Number of available slots
  bCLK;                                             // Main loop clock

word
  wDACnorm;                                         // DAC normalyzed value

bool
  aboSlot[SCART_MUX_SLOTS];                         // Slot status

const byte
  abSlotEnablePorts[SCART_MUX_SLOTS] = {            // MCU slot enable digital ports
    5, 6, 7, 8, 9, 10
  },
  abDACPorts[DACS] = {                              // MCU DAC analog ports
    A0, A1
  };

const byte                                          // DAC inputs order
  abDACTranslationTable[SCART_MUX_SLOTS] = {        // index=slot#-1 value=assigned slot#-1
    3, 2, 1, 0, 5, 4,                               // Reordered DAC#1-D3=Input#1 DAC#2-D3=Input#5
    // 0, 1, 2, 3, 4, 5,                              // Regular DAC#1-D0=Input#1 DAC#2-D0=Input#5
  };

const word
  awPow2[10] = {                                    // Powers of 2 for 10-bit ADC decoding
    1, 2, 4, 8, 16, 32, 64, 128, 256, 512
  };


#endif
