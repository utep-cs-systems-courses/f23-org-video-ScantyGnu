/** \file lcddraw.c
 *  \brief Adapted from RobG's EduKit
 */
#include "lcdutils.h"
#include "lcddraw.h"


/** Draw single pixel at x,row 
 *
 *  \param col Column to draw to
 *  \param row Row to draw to
 *  \param colorBGR Color of pixel in BGR
 */
void drawPixel(u_char col, u_char row, u_int colorBGR) 
{
  lcd_setArea(col, row, col, row);
  lcd_writeColor(colorBGR);
}

void drawTriangle(u_char col, u_char row, u_char width, u_char height, u_int colorBGR){
  row = row + height - 1;
  for (int i = 0; i < height/2; i ++){
    fillRectangle(col, row - (i*2), width-i, 2, colorBGR);
  }
}

void drawRevTriangle(u_char col, u_char row, u_char width, u_char height, u_int colorBGR){
  row = row + height - 1;
  for (int i = 0; i < height/2; i++){
    fillRectangle(col+i, row - (i*2), width-i, 2, colorBGR);
  }
}

void drawTree(){
  drawRevTriangle(50, 127, 3, 6, COLOR_BROWN);
  fillRectangle(53, 127, 20, 6, COLOR_BROWN);
  drawTriangle(73, 127, 3, 6, COLOR_BROWN);
  drawRevTriangle(27, 93, 17, 34, COLOR_GREEN);
  fillRectangle(44, 94, 38, 33, COLOR_GREEN);
  drawTriangle(82, 93, 17, 34, COLOR_GREEN); 
  drawRevTriangle(34, 53, 20, 40, COLOR_GREEN);
  fillRectangle(54, 54, 20, 39, COLOR_GREEN);
  drawTriangle(74, 53, 20, 40, COLOR_GREEN);
  drawRevTriangle(49, 29, 12, 24, COLOR_GREEN);
  fillRectangle(61, 28, 6, 25, COLOR_GREEN);
  drawTriangle(67, 29, 12, 24, COLOR_GREEN);
  fillRectangle(62, 27, 4, 1, COLOR_GREEN);
}

void drawSphere(u_char col, u_char row, u_int colorBGR){
  fillRectangle(col+26, row+27, 3, 3, colorBGR);
}

void drawEffects(){
  drawSphere(39, 7, COLOR_RED);
  drawSphere(44, 18, COLOR_BLUE);
  drawSphere(29, 19, COLOR_YELLOW);
  drawSphere(30, 30, COLOR_BLUE);
  drawSphere(45, 29, COLOR_YELLOW);
  drawSphere(25, 39, COLOR_RED);
  drawSphere(49, 45, COLOR_BLUE);
  drawSphere(41, 50, COLOR_YELLOW);
  drawSphere(50, 56, COLOR_RED);
  drawSphere(23, 61, COLOR_RED);
  drawSphere(26, 72, COLOR_YELLOW);
  drawSphere(34, 76, COLOR_BLUE);
  drawSphere(45, 71, COLOR_RED);
  drawSphere(55, 76, COLOR_YELLOW);
  drawSphere(15, 90, COLOR_BLUE);
  drawSphere(40, 92, COLOR_YELLOW);
  drawSphere(61, 94, COLOR_BLUE);
}

/** Fill rectangle
 *
 *  \param colMin Column start
 *  \param rowMin Row start
 *  \param width Width of rectangle
 *  \param height height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void fillRectangle(u_char colMin, u_char rowMin, u_char width, u_char height, 
		   u_int colorBGR)
{
  u_char colLimit = colMin + width, rowLimit = rowMin + height;
  lcd_setArea(colMin, rowMin, colLimit - 1, rowLimit - 1);
  u_int total = width * height;
  u_int c = 0;
  while ((c++) < total) {
    lcd_writeColor(colorBGR);
  }
}

/** Clear screen (fill with color)
 *  
 *  \param colorBGR The color to fill screen
 */
void clearScreen(u_int colorBGR) 
{
  u_char w = screenWidth;
  u_char h = screenHeight;
  fillRectangle(0, 0, screenWidth, screenHeight, colorBGR);
}

/** 5x7 font - this function draws background pixels
 *  Adapted from RobG's EduKit
 */
void drawChar5x7(u_char rcol, u_char rrow, char c, 
     u_int fgColorBGR, u_int bgColorBGR) 
{
  u_char col = 0;
  u_char row = 0;
  u_char bit = 0x01;
  u_char oc = c - 0x20;

  lcd_setArea(rcol, rrow, rcol + 4, rrow + 7); /* relative to requested col/row */
  while (row < 8) {
    while (col < 5) {
      u_int colorBGR = (font_5x7[oc][col] & bit) ? fgColorBGR : bgColorBGR;
      lcd_writeColor(colorBGR);
      col++;
    }
    col = 0;
    bit <<= 1;
    row++;
  }
}

/** Draw string at col,row
 *  Type:
 *  FONT_SM - small (5x8,) FONT_MD - medium (8x12,) FONT_LG - large (11x16)
 *  FONT_SM_BKG, FONT_MD_BKG, FONT_LG_BKG - as above, but with background color
 *  Adapted from RobG's EduKit
 *
 *  \param col Column to start drawing string
 *  \param row Row to start drawing string
 *  \param string The string
 *  \param fgColorBGR Foreground color in BGR
 *  \param bgColorBGR Background color in BGR
 */
void drawString5x7(u_char col, u_char row, char *string,
		u_int fgColorBGR, u_int bgColorBGR)
{
  u_char cols = col;
  while (*string) {
    drawChar5x7(cols, row, *string++, fgColorBGR, bgColorBGR);
    cols += 6;
  }
}


/** Draw rectangle outline
 *  
 *  \param colMin Column start
 *  \param rowMin Row start 
 *  \param width Width of rectangle
 *  \param height Height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void drawRectOutline(u_char colMin, u_char rowMin, u_char width, u_char height,
		     u_int colorBGR)
{
  /**< top & bot */
  fillRectangle(colMin, rowMin, width, 1, colorBGR);
  fillRectangle(colMin, rowMin + height, width, 1, colorBGR);

  /**< left & right */
  fillRectangle(colMin, rowMin, 1, height, colorBGR);
  fillRectangle(colMin + width, rowMin, 1, height, colorBGR);
}

