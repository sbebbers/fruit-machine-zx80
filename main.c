#include <stdio.h>
#include <stdlib.h>

/**
 * Function prototypes
 */
void main();

/**
 * Game function prototypes
 */
unsigned short getWinningAmount(unsigned char reel1, unsigned char reel2, unsigned char reel3);
unsigned char setReel(unsigned char reel);
void startGame();
void endGame();
void titleScreen();
void playAgain();

/**
 * Base function prototypes
 */
void cls();
void printSpc(unsigned char spc, unsigned char txt[31]);
void printTab(unsigned char tab, unsigned char txt[28]);
void printCurrency(unsigned short amount, unsigned char newLine);
void prompt(unsigned char txt[32], unsigned char lineNumber);
void randomise();
void zx80Init();

/**
 * Reusable/API variables
 */
unsigned char stringBuffer[33];
unsigned char random;
unsigned char i;

/**
 * Game variables
 */
unsigned short pounds, bank;
unsigned char winLine[]	=
{
	45, 45, 45
};

/**
 * Game constants
 */
unsigned char REEL		= 16;
unsigned short SPINCOST	= 25;

unsigned char REEL1[]	= "*$\xa3x*$*x*-x*x-x*";
unsigned char REEL2[]	= "$\xa3x*$*x*-x*x-x**";
unsigned char REEL3[]	= "\xa3x*$*x*-x*x-x**$";

/**
 * Main entry point of game
 *
 * @param	na
 * @author	sbebbington
 * @date	26 Nov 2017
 * @version	1.0
 */
void main()
{
	zx80Init();
	titleScreen();
}

/**
 * Prompts player, y restarts game,
 * any other entry exits it
 *
 * @author	sbebbington
 * @date	3 Dec 2017
 * @version	1.0
 * @todo	Handle the user inputs better
 */
void playAgain()
{
	if(!bank)
	{
		printf("unfortunately your money is\nspent. the management do not\ngive credit. we welcome back\npaying customers who enter R\nand press RETURN, otherwise\nplease move along.");
	}
	else
	{
		printf("you have banked some monies in\nenter the amount that you would\nlike to re-invest in our superb\nfruit machine or enter R to\nreturn to the title screen\n");
	}
	prompt("", 2);
	gets(stringBuffer);
	if(stringBuffer[0] == 121)
	{
		titleScreen();
	}
	endGame();
}

/**
 * Show title screen
 *
 * @param	na
 * @author	sbebbington
 * @date	4 Dec 2017
 * @version	1.0
 */
void titleScreen()
{
	cls();
	printSpc(7, "donkeysoft  mmxvii\n\n");
	printSpc(4, "and monument  microgames\n\n");
	printSpc(12, "presents\n\n");
	printSpc(10, "QuIcK FrUiTs++\n\nyou start with");
	printTab(1, "\xa3");
	printf("5.00\neach spin costs");
	printSpc(3, "\xa3" "0.25\nWIN TABLE:\n");
	printTab(2, "\xa3 \xa3 \xa3 pays out \xa3" "10.00\n");
	printTab(2, "$ $ $ pays out  \xa3" "7.50\n");
	printTab(2, "x x x pays out  \xa3" "4.00\n");
	printTab(2, "* * * pays out  \xa3" "2.00\n");
	printTab(2, "? ? - pays out  \xa3" "1.00\n");
	printTab(2, "? - ? pays out  \xa3" "?.??\n");
	printTab(2, "- - - pays out  \xa3" "ZERO\n");
	prompt("press the ANY KEY to play", 2);
	gets(stringBuffer);
	startGame();
}

/**
 * Starts the reels spinning,
 * continues until GBP is 0
 *
 * @author	sbebbington
 * @date	4 Dec 2017
 * @version	1.0
 */
void startGame()
{
	unsigned char _reel;
	unsigned short favourComputer;
	pounds = 500;
	bank = 0;
	restart:
	while(pounds)
	{
		cls();
		favourComputer = 0;
		pounds -= SPINCOST;

		winLine[0] = setReel(0);
		winLine[1] = setReel(1);
		winLine[2] = setReel(2);

		printf("#######\n");
		printf("#%c#%c#%c#\n", winLine[0], winLine[1], winLine[2]);
		printf("####\"\"#\n");
		printf("#######\n");

		favourComputer = getWinningAmount(winLine[0], winLine[1], winLine[2]);
		if(favourComputer && (!random % 5 || !random))
		{
			while(favourComputer)
			{
				winLine[1] = setReel(1);
				winLine[2] = setReel(2);
				favourComputer = getWinningAmount(winLine[0], winLine[1], winLine[2]);
				randomise();
			}
		}
		if(favourComputer > 0)
		{
			printf("\nYOU WIN ");
			printCurrency(favourComputer, 1);
			bank += favourComputer;
		}
		
		printf("\ncurrently banked ");
		printCurrency(bank, 1);

		printf("\nMONEY REMAINING ");
		printCurrency(pounds, 0);

		prompt("", 2);
		gets(stringBuffer);
	}
	playAgain(bank);
}

/**
 * The end is nigh
 *
 * @author	sbebbington
 * @date	5 Dec 2017
 */
void endGame()
{
	cls();
	prompt("THANKS FOR PLAYING", 1);
	gets(stringBuffer);
	zx80Init();
}

/**
 * Sets each reel position
 *
 * @param	unsigned char
 * @author	sbebbington
 * @date	4 Dec 2017
 * @version	1.0
 * @return	unsigned char
 */
unsigned char setReel(unsigned char reel)
{
	unsigned char reelPos = 0;
	randomise();
	if(!reel)
	{
		reelPos = REEL1[random % REEL];
	}
	else if(reel == 1)
	{
		reelPos = REEL2[random % REEL];
	}
	else
	{
		reelPos = REEL3[random % REEL];
	}
	if(!random % 8)
	{
		setReel(reel);
	}
	return reelPos;
}

/**
 * Checks to see if reels have
 * matched
 *
 * @param	unsigned char, unsigned char, unsigned char
 * @author	sbebbington
 * @date	26 Nov 2017
 * @version	1.0
 * @return	unsigned short
 */
unsigned short getWinningAmount(unsigned char reel1, unsigned char reel2, unsigned char reel3)
{
	unsigned short winnings = 0;
	if(reel1 != 45)
	{
		if(reel1 == reel2 && reel1 == reel3)
		{
			if(reel1 == 163)
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
			winnings = SPINCOST;
			winnings *= random % 7;
			if(!winnings)
			{
				winnings += 75;
			}
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
	for(i = 24; i > 0; i--)
	{
		printf("\n");
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
 * Simulates the BASIC PRINT SPC(x);"TEST"
 * function
 *
 * @param	unsigned char, unsigned char[]
 * @author	sbebbington
 * @date	27 Nov 2017
 * @version	1.0
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
 * Assumes 4 spaces is one tab (PRINT TAB(x);"TEST")
 *
 * @param	unsigned char, unsigned char[]
 * @author	sbebbington
 * @date	27 Nov 2017
 * @version	1.0
 */
void printTab(unsigned char tab, unsigned char txt[28])
{
	tab *= 4;
	printSpc(tab, txt);
}

/**
 * Prints the amount to two decimal places
 *
 * @param	unsigned short amount
 * @authod	sbebbington
 * @date	14 Dec 2017
 * @version 1.0
 */
void printCurrency(unsigned short amount, unsigned char newLine)
{
	printf("\xa3%d.%d", amount / 100, amount % 100);
	if(amount % 100 == 0)
	{
		printf("0");
	}
	if(newLine)
	{
		printf("\n");
	}
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
