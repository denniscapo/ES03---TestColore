//=== INCLUDES =============================================================================
#include "lcd.h"
#include "stm32l476xx.h"
#include <string.h>


/*  =========================================================================
                                 LCD MAPPING
    =========================================================================
LCD allows to display informations on six 14-segment digits and 4 bars:

  1       2       3       4       5       6
-----   -----   -----   -----   -----   -----   
|\|/| o |\|/| o |\|/| o |\|/| o |\|/|   |\|/|   BAR3
-- --   -- --   -- --   -- --   -- --   -- --   BAR2
|/|\| o |/|\| o |/|\| o |/|\| o |/|\|   |/|\|   BAR1
----- * ----- * ----- * ----- * -----   -----   BAR0

LCD segment mapping:
--------------------
  -----A-----        _ 
  |\   |   /|   COL |_|
  F H  J  K B          
  |  \ | /  |        _
  --G-- --M--   COL |_|
  |  / | \  |          
  E Q  P  N C          
  |/   |   \|        _ 
  -----D-----   DP  |_|

 An LCD character coding is based on the following matrix:
COM           0   1   2     3
SEG(n)      { E , D , P ,   N   }
SEG(n+1)    { M , C , COL , DP  }
SEG(23-n-1) { B , A , K ,   J   }
SEG(23-n)   { G , F , Q ,   H   }
with n positive odd number.

 The character 'A' for example is:
  -------------------------------
LSB   { 1 , 0 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
MSB   { 1 , 1 , 0 , 0   }
      -------------------
  'A' =  F    E   0   0 hexa

  @endverbati

*/

/* Constant table for cap characters 'A' --> 'Z' */
const uint16_t CapLetterMap[26] = {
        /* A      B      C      D      E      F      G      H      I  */
        0xFE00,0x6714,0x1d00,0x4714,0x9d00,0x9c00,0x3f00,0xfa00,0x0014,
        /* J      K      L      M      N      O      P      Q      R  */
        0x5300,0x9841,0x1900,0x5a48,0x5a09,0x5f00,0xFC00,0x5F01,0xFC01,
        /* S      T      U      V      W      X      Y      Z  */
        0xAF00,0x0414,0x5b00,0x18c0,0x5a81,0x00c9,0x0058,0x05c0
};

/* Constant table for number '0' --> '9' */
const uint16_t NumberMap[10] = {
        /* 0      1      2      3      4      5      6      7      8      9  */
        0x5F00,0x4200,0xF500,0x6700,0xEa00,0xAF00,0xBF00,0x04600,0xFF00,0xEF00
};


/* Macros used for set/reset bar LCD bar */
/* LCD BAR status: We don't write directly in LCD RAM for save the bar setting */
uint8_t t_bar[2] = {0x00,0x00};

//=== CONSTANTS ============================================================================
#define BAR0_ON  t_bar[1] |= 8
#define BAR0_OFF t_bar[1] &= ~8
#define BAR1_ON  t_bar[0] |= 8
#define BAR1_OFF t_bar[0] &= ~8
#define BAR2_ON  t_bar[1] |= 2
#define BAR2_OFF t_bar[1] &= ~2
#define BAR3_ON  t_bar[0] |= 2 
#define BAR3_OFF t_bar[0] &= ~2 

#define DOT                   ((uint16_t) 0x8000 ) /* for add decimal point in string */
#define DOUBLE_DOT            ((uint16_t) 0x4000) /* for add decimal point in string */

/* code for '(' character */
#define C_OPENPARMAP          ((uint16_t) 0x0028)

/* code for ')' character */
#define C_CLOSEPARMAP         ((uint16_t) 0x0011)

/* code for 'd' character */
#define C_DMAP                ((uint16_t) 0xf300)

/* code for 'm' character */
#define C_MMAP                ((uint16_t) 0xb210)

/* code for 'n' character */
#define C_NMAP                ((uint16_t) 0x2210)

/* code for '�' character */
#define C_UMAP                ((uint16_t) 0x6084)

/* constant code for '*' character */
#define C_STAR                ((uint16_t) 0xA0DD)

/* constant code for '-' character */
#define C_MINUS               ((uint16_t) 0xA000)

/* constant code for '+' character */
#define C_PLUS                ((uint16_t) 0xA014)

/* constant code for '/' */
#define C_SLATCH              ((uint16_t) 0x00c0)

/* constant code for � */
#define C_PERCENT_1           ((uint16_t) 0xec00)

/* constant code for small o */
#define C_PERCENT_2           ((uint16_t) 0xb300)

#define C_FULL                ((uint16_t) 0xffdd)



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// You are required to complete the following functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void delayLCD(void){
	unsigned int i;
	for(i=0;i<SystemCoreClock/50;i++){;}
	
}
void LCDPinInit(void){
	// LCD (24 segments, 4 commons, multiplexed 1/4 duty, 1/3 bias) on DIP28 connector
	//   VLCD = PC3
	//   COM0 = PA8     COM1  = PA9      COM2  = PA10    COM3  = PB9
	//   SEG0 = PA7     SEG6  = PD11     SEG12 = PB5     SEG18 = PD8
	//   SEG1 = PC5     SEG7  = PD13     SEG13 = PC8     SEG19 = PB14
	//   SEG2 = PB1     SEG8  = PD15     SEG14 = PC6     SEG20 = PB12
	//   SEG3 = PB13    SEG9  = PC7      SEG15 = PD14    SEG21 = PB0
	//   SEG4 = PB15    SEG10 = PA15     SEG16 = PD12    SEG22 = PC4
	//   SEG5 = PD9     SEG11 = PB4      SEG17 = PD10    SEG23 = PA6
	RCC->AHB2ENR |= 0xF;
	GPIOA->MODER &= ~(GPIO_MODER_MODER6|GPIO_MODER_MODER7|GPIO_MODER_MODER8|GPIO_MODER_MODER9|GPIO_MODER_MODER10|GPIO_MODER_MODER15);
	GPIOA->MODER |= GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1|GPIO_MODER_MODER8_1|GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1|GPIO_MODER_MODER15_1;
	GPIOA->AFR[0] &= ~ (GPIO_AFRL_AFRL6|GPIO_AFRL_AFRL7);
	GPIOA->AFR[0] |=  (11<<4*6|11<<4*7);
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFRH0|GPIO_AFRH_AFRH1|GPIO_AFRH_AFRH2|GPIO_AFRH_AFRH7);
	GPIOA->AFR[1] |=  (11<<4*0|11<<4*1|11<<4*2|11<<4*7);
	
	GPIOB->MODER &= ~(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER4|GPIO_MODER_MODER5|GPIO_MODER_MODER9|GPIO_MODER_MODER12|GPIO_MODER_MODER13|GPIO_MODER_MODER14|GPIO_MODER_MODER15);
	GPIOB->MODER |= GPIO_MODER_MODER0_1|GPIO_MODER_MODER1_1|GPIO_MODER_MODER4_1|GPIO_MODER_MODER5_1|GPIO_MODER_MODER9_1|GPIO_MODER_MODER12_1|GPIO_MODER_MODER13_1|GPIO_MODER_MODER14_1|GPIO_MODER_MODER15_1;
	GPIOB->AFR[0] &= (GPIO_AFRL_AFRL0|GPIO_AFRL_AFRL1|GPIO_AFRL_AFRL4|GPIO_AFRL_AFRL5);
	GPIOB->AFR[0] |= (11<<4*0|11<<4*1|11<<4*2|11<<4*4|11<<4*5);
	GPIOB->AFR[1] &= (GPIO_AFRH_AFRH1|GPIO_AFRH_AFRH4|GPIO_AFRH_AFRH5|GPIO_AFRH_AFRH6|GPIO_AFRH_AFRH7);
	GPIOB->AFR[1] |= (11<<4*1|11<<4*4|11<<4*5|11<<4*6|11<<4*7);
	
	GPIOC->MODER  &= ~(GPIO_MODER_MODER3|GPIO_MODER_MODER4|GPIO_MODER_MODER5|GPIO_MODER_MODER6|GPIO_MODER_MODER7|GPIO_MODER_MODER8);
	GPIOC->MODER |= GPIO_MODER_MODER3_1|GPIO_MODER_MODER4_1|GPIO_MODER_MODER5_1|GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1|GPIO_MODER_MODER8_1;
	GPIOC->AFR[0] &= ~(GPIO_AFRL_AFRL3|GPIO_AFRL_AFRL4|GPIO_AFRL_AFRL5|GPIO_AFRL_AFRL6|GPIO_AFRL_AFRL7);
	GPIOC->AFR[0] |= (11<<4*3|11<<4*4|11<<4*5|11<<4*6|11<<4*7);
	GPIOC->AFR[1] &= ~(GPIO_AFRH_AFRH0);
	GPIOC->AFR[1] |= (11<<4*0);
	
	GPIOD->MODER  &= ~(GPIO_MODER_MODER8|GPIO_MODER_MODER9|GPIO_MODER_MODER10|GPIO_MODER_MODER11|GPIO_MODER_MODER12|GPIO_MODER_MODER13|GPIO_MODER_MODER14|GPIO_MODER_MODER15);
	GPIOD->MODER |= GPIO_MODER_MODER8_1|GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1|GPIO_MODER_MODER11_1|GPIO_MODER_MODER12_1|GPIO_MODER_MODER13_1|GPIO_MODER_MODER14_1|GPIO_MODER_MODER15_1;
	GPIOD->AFR[1] &= ~(GPIO_AFRH_AFRH0|GPIO_AFRH_AFRH1|GPIO_AFRH_AFRH2|GPIO_AFRH_AFRH3|GPIO_AFRH_AFRH4|GPIO_AFRH_AFRH5|GPIO_AFRH_AFRH6|GPIO_AFRH_AFRH7);
	GPIOD->AFR[1] |= (11<<4*0|11<<4*1|11<<4*2|11<<4*3|11<<4*4|11<<4*5|11<<4*6|11<<4*7);
	
	
}

void LCDDisplayString(uint8_t* ptr){
	int i=0;
	while(*ptr){	
		LCDWriteChar(ptr,0,0,i);
		ptr++;
		i++;
	}
	LCD->SR |= LCD_SR_UDR;
}

void LCDDisplayStringEx(uint8_t* ptr){
	uint8_t* stringa=ptr;
	int len=strlen((char*)stringa);
	int i=0,j=0;
//	do{
//		while(*(stringa+j)&&(i<6)){
//			LCDWriteChar(stringa+j,0,0,i);
//			stringa++;
//			i++;
//		}
//		if((i==6)&&(*(stringa+j))){
//			i=0;
//			j++;
//			stringa=ptr;
//			delayLCD();
//			LCDClear();
//		}
//		
//	}
//	while(len>(j+5));
	for(j=0;len>(j+5);j++){
		while(*(stringa+j)&&(i<6)){
			LCDWriteChar(stringa+j,0,0,i);
			stringa++;
			i++;
		}
		if((i==6)&&(*(stringa+j))){
			i=0;
			stringa=ptr;
			delayLCD();
			LCDClear();
		}
	}
}

void LCDDisplayName(void){
}

void LCDConfigure(void){
	LCD->CR &= ~(LCD_CR_MUX_SEG_Msk|LCD_CR_BIAS_Msk|LCD_CR_DUTY_Msk);
	LCD->CR |= (LCD_CR_DUTY_1|LCD_CR_DUTY_0)|(LCD_CR_BIAS_1);
	LCD->FCR &= ~LCD_FCR_CC_Msk;
	LCD->FCR |= LCD_FCR_CC_1;
	LCD->FCR &= ~LCD_FCR_PS_Msk;
	LCD->FCR |= 3<<LCD_FCR_PS_Pos;
	LCD->CR |= LCD_CR_LCDEN;
}
void LCDContrastoUp(void){
	uint8_t c= LCDContrastoValue();
	if(c<7){
		c++;
		LCDContrastoSet(c);
	}
	
}
void LCDContrastoDown(void){
	uint8_t c= LCDContrastoValue();
	if(c>0){
		c--;
		LCDContrastoSet(c);
	}
}
uint8_t LCDContrastoValue(void){
	return 	(LCD->FCR & LCD_FCR_CC_Msk)>>10;
}
void LCDContrastoSet(uint8_t c){
	LCD->FCR &= ~LCD_FCR_CC_Msk;
	LCD->FCR |= c<<10;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not change the codes below
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LCDInitialization(void){
	LCDPinInit();
	LCDClockInit();	
	LCDConfigure();
	LCDClear();
}


void LCDClockInit(void){
	// Enable write access to Backup domain
	if ( (RCC->APB1ENR1 & RCC_APB1ENR1_PWREN) == 0)
		RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;	// Power interface clock enable
	(void) RCC->APB1ENR1;  // Delay after an RCC peripheral clock enabling
	
	// Select LSE as RTC clock soucre 
	if ( (PWR->CR1 & PWR_CR1_DBP) == 0) {
		PWR->CR1  |= PWR_CR1_DBP;				  			// Enable write access to Backup domain
		while((PWR->CR1 & PWR_CR1_DBP) == 0);  	// Wait for Backup domain Write protection disable
	}
	
	// Reset LSEON and LSEBYP bits before configuring the LSE
	RCC->BDCR &= ~(RCC_BDCR_LSEON | RCC_BDCR_LSEBYP);

	// RTC Clock selection can be changed only if the Backup Domain is reset
	RCC->BDCR |=  RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	
	// Note from STM32L4 Reference Manual: 	
  // RTC/LCD Clock:  (1) LSE is in the Backup domain. (2) HSE and LSI are not.	
	while((RCC->BDCR & RCC_BDCR_LSERDY) == 0){  // Wait until LSE clock ready
		RCC->BDCR |= RCC_BDCR_LSEON;
	}
	
	// Select LSE as RTC clock source
	// BDCR = Backup Domain Control Register 
	RCC->BDCR	&= ~RCC_BDCR_RTCSEL;	  // RTCSEL[1:0]: 00 = No Clock, 01 = LSE, 10 = LSI, 11 = HSE
	RCC->BDCR	|= RCC_BDCR_RTCSEL_0;   // Select LSE as RTC clock	
	
	RCC->APB1ENR1 &= ~RCC_APB1ENR1_PWREN;	// Power interface clock disable
	
	// Wait for the external capacitor Cext which is connected to the VLCD pin is charged (approximately 2ms for Cext=1uF) 
	
	// Enable LCD peripheral Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_LCDEN;
	
	// Enable SYSCFG 
	// RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}
	

void LCDClear(void){
  uint8_t counter = 0;

  // Wait until LCD ready */  
	while ((LCD->SR & LCD_SR_UDR) != 0); // Wait for Update Display Request Bit
  
  for (counter = 0; counter <= 15; counter++) {
    LCD->RAM[counter] = 0;
  }

  /* Update the LCD display */
	LCD->SR |= LCD_SR_UDR; 
}


// Setting bar on LCD, writes bar value in LCD frame buffer 
void LCDBar(void) {

	// TO wait LCD Ready *
  while ((LCD->SR & LCD_SR_UDR) != 0); // Wait for Update Display Request Bit
	// Bar 0: COM3, LCD_SEG11 -> MCU_LCD_SEG8
	// Bar 1: COM2, LCD_SEG11 -> MCU_LCD_SEG8
	// Bar 2: COM3, LCD_SEG9 -> MCU_LCD_SEG25
	// Bar 3: COM2, LCD_SEG9 -> MCU_LCD_SEG25
	
  LCD->RAM[4] &= ~(1U << 8 | 1U << 25);
  LCD->RAM[6] &= ~(1U << 8 | 1U << 25);
	
  /* bar1 bar3 */
  if (BAR0_ON)
		LCD->RAM[6] |= 1U << 8;
  
  if (BAR1_ON)
		LCD->RAM[4] |= 1U << 8;
 
	if (BAR2_ON)
		LCD->RAM[6] |= 1U << 25;
  
  if (BAR1_ON)
		LCD->RAM[4] |= 1U << 25;
	
	LCD->SR |= LCD_SR_UDR; 
}

void LCDbarOn(bar b){
	while ((LCD->SR & LCD_SR_UDR) != 0);
	if(b==BAR0){
		LCD->RAM[6] |= 1U << 8;
	}
	else if(b==BAR1){
		LCD->RAM[4] |= 1U << 8;
	}
	else if(b==BAR2){
		LCD->RAM[6] |= 1U << 25;
	}
	else if(b==BAR3){
		LCD->RAM[4] |= 1U << 25;
	}
	LCD->SR |= LCD_SR_UDR;
}

void LCDbarOff(bar b){
	while ((LCD->SR & LCD_SR_UDR) != 0);
	if(b==BAR0){
		LCD->RAM[6] &= ~1U << 8;
	}
	else if(b==BAR1){
		LCD->RAM[4] &= ~1U << 8;
	}
	else if(b==BAR2){
		LCD->RAM[6] &= ~1U << 25;
	}
	else if(b==BAR3){
		LCD->RAM[4] &= ~1U << 25;
	}
	LCD->SR |= LCD_SR_UDR;
}

/**
  * @brief  Converts an ascii char to the a LCD digit.
  * @param  c: a char to display.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  colon : flag indicating if a colon has to be add in front
  *         of displayed character.
  *         This parameter can be: colon_OFF or colon_ON.
	* @param 	digit array with segment 
  * @retval None
  */
static void LCDConvCharSeg(uint8_t* c, bool point, bool colon, uint8_t* digit) {
  uint16_t ch = 0 ;
  uint8_t loop = 0, index = 0;
  
  switch (*c)
    {
    case ' ' :
      ch = 0x00;
      break;

    case '*':
      ch = C_STAR;
      break;

    case '(' :
      ch = C_OPENPARMAP;
      break;

    case ')' :
      ch = C_CLOSEPARMAP;
      break;
      
    case 'd' :
      ch = C_DMAP;
      break;
    
    case 'm' :
      ch = C_MMAP;
      break;
    
    case 'n' :
      ch = C_NMAP;
      break;

    case '�' :
      ch = C_UMAP;
      break;

    case '-' :
      ch = C_MINUS;
      break;

    case '+' :
      ch = C_PLUS;
      break;

    case '/' :
      ch = C_SLATCH;
      break;  
      
    case '�' :
      ch = C_PERCENT_1;
      break;  
		
    case '%' :
      ch = C_PERCENT_2; 
      break;
    case 255 :
      ch = C_FULL;
      break ;
    
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':			
      ch = NumberMap[*c-0x30];		
      break;
          
    default:
      /* The character c is one letter in upper case*/
      if ( (*c < 0x5b) && (*c > 0x40) )
      {
        ch = CapLetterMap[*c-'A'];
      }
      /* The character c is one letter in lower case*/
      if ( (*c <0x7b) && ( *c> 0x60) )
      {
        ch = CapLetterMap[*c-'a'];
      }
      break;
  }
       
  /* Set the digital point can be displayed if the point is on */
  if (point)
  {
    ch |= 0x0002;
  }

  /* Set the "COL" segment in the character that can be displayed if the colon is on */
  if (colon)
  {
    ch |= 0x0020;
  }		

	for (loop = 12,index=0 ;index < 4; loop -= 4,index++)
  {
    digit[index] = (ch >> loop) & 0x0f; /*To isolate the less significant digit */
  }
	
}


/**
  * @brief Write a character in the LCD frame buffer
  * @param  ch: the character to display.
  * @param  Point: a point to add in front of char
  * @param  Colon: flag indicating if a colon character has to be added in front
  *         of displayed character.         
  * @param  Position: position in the LCD of the character to write [1:6]
	*/

void LCDWriteChar(uint8_t* ch, bool point, bool colon, uint8_t position){
  uint8_t digit[4];     /* Digit frame buffer */
   
  // Convert displayed character in segment in array digit 
  LCDConvCharSeg(ch, point, colon, digit);

  // TO wait LCD Ready *
  while ((LCD->SR & LCD_SR_UDR) != 0); // Wait for Update Display Request Bit
  
  switch (position) {
		
    /* Position 1 on LCD (digit1)*/
    case 0:
	
			LCD->RAM[0] &= ~( 1U << 4 | 1U << 23 | 1U << 22 | 1U << 3 );
      LCD->RAM[2] &= ~( 1U << 4 | 1U << 23 | 1U << 22 | 1U << 3 );
      LCD->RAM[4] &= ~( 1U << 4 | 1U << 23 | 1U << 22 | 1U << 3 );
      LCD->RAM[6] &= ~( 1U << 4 | 1U << 23 | 1U << 22 | 1U << 3 );
			/* 1G 1B 1M 1E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 4) | (((digit[0] & 0x2) >> 1) << 23) | (((digit[0] & 0x4) >> 2) << 22) | (((digit[0] & 0x8) >> 3) << 3);
      /* 1F 1A 1C 1D  */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 4) | (((digit[1] & 0x2) >> 1) << 23) | (((digit[1] & 0x4) >> 2) << 22) | (((digit[1] & 0x8) >> 3) << 3);
      /* 1Q 1K 1Col 1P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 4) | (((digit[2] & 0x2) >> 1) << 23) | (((digit[2] & 0x4) >> 2) << 22) | (((digit[2] & 0x8) >> 3) << 3);
      /* 1H 1J 1DP 1N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 4) | (((digit[3] & 0x2) >> 1) << 23) | (((digit[3] & 0x4) >> 2) << 22) | (((digit[3] & 0x8) >> 3) << 3);

			break;

    /* Position 2 on LCD (digit2)*/
    case 1:
			
			LCD->RAM[0] &= ~( 1U << 6 | 1U << 13 | 1U << 12 | 1U << 5 );
      LCD->RAM[2] &= ~( 1U << 6 | 1U << 13 | 1U << 12 | 1U << 5 );
      LCD->RAM[4] &= ~( 1U << 6 | 1U << 13 | 1U << 12 | 1U << 5 );
      LCD->RAM[6] &= ~( 1U << 6 | 1U << 13 | 1U << 12 | 1U << 5 );
			/* 2G 2B 2M 2E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 6) | (((digit[0] & 0x2) >> 1) << 13) | (((digit[0] & 0x4) >> 2) << 12) | (((digit[0] & 0x8) >> 3) << 5);
      /* 2F 2A 2C 2D  */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 6) | (((digit[1] & 0x2) >> 1) << 13) | (((digit[1] & 0x4) >> 2) << 12) | (((digit[1] & 0x8) >> 3) << 5);
      /* 2Q 2K 2Col 2P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 6) | (((digit[2] & 0x2) >> 1) << 13) | (((digit[2] & 0x4) >> 2) << 12) | (((digit[2] & 0x8) >> 3) << 5);
      /* 2H 2J 2DP 2N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 6) | (((digit[3] & 0x2) >> 1) << 13) | (((digit[3] & 0x4) >> 2) << 12) | (((digit[3] & 0x8) >> 3) << 5);

			break;
    
    /* Position 3 on LCD (digit3)*/
    case 2:
			
			LCD->RAM[0] &= ~( 1U << 15 | 1U << 29 | 1U << 28 | 1U << 14 );
      LCD->RAM[2] &= ~( 1U << 15 | 1U << 29 | 1U << 28 | 1U << 14 );
      LCD->RAM[4] &= ~( 1U << 15 | 1U << 29 | 1U << 28 | 1U << 14 );
      LCD->RAM[6] &= ~( 1U << 15 | 1U << 29 | 1U << 28 | 1U << 14 );
			/* 3G 3B 3M 3E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 15) | (((digit[0] & 0x2) >> 1) << 29) | (((digit[0] & 0x4) >> 2) << 28) | (((digit[0] & 0x8) >> 3) << 14);
      /* 3F 3A 3C 3D */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 15) | (((digit[1] & 0x2) >> 1) << 29) | (((digit[1] & 0x4) >> 2) << 28) | (((digit[1] & 0x8) >> 3) << 14);
      /* 3Q 3K 3Col 3P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 15) | (((digit[2] & 0x2) >> 1) << 29) | (((digit[2] & 0x4) >> 2) << 28) | (((digit[2] & 0x8) >> 3) << 14);
      /* 3H 3J 3DP  3N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 15) | (((digit[3] & 0x2) >> 1) << 29) | (((digit[3] & 0x4) >> 2) << 28) | (((digit[3] & 0x8) >> 3) << 14);

			break;
    
    /* Position 4 on LCD (digit4)*/
    case 3:
			
			LCD->RAM[0] &= ~( 1U << 31 | 1U << 30);
			LCD->RAM[1] &= ~( 1U << 1 | 1U << 0 );
      LCD->RAM[2] &= ~( 1U << 31 | 1U << 30);
			LCD->RAM[3] &= ~( 1U << 1 | 1U << 0 );
      LCD->RAM[4] &= ~( 1U << 31 | 1U << 30);
			LCD->RAM[5] &= ~( 1U << 1 | 1U << 0 );
      LCD->RAM[6] &= ~( 1U << 31 | 1U << 30);
			LCD->RAM[7] &= ~( 1U << 1 | 1U << 0 );
			/* 4G 4B 4M 4E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 31) | (((digit[0] & 0x8) >> 3) << 30);
			LCD->RAM[1] |= (((digit[0] & 0x2) >> 1) << 1) | (((digit[0] & 0x4) >> 2) << 0);
      /* 4F 4A 4C 4D */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 31) | (((digit[1] & 0x8) >> 3) << 30);
			LCD->RAM[3] |= (((digit[1] & 0x2) >> 1) << 1) | (((digit[1] & 0x4) >> 2) << 0);
      /* 4Q 4K 4Col 4P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 31) | (((digit[2] & 0x8) >> 3) << 30);
			LCD->RAM[5] |= (((digit[2] & 0x2) >> 1) << 1) | (((digit[2] & 0x4) >> 2) << 0);
      /* 4H 4J 4DP  4N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 31) | (((digit[3] & 0x8) >> 3) << 30);
			LCD->RAM[7] |= (((digit[3] & 0x2) >> 1) << 1) | (((digit[3] & 0x4) >> 2) << 0);

			break;
    
    /* Position 5 on LCD (digit5)*/
    case 4:
			
			LCD->RAM[0] &= ~( 1U << 25 | 1U << 24);
			LCD->RAM[1] &= ~( 1U << 3 | 1U << 2 );
      LCD->RAM[2] &= ~( 1U << 25 | 1U << 24);
			LCD->RAM[3] &= ~( 1U << 3 | 1U << 2 );
      LCD->RAM[4] &= ~( 1U << 25 | 1U << 24 );
			LCD->RAM[5] &= ~( 1U << 3 | 1U << 2 );
      LCD->RAM[6] &= ~( 1U << 25 | 1U << 24 );
			LCD->RAM[7] &= ~( 1U << 3 | 1U << 2 );
			/* 5G 5B 5M 5E */
      LCD->RAM[0] |= (((digit[0] & 0x2) >> 1) << 25) | (((digit[0] & 0x4) >> 2) << 24);
			LCD->RAM[1] |= ((digit[0] & 0x1) << 3) | (((digit[0] & 0x8) >> 3) << 2);
      /* 5F 5A 5C 5D */
      LCD->RAM[2] |= (((digit[1] & 0x2) >> 1) << 25) | (((digit[1] & 0x4) >> 2) << 24);
			LCD->RAM[3] |= ((digit[1] & 0x1) << 3) | (((digit[1] & 0x8) >> 3) << 2);
      /* 5Q 5K 5Col 5P  */
      LCD->RAM[4] |= (((digit[2] & 0x2) >> 1) << 25) | (((digit[2] & 0x4) >> 2) << 24);
			LCD->RAM[5] |= ((digit[2] & 0x1) << 3) | (((digit[2] & 0x8) >> 3) << 2);
      /* 5H 5J 5DP  5N  */
      LCD->RAM[6] |= (((digit[3] & 0x2) >> 1) << 25) | (((digit[3] & 0x4) >> 2) << 24);
			LCD->RAM[7] |= ((digit[3] & 0x1) << 3) | (((digit[3] & 0x8) >> 3) << 2);

			break;
    
    /* Position 6 on LCD (digit6)*/
    case 5:
			
			LCD->RAM[0] &= ~( 1U << 17 | 1U << 8 | 1U << 9 | 1U << 26 );
      LCD->RAM[2] &= ~( 1U << 17 | 1U << 8 | 1U << 9 | 1U << 26 );
      LCD->RAM[4] &= ~( 1U << 17 | 1U << 8 | 1U << 9 | 1U << 26 );
      LCD->RAM[6] &= ~( 1U << 17 | 1U << 8 | 1U << 9 | 1U << 26 );
			/* 6G 6B 6M 6E */
      LCD->RAM[0] |= ((digit[0] & 0x1) << 17) | (((digit[0] & 0x2) >> 1) << 8) | (((digit[0] & 0x4) >> 2) << 9) | (((digit[0] & 0x8) >> 3) << 26);
      /* 6F 6A 6C 6D */
      LCD->RAM[2] |= ((digit[1] & 0x1) << 17) | (((digit[1] & 0x2) >> 1) << 8) | (((digit[1] & 0x4) >> 2) << 9) | (((digit[1] & 0x8) >> 3) << 26);
      /* 6Q 6K 6Col 6P  */
      LCD->RAM[4] |= ((digit[2] & 0x1) << 17) | (((digit[2] & 0x2) >> 1) << 8) | (((digit[2] & 0x4) >> 2) << 9) | (((digit[2] & 0x8) >> 3) << 26);
      /* 6H 6J 6DP  6N  */
      LCD->RAM[6] |= ((digit[3] & 0x1) << 17) | (((digit[3] & 0x2) >> 1) << 8) | (((digit[3] & 0x4) >> 2) << 9) | (((digit[3] & 0x8) >> 3) << 26);

			break;
    
     default:
      break;
  }

  /* Refresh LCD  bar */
  //LCDBar();

  // Update the LCD display 
	// Set the Update Display Request.
	//
	// Each time software modifies the LCD_RAM, it must set the UDR bit to transfer the updated
	// data to the second level buffer. The UDR bit stays set until the end of the update and during
	// this time the LCD_RAM is write protected.
	//
	// When the display is enabled, the update is performed only for locations for which
	// commons are active (depending on DUTY). For example if DUTY = 1/2, only the
	// LCD_DISPLAY of COM0 and COM1 will be updated.
	LCD->SR |= LCD_SR_UDR; 								// Update display request. Cleared by hardware
	//while ((LCD->SR & LCD_SR_UDD) == 0);	// Wait Until the LCD display is done
	//LCD->CLR &= ~LCD_CLR_UDDC;            // Clear UDD flag
  
}
