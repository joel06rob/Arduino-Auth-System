#include <Arduino_RouterBridge.h>
#include "authcode_numbers.h"
#include "animation_frames.h"
#include <Arduino_LED_Matrix.h>

//Display States
//
enum DisplayState {
  IDLE, //0
  SHOWING_CODE, //1
  UNLOCKED //2
  
};
DisplayState currentState;


//LED Matrix Buffer
//
ArduinoLEDMatrix matrix;
uint8_t frame[8][13];

//Initialize code
//
int currentCode = 0;


void setup() {

  //Initialisations
  Monitor.begin();
  
  Bridge.begin();

  matrix.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //Setup bridge connection functions (Python callable)
  Bridge.provide("processAuthCode", processAuthCode);
  Bridge.provide("unlockAuth", unlockAuth);
  Bridge.provide("getDisplayState", getDisplayState);
  Bridge.provide("setIdleState", setIdleState);

  setIdleState(false);

  updateDisplay();
}

void loop() {
  
}

//==================
//Display functions
//

//Update the LED Matrix display depending on current state
void updateDisplay()
{
  switch(currentState)
  {
    case IDLE:
      matrix.renderBitmap(idle_logo, 8, 13);
      break;

    case SHOWING_CODE:
      showAuthCode(currentCode);
      break;

    case UNLOCKED:
      matrix.renderBitmap(unlocked_logo, 8, 13);
      break;
  }
}

//Display auth codes onto display - split digits seperately, then map to the matrix
void showAuthCode(int code)
{
  memset(frame, 0, sizeof(frame));

  int firstnum  = code / 100;
  int secondnum = (code / 10) % 10;
  int thirdnum  = code % 10;

  drawDigit(firstnum, 1);
  drawDigit(secondnum, 5);
  drawDigit(thirdnum, 9);

  matrix.renderBitmap(frame, 8, 13);
}


//Draw each digit in LED matrix buffer
//
void drawDigit(int digit, int startCol){

  if(digit < 0 || digit > 9){
    return;
  }
  
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 3; c++) {

      //Spacing for readibility
      frame[r + 1][c + startCol] = digits[digit][r][c];
    }
  }
}



//==================
//Bridge functions
//

//Set state to SHOWING_CODE for matrix to display received code
void processAuthCode(int code){

  Monitor.print("Received Code: ");
  Monitor.println(code);

  currentCode = code;
  currentState = SHOWING_CODE;

  updateDisplay();
  
}

//Set state to UNLOCKED for access
void unlockAuth()
{
  currentState = UNLOCKED;
  currentCode = 0;

  updateDisplay();
}

//Set state to IDLE at beginning, if error is called, display warning LED
void setIdleState(bool isError)
{
  if(isError){
    digitalWrite(LED_BUILTIN, LOW);
  }
  currentState = IDLE;
  updateDisplay();
}

int getDisplayState()
{
  return currentState;
}