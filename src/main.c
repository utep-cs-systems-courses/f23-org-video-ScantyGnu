#include <msp430.h>
#include "libTimer.h"
#include "lcdutils.h"
#include "lcddraw.h"

#define LED_RED BIT6
#define LED_GREEN BIT0
#define LEDS (BIT0 | BIT6)

#define SW1 BIT0 // button at P2.0
#define SW2 BIT1 // button at P2.1
#define SW3 BIT2 // button at P2.2
#define SW4 BIT3 // button at P2.3
#define SWITCHES (SW1 | SW2 | SW3 | SW4)

typedef struct{
  short col, row;
} Pos;

const short Gs = 880;
const short G = 932;
const short Fs = 987;
const short F = 523;
const short E = 554;
const short HDs = 1174;
const short Ds = 587;
const short D = 622;
const short HD = 1244;
const short Cs = 659;
const short Ce = 698;
const short LC = 349;
const short B = 739;
const short LAs = 391;
const short As = 783;
const short A = 830;
const short none = 0;

int noteNum = 143;

// Each line is a 3 groups of 6 notes so 18 notes
const short silentNight[] = {F, F, F, G, F, F, D, D, D, D, D, D, F, F, F, G, F, F,
		 D, D, D, D, D, D, Ce, Ce, Ce, none, Ce, Ce, A, A, A, A, A, A,
		 As, As, As, none, As, As, F, F, F, F, F, F, G, G, G, none, G, G,
		 As, As, As, A, G, G, F, F, F, G, F, F, D, D, D, D, D, D,
		 G, G, G, none, G, G, As, As, As, A, G, G, F, F, F, G, F, F,
		 D, D, D, D, D, D, Ce, Ce, Ce, none, Ce, Ce, HDs, HDs, HDs, Ce, A, A,
		 As, As, As, As, As, As, HD, HD, HD, HD, HD, HD, As, As, F, F, D, D,
		 F, F, F, Ds, LC, LC, LAs, LAs, LAs, LAs, LAs, LAs,
		       none, none, none, none, none, none};

const short littleDrummerBoy[] = {LC, LC, LC, LC, LC, LC, D, D, E, E, E, none, E, none, E, E,
				  F, E, F, F, E, E, E, E, E, E, E, E, none, none, none, none,
			       none, none, LC, none, LC, LC, D, D, E, none, E, none, E, none, E, E,
				  F, E, F, F, E, E, E, E, E, E, E, E, none, none, none, none,
				  none, none, D, D, E, E, F, F, G, none, G, none, G, G, A, A,
				  G, F, E, E, D, D, D, D, D, D, D, D, none, none, none, none,
				  none, none, D, D, E, E, F, F, G, none, G, none, G, G, A, A,
				  As, A, G, G, F, F, F, F, A, G, F, F, E, E, E, E,
				  G, F, E, E, D, D, D, D, D, D, D, D, D, D, D, D,
				  LC, LC, LC, LC, LC, LC, D, D, E, none, E, none, E, none, E, E,
				  F, E, F, F, E, E, E, E, E, E, E, E, none, none, none, none,
				  D, LC, D, D, LC, LC, LC, LC, LC, LC, LC, LC, LC, LC, LC, LC,
				  none, none, none, none, none, none, none, none};

const short joyToTheWorld[] = {HD, HD, Cs, B, A, A, A, G, F, F, E, E, D, D, D, A,
			       B, B, none, B, Cs, Cs, none, Cs, HD, none, HD, none, HD, Cs, B, A,
			       A, G, F, HD, HD, Cs, B, A, A, G, F, F, F, F, F, G,
			       A, A, G, F, E, none, E, none, F, G, F, E, D, HD, HD, B,
			       A, G, F, G, F, F, E, E, D, D, D, D};

const short *songs[] = {silentNight, littleDrummerBoy, joyToTheWorld};

Pos snow[] = {
  {90, 0},
  {80, 12},
  {30, 30},
  {90, 105},
  {12, 12},
  {80, 80},
  {105, 30},
  {60, 60},
  {30, 105},
  {12, 80}
};

int redrawScreen = 1;

void switch_init() {
  P2REN |= SWITCHES;/* enables resistors for switches */
  P2IE |= SWITCHES;/* enable interrupts from switches */
  P2OUT |= SWITCHES;/* pull-ups for switches */
  P2DIR &= ~SWITCHES;/* set switches' bits for input */
}

void led_init() {
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;/* leds initially off */
}

void buzzer_init(){
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
}

void wdt_init() {
  configureClocks();/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();/* enable periodic interrupt */
}

void redraw() {
  for (int i = 0; i < 9; i ++){
    snow[i].col -= 1;
    snow[i].row += 2;

    if (snow[i].col < 0){
      snow[i].col = 128;
    }
    if (snow[i].row > 120){
      snow[i].row = 0;
    }
    fillRectangle(snow[i].col, snow[i].row, 2, 2, COLOR_WHITE);
  }
}

void drawBG(){
  clearScreen(COLOR_BLACK);
  fillRectangle(0,121,128,40,COLOR_WHITE);
  drawTree();
  drawEffects();
}

void main(void) {
  timerAUpmode();
  switch_init();
  led_init();
  wdt_init();
  lcd_init();
  buzzer_init();
  or_sr(0x8);  // CPU off, GIE on

  while(1) {
    if (redrawScreen){
      drawBG();
      redraw();
      redrawScreen = 0;
    }
    or_sr(0x10);
  }
}

static int blinkState = 0;
static int songState = 0;
static int time = 1; // from 1 to 2048

// alternates when used to flash silly pattern
static int sillyBlink = 0;
// indicates state of wave pattern
static int waveState = 0;
static int wavePeriod = 64;

void move_blink(int mov){
  P1OUT &= ~LEDS; // Turn leds off
  blinkState += mov; // Move state machine
  sillyBlink = 0; // reset blink states
  waveState = 0;
  wavePeriod = 64; 
  if (blinkState >= 4) {
    blinkState = 0;
  }
  else if (blinkState < 0) {
    blinkState = 3;
  }
}

int currNote = 0;

void move_song(int mov){
  songState += mov;
  currNote =  0;
  if (songState >= 3){
    songState = 0;
  }
  else if (songState < 0){
    songState = 2;
  }
}

int numNotes[] = {143, 199, 75};
void sing(){
  short curr = songs[songState][currNote]; 
  CCR0 = curr;
  CCR1 = curr >> 1;
  currNote ++;

  if (currNote > numNotes[songState]){
    currNote = 0;
  }
}

void blink(){
  switch (blinkState) {
  case 0: // Silly blink state
    if (time % 256 == 0){
      if (sillyBlink){ // toggle green
	if (P1OUT & LED_RED){ // only if red is on
	  if (P1OUT & LED_GREEN){
	    P1OUT &= ~LED_GREEN;
	  }
	  else{
	    P1OUT |= LED_GREEN;
	  }
	}
      }
      else { // toggle red always
	if (P1OUT & LED_RED){
	  P1OUT &= ~LED_RED;
	}
	else{
	  P1OUT |= LED_RED;
	}
      }
      if (sillyBlink){
	sillyBlink = 0;
      }
      else{
	sillyBlink = 1;
      }
    }
    break;
    
  case 1: // Wave Pattern
    switch (waveState){
    case 0: //Green lights up
      if (time % wavePeriod == 0){
	P1OUT |= LED_GREEN;
	if (time % 64 == 0){
	  wavePeriod /= 2;
	}

	if (wavePeriod == 1){
	  wavePeriod = 128;
	  waveState = 1;
	}
      }
      else{
	P1OUT &= ~LED_GREEN;
      }
      P1OUT | LED_GREEN;
      break;
    case 1: // Red Lights up
      if (time % wavePeriod == 0){
	P1OUT |= LED_RED;
	if (time % 64 == 0){
	  wavePeriod /= 2;
	}
	
	if (wavePeriod == 1){
	  waveState = 2;
	}
      }
      else{
	P1OUT &= ~LED_RED;
      }
      break;
    case 2: // Green powers off
      if (time % wavePeriod == 0){
	P1OUT |= LED_GREEN;
	if (time % 64 == 0){
	  wavePeriod *= 2;
	}
	
	if (wavePeriod == 128){
	  P1OUT &= ~LED_GREEN;
	  wavePeriod = 1;
	  waveState = 3;
	}
      }
      else{
	P1OUT &= ~LED_GREEN;
      }
      break;
    case 3: // Red powers off
      if (time % wavePeriod == 0){
	P1OUT |= LED_RED;
	if (time % 64 == 0){
	  wavePeriod *= 2;
	}

	if (wavePeriod == 128){
	  P1OUT &= ~LED_RED;
	  wavePeriod = 128;
	  waveState = 0;
	}
      }
      else{
	P1OUT &= ~LED_RED;
      }
      break;
    default:
      P1OUT |= LEDS;
    }
    break;
    
  case 2:
    P1OUT |= LED_GREEN;
    break;
  case 3:
    P1OUT |= LED_RED;
    break;
  default: // Blink red led if there is an error
    if (P1OUT & LED_RED){
      P1OUT &= ~LED_RED;
    }
    else {
      P1OUT |= LED_RED;
    }
    break;
  }
}

void __interrupt_vec(PORT2_VECTOR) Port_2() {
  if (P2IFG & SW1 & P2IN){
    P2IFG &= ~SW1;
    move_blink(1);
  }
  else if (P2IFG & SW2 & P2IN){
    P2IFG &= ~SW2;
    move_blink(-1);
  }
  else if (P2IFG & SW3 & P2IN){
    P2IFG &= ~SW3;
    move_song(-1);
  }
  else if (P2IFG & SW4 & P2IN){
    P2IFG &= ~SW4;
    move_song(1);
  }
  P1IES |= (P1IN & SWITCHES);
  P1IES &= (P1IN | ~SWITCHES);
}

void wdt_c_handler() {
  time++;
  if (time >= 2048){
    time = 1;
  }
  if (time % 64 == 0){
    sing();
  }
  if (time % 256 == 0){
    redrawScreen = 1;
  }
  blink();
}
