uint16_t rData;
uint16_t gData;
uint16_t bData;
byte color;
byte RRR;
byte GGG;
byte BB;
byte anodebyte=1;

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
  for (byte i=0;i<rows;i++) {
    color=currentMatrix[i][currentCol];
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
  SPI.transfer(anodebyte);

  digitalWriteFast(latchPin,HIGH);
  delayMicroseconds(1); //for (int i=0;i<7;i++) __asm__("nop\n\t"); 
  digitalWriteFast(latchPin,LOW);
}

