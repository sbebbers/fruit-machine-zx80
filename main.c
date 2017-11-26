#include <stdio.h>
#include <stdlib.h>
#include "zx80s.h"

/**
 * Function prototypes
 */
int main();

void titleScreen();
void startGame();
void zx80Init();
void cls();

unsigned char prompt(unsigned char txt[32], unsigned char lineNumber);
unsigned char printAt(unsigned short xy);
unsigned char setText(unsigned char txt[33], unsigned char x, unsigned char y, unsigned char inv);

/**
 * Reusable variables
 */
unsigned char _strBuffer[33];
unsigned char text[33];

/**
 * Game variables
 */
unsigned float money	= 5.00f;
unsigned char reel		= 20;
unsigned char entropy	= 1;
unsigned char winLine[3]= "   ";
/**
 * Game constants
 */
unsigned float SPINCOST		= 0.25f;
unsigned float BONUS		= 0.50f;
unsigned float WINNING[5]	=
{
	1.00f, 2.00f, 4.00f, 7.50f, 10.00f
};
unsigned char REELS[26]		= "*$£-x $ $*0- -* ox*   x --";

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
	entropy += _strBuffer[0];
	startGame();
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
	printf("       donkeysoft  mmxvii\r\n    and monument  microgames\r\n            presents\r\n          QuIcK FrUiTs");
	printf("\r\nyou start with £5.00 \r\neach spin costs £0.25p");
	prompt("press any key to play", 2);
}

void startGame()
{
	unsigned char _reel, i;
	while(1)
	{
		cls();
		for(i = 3; i > 0; i--)
		{
			entropy += rand()%16;
			_reel = srand(srand(entropy)) % 26;
			winLine[i] = REELS[_reel];
			printf("%c ", winLine[i]);
		}
		prompt("",2);
		gets(_strBuffer);
	}
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
	text[0] = EOF;
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
unsigned char prompt(unsigned char txt[32], unsigned char lineNumber)
{
	unsigned char y;
	if(lineNumber)
	{
		for(y = lineNumber; y > 0; y--)
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
 * This sets the fast text output
 * by passing the already defined
 * ZX80 char array to the global
 * used in the printAt() function
 *
 * @param	na
 * @author	sbebbington
 * @date	26 Nov 2017
 * @version	1.0a
 */
unsigned char setText(unsigned char txt[33], unsigned char x, unsigned char y, unsigned char inv)
{
	unsigned char c = 0;
	while(txt[c] != EOF)
	{
		if(inv)
		{
			text[c] = INVERSE(txt[c]);
		}
		else
		{
			text[c] = txt[c];
		}
		c++;
	}
	text[c] = EOF;
	printAt(SCRLOC(x,y));
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
	ld hl,($400c)
	ld bc,$0300
	ld d,$21
	inc l
	CLS:
		dec d
		jr z,NEWLINE
		ld (hl),$00
	DECC:
		inc hl
		dec c
		jr z,DECB
		jr CLS
	DECB:
		dec b
		jr z,EXIT
		jr CLS
	NEWLINE:
		ld (hl),$76
		ld d,$21
		jr DECC
	EXIT:
	call $0747
	exx
	__endasm;
}

/**
 * At last, the fast printAt function works
 * does a look-up on the text global above
 * to work out which character or string
 * to write to the DFILE and at what position
 *
 * @param	unsigned char, unsigned char
 * @author	sbebbington
 * @date	22 Aug 2017
 * @version	1.2b
 */
unsigned char printAt(unsigned short xy) __z88dk_fastcall
{
	__asm
	ld b,h
	ld c,l
	ld hl,($400c)
	inc l
	add hl,bc
	ld bc,_text
	CHAROUT:
		ld a,(bc)
		cp $ff
		jr z,RETURN
		ld (hl),a
		inc bc
		inc hl
		jr CHAROUT
	RETURN:
	__endasm;
}
