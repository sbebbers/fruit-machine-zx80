#include <stdio.h>
#include <stdlib.h>
#include "zx80s.h"

/**
 * Function prototypes
 */
int main();

/**
 * Game function prototypes
 */
unsigned short checkReels(unsigned char reel1, unsigned char reel2, unsigned char reel3);
unsigned char setReel(unsigned char reel);
void startGame();
void titleScreen();
void playAgain();

/**
 * Base function prototypes
 */
void cls();
void printSpc(unsigned char spc, unsigned char txt[31]);
void printTab(unsigned char tab, unsigned char txt[28]);
void prompt(unsigned char txt[32], unsigned char lineNumber);
void randomise();
void zx80Init();

/**
 * Reusable/API variables
 */
unsigned char _strBuffer[33];
unsigned char random;
unsigned char i;

/**
 * Game variables
 */
unsigned short pounds		= 500;
unsigned char winLine[]		=
{
	45, 45, 45
};

/**
 * Game constants
 */
unsigned char REEL			= 16;
unsigned short SPINCOST		= 25;

unsigned char REEL1[]		= "*$\xa3x*$*x*-x*x-x*";
unsigned char REEL2[]		= "$\xa3x*$*x*-x*x-x**";
unsigned char REEL3[]		= "\xa3x*$*x*-x*x-x**$";

/**
 * Main entry point of game
 *
 * @param	na
 * @author	sbebbington
 * @date	26 Nov 2017
 * @version	1.0
 */
int main()
{
	zx80Init();
	titleScreen();
	gets(_strBuffer);
	randomise();
	startGame();
	pounds = 500;
	playAgain();
	return 0;
}

/**
 *
 */
void playAgain()
{
	prompt("Y to play again", 2);
	gets(_strBuffer);
	if(_strBuffer[0] == 121)
	{
		main();
	}
}

/**
 * Show title screen
 *
 * @param	na
 * @author	sbebbington
 * @date	26 Nov 2017
 * @version	1.0
 */
void titleScreen()
{
	printSpc(7, "donkeysoft  mmxvii\n\n");
	printSpc(4, "and monument  microgames\n\n");
	printSpc(12, "presents\n\n");
	printSpc(10, "QuIcK FrUiTs\n\nyou start with");
	printTab(1,"\xa3");
	printf("5.00\neach spin costs");
	printSpc(3,"\xa3" "0.25\nWIN TABLE:\n");
	printTab(2,"\xa3 \xa3 \xa3 = \xa3" "10.00\n");
	printTab(2, "$ $ $ = £7.50\n");
	printTab(2, "x x x = £4.00\n");
	printTab(2, "* * * = £2.00\n");
	printTab(2, "? ? - = £1.00\n");
	printTab(2, "? - ? = £?.??\n");
	printTab(2, "- - - = £ZERO\n");
	prompt("press any key to play", 2);
}

/**
 *
 */
void startGame()
{
	unsigned char _reel, pence = 0;
	while(pounds)
	{
		cls();
		pounds -= 25;
		winLine[0] = setReel(0);
		winLine[1] = setReel(1);
		winLine[2] = setReel(2);
		printf("%c %c %c", winLine[0], winLine[1], winLine[2]);
		pounds += checkReels(winLine[0], winLine[1], winLine[2]);
		pence = pounds % 100;
		printf("\nMONEY REMAINING \xa3%d.%d", pounds / 100, pence);
		if(!pence)
		{
			printf("0");
		}
		prompt("", 2);
		
		gets(_strBuffer);
	}
}

/**
 *
 */
unsigned char setReel(unsigned char reel)
{
	unsigned char reelPos = 0;
	randomise();
	if(!reel)
	{
		reelPos = REEL1[srand(random) % REEL];
	}
	else if(reel == 1)
	{
		reelPos = REEL2[srand(random) % REEL];
	}
	else
	{
		reelPos = REEL3[srand(random) % REEL];
	}
	return reelPos;
}

/**
 *
 */
unsigned short checkReels(unsigned char reel1, unsigned char reel2, unsigned char reel3)
{
	unsigned short winnings = 0;
	unsigned char pennies = 0;
	if(reel1 != 45){
		if(reel1 == reel2 && reel1 == reel3)
		{
			if(reel1 == 0xa3)
			{
				winnings = 1000;
			}
			if(reel1 == 36)
			{
				winnings = 750;
			}
			if(reel1 == 120)
			{
				winnings = 400;
			}
			if(reel1 == 42)
			{
				winnings = 200;
			}
		}
		if(reel1 == reel2 && reel2 != reel3)
		{
			winnings = 100;
		}
		if(reel1 != reel2 && reel1 == reel3)
		{
			randomise();
			winnings = 25;
			winnings *= srand(random) % 7;
			if(!winnings || !(random % 16))
			{
				winnings += 75;
			}
		}
		if(winnings)
		{
			pennies = winnings % 100;
			printf("\nyou win \xa3%d.%d", winnings / 100, pennies);
			if(!pennies)
			{
				printf("0");
			}
			printf("\n");
		}
	}
	return winnings;
}

/**
 * This does the basic initialisation
 * for the game and for the ZX80 itself
 *
 * @param	na
 * @author	sbebbington
 * @date	20 Aug 2017
 * @version	1.0
 */
void zx80Init()
{
	unsigned char y;
	for(y = 24; y > 0; y--)
	{
		printf("                                \n");
	}
	cls();
}

/**
 * Outputs the prompt, also accepts
 * a string and on which line the
 * prompt should appear
 *
 * @param	unsigned char, unsigned char
 * @author	sbebbington
 * @date	21 Aug 2017
 * @version	1.1
 */
void prompt(unsigned char txt[32], unsigned char lineNumber)
{
	if(lineNumber)
	{
		for(; lineNumber > 0; lineNumber--)
		{
			printf("\n");
		}
	}
	if(txt[0])
	{
		printf("%s\n",txt);
	}
	printf("c:>");
}

/**
 *
 */
void printSpc(unsigned char spc, unsigned char txt[31])
{
	for(; spc > 0; spc--)
	{
		printf(" ");
	}
	printf("%s", txt);
}

/**
 *
 */
void printTab(unsigned char tab, unsigned char txt[28])
{
	tab = tab * 4;
	printSpc(tab, txt);
}

/**
 * Clears the screen
 *
 * @param	na
 * @author	sbebbington
 * @date	22 Aug 2017
 * @version	1.1a
 */
void cls()
{
	__asm
	exx
	ld hl, ($400c)
	ld bc, $0300
	ld d, $21
	inc l
	CLS:
		dec d
		jr z, NEWLINE
		ld (hl), $00
	DECC:
		inc hl
		dec c
		jr z, DECB
		jr CLS
	DECB:
		dec b
		jr z, EXIT
		jr CLS
	NEWLINE:
		ld (hl), $76
		ld d, $21
		jr DECC
	EXIT:
	call $0747
	exx
	__endasm;
}

/**
 * Hopefully this will be a
 * better randomiser
 *
 * @author	sbebbington
 * @date	27 Nov 2017
 * @version	1.0
 */
void randomise()
{
	__asm
	ld hl, ($401e)
	ld a, l
	ld (_random), a
	__endasm;
}
