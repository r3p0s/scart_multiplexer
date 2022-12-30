// #############################################################################
//  ┏┻┻┻┻┻┻┓
//  ┫      ┣ VaDeRetro
//  ┫      ┣ SCART
//  ┫      ┣ Multiplexer
//  ┗┳┳┳┳┳┳┛
//           MCU functions headers
//
// #############################################################################

// Set all pins as INPUTS
void setAllPinsAsInputs();

// Activate slot
void enableSlot( byte bSlot );

// Deactivate slot
void disableSlot( byte bSlot );

// Find previous available slot
byte findPreviousSlot();

// Find next available slot
static byte findNextSlot( bool boAbsoluteFirst );

// Activate first available slot
void enableFirstAvailableSlot();

// Update slot status
void updateSlots();
