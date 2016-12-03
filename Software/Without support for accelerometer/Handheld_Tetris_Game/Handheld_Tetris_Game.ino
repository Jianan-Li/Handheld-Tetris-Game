#include <SPI.h>
#include <digitalWriteFast.h>

# define DISPLAY_8_BIT 0

// Color Designation 
// 100 (4) - Red; 010 (2) - Green;  001 (1) - Blue; 110 (6) - Yellow; 011 (3) - Cyan; 101 (5) - Purple; 111 (7) - White 

//  Pins Definition
//// Source and Sink Drivers
#define latchPin        8  //MIC5891 & STP16DP05 LATCH
#define dataPin        11  //MIC5891 & STP16DP05 DATA  (USES SPI; DO NOT CHANGE THIS PIN)
#define clockPin       13  //MIC5891 & STP16DP05 CLOCK (USES SPI; DO NOT CHANGE THIS PIN)

//// Buttons
#define upButton        2
#define leftButton      3
#define rightButton     4
#define downButton      5
#define dropButton     14
#define pauseButton    15
#define restartButton  16

//// Others
#define barGraphLeft    9 // PWM
#define barGraphRight   6 // PWM
#define enableAccel     7
#define motor          17

#define testPin        19  
#define randomPin      18
////I2C used by the accelerometer
////SDA -- 18
////SCL -- 19

//// Using TX (pin 1) to talk to the ATtiny85
//// Start/Restart playing: 10
//// Pause: 11
//// Resume: 12
#define musicRestart 10
#define musicPause   11
#define musicResume  12

//  Matrix Definition
#define cols         8
#define rows        16
#define leftLimit    0
#define rightLimit   7
#define topLimit     0
#define bottomLimit 15

//Tetris pieces: these piece matrices are represented by their corresponding colors (1-7)
// Color Designation 
// 100 (4) - Red; 010 (2) - Green;  001 (1) - Blue; 110 (6) - Yellow; 011 (3) - Cyan; 101 (5) - Purple; 111 (7) - White
#define white { \
  {24, 0, 0, 0, 0, 0, 0, 0}, \
  {24,24, 0, 0, 0, 0, 0, 0}, \
  {24,24,24, 0, 0, 0, 0, 0}, \
  {24,24,24,24, 0, 0, 0, 0}, \
  {24,24,24,24,24, 0, 0, 0}, \
  {24,24,24,24,24,24, 0, 0}, \
  {24,24,24,24,24,24,24, 0}, \
  {24,24,24,24,24,24,24,24}, \
  {24,24,24,24,24,24,24,24}, \
  {24,24,24,24,24,24,24, 0}, \
  {24,24,24,24,24,24, 0, 0}, \
  {24,24,24,24,24, 0, 0, 0}, \
  {24,24,24,24, 0, 0, 0, 0}, \
  {24,24,24, 0, 0, 0, 0, 0}, \
  {24,24, 0, 0, 0, 0, 0, 0}, \
  {24, 0, 0, 0, 0, 0, 0, 0}, \
}

#define spectrum { \
  { 1, 2, 3, 4, 5, 6, 7, 8}, \
  { 2, 3, 4, 5, 6, 7, 8, 9}, \
  { 3, 4, 5, 6, 7, 8, 9,10}, \
  { 4, 5, 6, 7, 8, 9,10,11}, \
  { 5, 6, 7, 8, 9,10,11,12}, \
  { 6, 7, 8, 9,10,11,12,13}, \
  { 7, 8, 9,10,11,12,13,14}, \
  { 8, 9,10,11,12,13,14,15}, \
  { 9,10,11,12,13,14,15,16}, \
  {10,11,12,13,14,15,16,17}, \
  {11,12,13,14,15,16,17,18}, \
  {12,13,14,15,16,17,18,19}, \
  {13,14,15,16,17,18,19,20}, \
  {14,15,16,17,18,19,20,21}, \
  {15,16,17,18,19,20,21,22}, \
  {16,17,18,19,20,21,22,23}, \
}

#define Z1 { \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {1, 1, 0, 0}, \
    {0, 1, 1, 0}, \
}
#define Z2 { \
    {0, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {1, 1, 0, 0}, \
    {1, 0, 0, 0}, \
}
#define S1 { \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {0, 8, 8, 0}, \
    {8, 8, 0, 0}, \
}
#define S2 { \
    {0, 0, 0, 0}, \
    {8, 0, 0, 0}, \
    {8, 8, 0, 0}, \
    {0, 8, 0, 0}, \
}
#define J1 { \
    { 0, 0, 0, 0}, \
    { 0, 0, 0, 0}, \
    {16,16,16, 0}, \
    { 0, 0,16, 0}, \
}
#define J2 { \
    { 0, 0, 0, 0}, \
    { 0,16, 0, 0}, \
    { 0,16, 0, 0}, \
    {16,16, 0, 0}, \
}
#define J3 { \
    { 0, 0, 0, 0}, \
    {16, 0, 0, 0}, \
    {16,16,16, 0}, \
    { 0, 0, 0, 0}, \
}
#define J4 { \
    { 0, 0, 0, 0}, \
    { 0,16,16, 0}, \
    { 0,16, 0, 0}, \
    { 0,16, 0, 0}, \
}
#define L1 { \
    { 0, 0, 0, 0}, \
    { 0, 0, 0, 0}, \
    { 5, 5, 5, 0}, \
    { 5, 0, 0, 0}, \
}
#define L2 { \
    { 0, 0, 0, 0}, \
    { 5, 5, 0, 0}, \
    { 0, 5, 0, 0}, \
    { 0, 5, 0, 0}, \
}
#define L3 { \
    { 0, 0, 0, 0}, \
    { 0, 0, 5, 0}, \
    { 5, 5, 5, 0}, \
    { 0, 0, 0, 0}, \
}
#define L4 { \
    { 0, 0, 0, 0}, \
    { 0, 5, 0, 0}, \
    { 0, 5, 0, 0}, \
    { 0, 5, 5, 0}, \
}
#define T1 { \
    { 0, 0, 0, 0}, \
    { 0, 0, 0, 0}, \
    {19,19,19, 0}, \
    { 0,19, 0, 0}, \
}
#define T2 { \
    { 0, 0, 0, 0}, \
    { 0,19, 0, 0}, \
    {19,19, 0, 0}, \
    { 0,19, 0, 0}, \
}
#define T3 { \
    { 0, 0, 0, 0}, \
    { 0,19, 0, 0}, \
    {19,19,19, 0}, \
    { 0, 0, 0, 0}, \
}
#define T4 { \
    { 0, 0, 0, 0}, \
    { 0,19, 0, 0}, \
    { 0,19,19, 0}, \
    { 0,19, 0, 0}, \
}
#define I1 { \
    { 0, 0, 0, 0}, \
    { 0, 0, 0, 0}, \
    {11,11,11,11}, \
    { 0, 0, 0, 0}, \
}
#define I2 { \
    { 0,11, 0, 0}, \
    { 0,11, 0, 0}, \
    { 0,11, 0, 0}, \
    { 0,11, 0, 0}, \
}
#define O1 {  \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {0,24,24, 0}, \
    {0,24,24, 0}, \
}

char currentMatrix[rows][cols]=spectrum;
//char currentMatrix[rows][cols];

//byte colors[8]={0, 0x03, 0x1C, 0x12, 0xE0, 0x63, 0xEC, 0xB6};
byte colors[26]={        0,B11100000,B11000100,B10101000,B10001100,B01110000,B01010100,B00111000,
                 B00011100,B00011101,B00011001,B00010110,B00010010,B00001111,B00001011,B00000111,
                 B00000011,B00100011,B01000011,B01100011,B10000010,B10100010,B11000001,B11100001,
                 B01110110,        0};

byte barGraphThresholds[11]={0,20,40,60,70,90,110,125,140,160,180};

//                      0,  1, 2,  3, 4,  5, 6,  7, 8, 9,10, 11,12,13,14, 15,16,17,18
char pieces[19][4][4]={O1, Z1,Z2, S1,S2, I1,I2, J1,J2,J3,J4, L1,L2,L3,L4, T1,T2,T3,T4};
byte pieceRotatedIndex[19]={0,2,1,4,3,6,5,8,9,10,7,12,13,14,11,16,17,18,15};

//                              0,  1,  2,  3,  4,  5,  6
char piecesGenerated[7][4][4]={O1, Z1, S1, I1, J1, L1, T1};
byte previousPieceIndex;

byte currentPieceIndex;        // stores current piece index: from 0 to 18
byte currentPieceRotatedIndex; // stores the index of the rotated version of the current piece
char currentPiece[4][4];       // stores the current piece matrix

//the top left corner of the current piece is tracked
int currentPieceRow;           // stores the row position of currentPiece[0][0]
int currentPieceCol;           // stores the col position of currentPiece[0][0]
int currentPieceMovedRow;      // stores the row position of currentPiece[0][0] after a certain move
int currentPieceMovedCol;      // stores the col position of currentPiece[0][0] after a certain move

byte numberOfLinesCleared;
boolean disableMove;    // disable is true only while the active piece is converted to dead blocks
boolean pieceDropping;
byte textColor;
boolean gameRunning;
boolean gameOver;
boolean fastDrop;

unsigned long lastButtonPressTime=millis();
unsigned long time=millis();               // used by all function that requires timing

// accelerometer
float beforePreviousAccel[3];
float previousAccel[3];
float currentAccel[3];

//vibration motor
boolean motorEnabled;
unsigned int motorOnTime;

// an array of seven byte to track the state of all seven buttons over time
// the order: up, left, right, down, drop, pause, restart
byte buttonState[7];
byte buttonPin[7]={upButton,leftButton,rightButton,downButton,dropButton,pauseButton,restartButton};
boolean buttonPressAllowed[7]; 

void clearTrace() {
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      if ((currentPieceRow+j>-1 && currentPieceRow+j<rows) && (currentPieceCol+i>-1 && currentPieceCol+i<cols)) {
        if (currentMatrix[currentPieceRow+j][currentPieceCol+i]>0) currentMatrix[currentPieceRow+j][currentPieceCol+i]=0;
      }
    }
  }
}

void updateCurrentMatrix() {
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      if ((currentPieceRow+j>-1 && currentPieceRow+j<rows) && (currentPieceCol+i>-1 && currentPieceCol+i<cols)) {
        if (currentPiece[j][i]!=0) currentMatrix[currentPieceRow+j][currentPieceCol+i]=currentPiece[j][i];
      }
    }
  }
}

void move(byte moveDirection) {  //updates: currentPieceRow, currentPieceCol, currentMatrix
  // moveDirection: 2-down; 3-left; 4-right
  if (moveDirection==2) {
    currentPieceMovedRow=currentPieceRow+1;
    currentPieceMovedCol=currentPieceCol;
  }
  else if (moveDirection==3) {
    currentPieceMovedRow=currentPieceRow;
    currentPieceMovedCol=currentPieceCol-1;
  }
  else if (moveDirection==4) {
    currentPieceMovedRow=currentPieceRow;
    currentPieceMovedCol=currentPieceCol+1;
  }
  // check if move is valid
  if (!checkOverlapAndOutOfBoundary(currentPieceIndex)) {
    clearTrace();
    currentPieceRow=currentPieceMovedRow;
    currentPieceCol=currentPieceMovedCol;
    updateCurrentMatrix();
  }
}

boolean checkOverlapAndOutOfBoundary(byte pieceIndex) {
  int row=currentPieceMovedRow;
  int col=currentPieceMovedCol;
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      if (pieces[pieceIndex][j][i]>0) {
        if (!( row+j<rows && (col+i>-1 && col+i<cols))) return true;  // checks out of boundary
        if (row+j>=0 && currentMatrix[row+j][col+i]<0) return true;  // checks overlap
      }
    }
  }
  return false;
}

void rotate() {  // updates: currentPieceRotatedIndex, currentPieceIndex, currentPiece, and currentMatrix
  currentPieceRotatedIndex=pieceRotatedIndex[currentPieceIndex];
  currentPieceMovedRow=currentPieceRow;
  currentPieceMovedCol=currentPieceCol;
    
  // check if the rotation is valid 
  if (!checkOverlapAndOutOfBoundary(currentPieceRotatedIndex)) {
    clearTrace();
    currentPieceIndex=currentPieceRotatedIndex;
    for (byte i=0;i<4;i++) {
      for (byte j=0;j<4;j++) {
        currentPiece[j][i]=pieces[currentPieceRotatedIndex][j][i];
      }
    }
    updateCurrentMatrix();
  }
}

boolean checkIfLanded() {
  for (int i=0;i<4;i++) {
    for (int j=0; j<4;j++) {
      if ((currentPieceRow+j>-1 && currentPieceRow+j<rows) && (currentPieceCol+i>-1 && currentPieceCol+i<cols)) {
        if (currentMatrix[currentPieceRow+j][currentPieceCol+i]>0) {
          if (currentPieceRow+j==bottomLimit) return true;            // if the piece has dropped on the bottom
          else if (currentMatrix[currentPieceRow+j+1][currentPieceCol+i]<0) return true;  //if there is any deadblock just below the piece
        }
      }
    }
  }
  return false;
}

boolean generatePiece() {  //updates: currentPiece, currentMatrix, currentPieceRow, currentPieceCol, currentPieceIndex
  byte pieceIndex=random(7);
  while (previousPieceIndex==pieceIndex) {
    pieceIndex=random(7);
  }
  previousPieceIndex=pieceIndex;
  for (byte i=0; i<4; i++) {
    for (byte j=0; j<4; j++) {
      currentPiece[j][i]=piecesGenerated[pieceIndex][j][i];
    }
  }
  for (byte i=0; i<4; i++) {
    for (byte j=0; j<2; j++) {
      if (piecesGenerated[pieceIndex][j+2][i]>0) {
        if (currentMatrix[j][i+2]<0) return false;
      }
    }
  }
  for (byte i=0; i<4; i++) {
    for (byte j=0; j<2; j++) {
      if (piecesGenerated[pieceIndex][j+2][i]>0) {
        currentMatrix[j][i+2]=piecesGenerated[pieceIndex][j+2][i];
      }
    }
  }
  currentPieceRow = -2;
  currentPieceCol = 2;
  switch (pieceIndex) {
    case 0:
      currentPieceIndex=0;
      break;
    case 1:
      currentPieceIndex=1;
      break;
    case 2:
      currentPieceIndex=3;
      break;
    case 3:
      currentPieceIndex=5;
      break;
    case 4:
      currentPieceIndex=7;
      break;
    case 5:
      currentPieceIndex=11;
      break;
    case 6:
      currentPieceIndex=15;
      break;
  }
  return true;
}

void convertToDeadBlock() {
  for (byte i=0;i<cols;i++) {
    for (byte j=0;j<rows;j++) {
      if (currentMatrix[j][i]>0) {
        currentMatrix[j][i]=-currentMatrix[j][i];
      }
    }
  }
}

void checkLinesCleared() {
  byte lineErased[rows];
  byte rowCounter=0;
  for (byte j=0;j<rows;j++) {
    byte colCounter=0;
    for (byte i=0;i<cols;i++) {
      if (currentMatrix[j][i]==0) {
        lineErased[j]=0;
        break;
      }
      else colCounter+=1;
    }
    if (colCounter==8) {
      lineErased[j]=1;
      rowCounter+=1;
    }  
  }
  // if no lines are cleared
  if (rowCounter==0) return;  
  // else
  flashClearedLines(lineErased,rowCounter);
  int tempMatrix[rows-rowCounter][cols];
  byte rowCounter2=0;
  for (byte j=0;j<rows;j++) {
    if (lineErased[j]==0) {
      for (byte i=0;i<cols;i++) tempMatrix[rowCounter2][i]=currentMatrix[j][i]+0;
      rowCounter2+=1;
    }
  }
  for (byte j=0;j<rowCounter;j++) {
    for (byte i=0;i<cols;i++) {
      currentMatrix[j][i]=0;
    }
  }
  for (byte j=0;j<rows-rowCounter;j++) {
    for (byte i=0;i<cols;i++) {
      currentMatrix[j+rowCounter][i]=tempMatrix[j][i];
    }
  }
  numberOfLinesCleared+=rowCounter;
}

void flashClearedLines(byte* lineErased,int numLinesCleared) {
  if (motorEnabled)  motorOnTime=150;
  int clearedLines[numLinesCleared][cols];
  byte rowCounter=0;
  for (byte n=0;n<2;n++) {
    rowCounter=0;
    for (byte j=0;j<rows;j++) {
      if (lineErased[j]==1) {
        for (byte i=0;i<cols;i++) {
          clearedLines[rowCounter][i]=currentMatrix[j][i]+0;
          currentMatrix[j][i]=0;
        }
        rowCounter+=1;
      }
    }
    time=millis();
    while (millis()<(time+100));
    rowCounter=0;
    for (byte j=0;j<rows;j++) {
      if (lineErased[j]==1) {
        for (byte i=0;i<cols;i++) {
          currentMatrix[j][i]=clearedLines[rowCounter][i]+0;
        }
        rowCounter+=1;
      }
    }
    time=millis();
    while (millis()<(time+100));
  }  
}

boolean checkIfLineClearedQuick() {
  for (byte j=0;j<rows;j++) {
    byte colCounter=0;
    for (byte i=0;i<cols;i++) {
      if (currentMatrix[j][i]==0) {
        break;
      }
      else colCounter+=1;
    }
    if (colCounter==8) {
      return true;
    }  
  }
  return false;
}

//  All Interrupt Functions Go Under Here

void pauseGame() {
  if (!gameOver) {
    gameRunning = !gameRunning;
    if (gameRunning) Serial.write(musicResume);
    else Serial.write(musicPause);
    for (int n=0;n<3;n++) {
      previousAccel[n]=0;
      currentAccel[n]=0;
    }
  }
}

void restartGame() {
  for (int i=0;i<cols;i++) {
    for (int j=0; j<rows; j++) currentMatrix[j][i]=0;
  }
  textColor=random(7)+1;
  numberOfLinesCleared=0;
  
  // reinitilize variables
  gameRunning=false;
  gameOver=false;
  disableMove=false;
  pieceDropping=false;
  fastDrop= false;
  generatePiece();
  digitalWriteFast(barGraphLeft,LOW); 
  digitalWriteFast(barGraphRight,LOW);
  Serial.write(musicRestart);
  for (int n=0;n<3;n++) {
    previousAccel[n]=0;
    currentAccel[n]=0;
  }
}

// 1-up; 2-down; 3-left; 4-right

void checkUp() {
  if (!disableMove && gameRunning) {
    rotate();
  }
}

void startDropping() {
  if (!gameRunning && digitalReadFast(dropButton))  {
    motorEnabled = ! motorEnabled;
    if (motorEnabled)  {
      motorOnTime=100;
    }
  }
  if (!disableMove && gameRunning) {
    pieceDropping=true;
  }
}

void checkLeft()  {
  if (!disableMove && gameRunning) {
    move(3);
  }
}

void checkRight() {  
  if (!disableMove && gameRunning) {
    move(4);
  }
}

void checkDrop() {  
  if (!disableMove && gameRunning) {
    fastDrop=true;
  }
}

//  All Interrupt Functions Go Above

void setup() {
  // Serial communication for both controlling the ATtiny85 and Debugging
  Serial.begin(57600);
  
  // motor pin
  pinMode(motor,OUTPUT);
  digitalWrite(motor,LOW);
  
  //  shift register pins
  pinMode(dataPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  
  pinMode(testPin,OUTPUT);
  
  // control the LED bar graphs
  pinMode(barGraphLeft, OUTPUT);
  pinMode(barGraphRight, OUTPUT);
  digitalWrite(barGraphLeft,LOW); 
  digitalWrite(barGraphRight,LOW); 
  
  // set all shift register pins to LOW
  digitalWrite(dataPin,LOW);
  digitalWrite(clockPin,LOW);
  digitalWrite(latchPin,LOW);
  
  //set all input pin to HIGH (pull-up resistor)
  digitalWrite(upButton,HIGH);
  digitalWrite(leftButton,HIGH);
  digitalWrite(rightButton,HIGH);
  digitalWrite(downButton,HIGH);
  digitalWrite(dropButton,HIGH);
  digitalWrite(pauseButton,HIGH);
  digitalWrite(restartButton,HIGH);
  
  //initialize variables
  numberOfLinesCleared=0;
  gameRunning = false;
  gameOver = false;
  disableMove = false;
  fastDrop = false;
  
  randomSeed(analogRead(randomPin));
  for (byte i=0;i<3;i++) byte rand1=random(7);
  
  gameOverFunc();
  //generatePiece();

  // SPI for controlling the LED driver chips
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  
  noInterrupts();
  TCCR2A = 0;  // set entire TCCR2A register to 0
  TCCR2B = 0;  // same for TCCR2B
  TCNT2  = 0;  // initialize counter value to 0
  OCR2A = 13;  // = (16*10^6) / (256*7*8*80fps) - 1 (must be <256)
  TCCR2A |= (1 << WGM21);                             // turn on CTC mode
  TCCR2B |= (1 << CS22) | (1 << CS21);  // set prescalar
  TIMSK2 |= (1 << OCIE2A);                            // enable timer compare interrupt
  interrupts();
}

boolean barGraphState=0;

// when interruptCounter reaches 480, update bargraph
int interruptCounter=0; 
byte currentCol = 0;
byte currentRun = 0;

unsigned long lastAccelTriggerTime=millis();
boolean moveLeftAllowed;
boolean moveRightAllowed;

// the accelerometer is centered at:
// y=0.05

ISR(TIMER2_COMPA_vect) {
  // monitor the state of all buttons, and decide whether a button press is valid
  for (byte i=0;i<7;i++) {
    byte temp = buttonState[i];
    temp = temp<<1;
    if (digitalReadFast(buttonPin[i])) temp +=1;
    buttonState[i] = temp;
    
    if ((buttonState[i] == 0) && buttonPressAllowed[i]) {
      buttonPressAllowed[i]=false;
      switch(i) {
        case 0: checkUp(); break;
        case 1: checkLeft(); break;
        case 2: checkRight(); break;
        case 3: startDropping(); break;
        case 4: checkDrop(); break;
        case 5: pauseGame(); break;
        case 6: restartGame(); break;  
      }
    }
    else if ((buttonState[i] == 255) && !buttonPressAllowed[i]) {
      buttonPressAllowed[i]=true;
    }
  }
  
  refreshDisplay();  
  
  // control the flashing of the LED bargraph
  if (interruptCounter>=3400) {
    interruptCounter=0;
    if ((gameRunning & !gameOver)) {
      if (barGraphState) {
        analogWrite(barGraphLeft, barGraphThresholds[numberOfLinesCleared/5]);
        analogWrite(barGraphRight, barGraphThresholds[numberOfLinesCleared/5]);
      }
      else {
        analogWrite(barGraphLeft, barGraphThresholds[(numberOfLinesCleared/5)+1]);
        analogWrite(barGraphRight, barGraphThresholds[(numberOfLinesCleared/5)+1]);
      }
      barGraphState=!barGraphState;
    }
  }
  else interruptCounter+=1;
  
  // using timer interrupt and a motorOnTime value to control the on/off and duration of the vibration motor
  if (motorOnTime>0) {
    digitalWriteFast(motor,HIGH);
    motorOnTime-=1;
  }
  else digitalWriteFast(motor,LOW);
}

//  used in the main loop to calculate the time interval between moves
unsigned long lastMoveTime=millis();
unsigned long lastLandedTime=millis();

void loop() {  
  if (gameRunning==false) {
    analogWrite(barGraphLeft, barGraphThresholds[numberOfLinesCleared/5]);
    analogWrite(barGraphRight, barGraphThresholds[numberOfLinesCleared/5]);
  }
  else {
    if (fastDrop==true) {
      while (!checkIfLanded()) move(2);
      time=millis();
      while (millis()<200+time);
      fastDrop=false;
    }
    if (digitalReadFast(downButton)==HIGH) pieceDropping=false;
    int timeInterval;
    if (!pieceDropping) timeInterval = 800-numberOfLinesCleared*15;
    else timeInterval= 120;
    time=millis();
    if (!checkIfLanded() && time-lastMoveTime>timeInterval) {
      move(2);
      lastMoveTime=millis();
    }
    else if (checkIfLanded()) {
      lastLandedTime=millis();
      if (!checkIfLineClearedQuick()) {  // if no line is cleared, wait for another 400ms for player to adjust the piece
        time=millis();
        while (time-lastLandedTime<400) time=millis();
      }
      if (!checkIfLanded()) return; // if the piece is quickly adjusted during the buffer period, restart the main loop and continue to move it
      else {
        disableMove = true;
        convertToDeadBlock();
        checkLinesCleared();
        if (!generatePiece()) {
          gameOverFunc();
          displayTextOverlay(0);
        }
        else {
          lastMoveTime=millis();
          disableMove=false;
        }
      }
    }
  }
}

void gameOverFunc() {
  analogWrite(barGraphLeft, barGraphThresholds[numberOfLinesCleared/5]);
  analogWrite(barGraphRight, barGraphThresholds[numberOfLinesCleared/5]);
  gameRunning = false;
  gameOver =true;
  Serial.write(musicRestart);
}

uint16_t rData;
uint16_t gData;
uint16_t bData;
byte color;
byte RRR;
byte GGG;
byte BB;
byte anodebyte=1;
char currentBlock;

void refreshDisplay() {    // refreshes the display; one run at a time; 7 runs per col; 8 cols
  currentRun++;
  if (currentRun==7) {
    currentRun=0;
    currentCol++;
    anodebyte = anodebyte << 1;
  }
  if (currentCol==8) {
    currentCol=0;
    anodebyte = 1;
  }
  //Serial.println(anodebyte);
  for (byte i=0;i<rows;i++) {
    
    currentBlock = currentMatrix[i][currentCol];
    currentBlock = (currentBlock>=0)? currentBlock:~currentBlock+1;  // calculate the absolute value of the current block
    color = colors[currentBlock];
    
    RRR = color >> 5;
    GGG = (color>>2) & 0x07;	// 0x07 = 0000 0111
    BB  = (color & 0x03) << 1;	// 0x03 = 0000 0011
    rData = (rData << 1) + ((RRR>currentRun)? 1:0);
    gData = (gData << 1) + ((GGG>currentRun)? 1:0);
    bData = (bData << 1) + ((BB>currentRun)? 1:0);
  } 
  
  SPI.transfer((rData >> 8)); SPI.transfer(rData);
  SPI.transfer((gData >> 8)); SPI.transfer(gData);
  SPI.transfer((bData >> 8)); SPI.transfer(bData);
  SPI.transfer(~anodebyte);
  
  digitalWriteFast(latchPin,HIGH);
  delayMicroseconds(1);
  digitalWriteFast(latchPin,LOW);
}

//64-bits
//    0000000000111000010001000101111100111001000101111101111000000000 : 15837774466244096
//    0000000001000100101001101101000001000101000101000001000100000000 : 19323711858675968
//    0000000001000001000101010101111001000101000101111001111000000000 : 18319368116477440
//    0000000001011101111101000101000001000101000101000001010000000000 : 26445798427595776
//    0000000001000101000101000101000001000100101001000001001000000000 : 19444108374577664
//    0000000000111001000101000101111100111000010001111101000100000000 : 16066472871186688
    
uint64_t gameOverValue[6]={
  15837774466244096LL,
  19323711858675968LL,
  18319368116477440LL,
  26445798427595776LL,
  19444108374577664LL,
  16066472871186688LL
};

void displayTextOverlay(int index) {  // i=0: GAMEOVER;
  textColor = random(24)+1;
  colors[25]=colors[textColor];
  colors[25]=colors[20];
  if (index==0)  {
    while (gameOver==true) {
      // there is a total of 49 frames; indexed 0 to 48
      for (byte n=0;n<57;n++) {
        if (gameOver==false) return;
        // first clean the area by setting all grids to 0
        for (byte i=0;i<cols;i++) {
          for (byte j=5;j<11;j++) {
            currentMatrix[j][i]=0;
          }
        } 
        // then draw the current frame of text
        for (int i=0; i<8; i++) {
          for (int j=0; j<6; j++) {
            if ( ( (gameOverValue[j]>>((57-n)+(7-i))) & 1 ) == 1 ) {
              currentMatrix[j+5][i]=25;
            }
          }
        }
        // wait for 100 ms before going to the next frame
        time=millis();
        while (millis()<(time+100));
      }
    }
  }
}
