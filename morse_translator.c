/*
 * XHYPER270A
 * lcd_test.c
 * 2008-03-05   <newboder@hybus.net>
 * 
 */

#include	<stdio.h>
#include	<string.h>
#include	<fcntl.h>
#include	<sys/time.h>
#include	<termio.h>

#define  MAXCHR  32
#define  DEL_TIME 500


static char lcdDev[] = "/dev/CLCD";
static int  lcdFd = (-1);

void push_to_board(char);
char convert(char* );

main(int ac, char *av[])
{
	int n;
	char ch;
	char mos_ch[7];
	int mos_index = 0;
	char	buf[MAXCHR];
	struct timeval *before, *after;
	int delta;
	
	printf("Usage: \n");
	printf("\tDot(z)\n");
	printf("\tDash(x)\n");
	printf("\tSend character(enter)\n\n");

	memset(mos_ch, 0x0, sizeof(mos_ch));
	gettimeofday(before, NULL);

	while ( (ch = getch()) != -1 )
	{
		if (ch == 'z' || ch == 'x')
		{
			mos_ch[mos_index++] = ch;
		}

		if(ch == '\n' || mos_index >= 5)
		{
			if (mos_index == 0)
			{
				ch = ' ';
			}
			else
			{
				mos_ch[mos_index] = '\0';
				ch = convert(mos_ch);
			}
			printf("[%s] %c\n", mos_ch, ch);
			push_to_board(ch);

			memset(mos_ch, 0x0, sizeof(mos_ch));
			mos_index = 0;
		}
	}

	return 0;
}

void push_to_board(char ch)
{
	static char buf[MAXCHR];
	static int index = 0;
	static int lcdFd = 0;

	if (lcdFd == 0)
	{
		lcdFd = open(lcdDev, O_RDWR);
		if (lcdFd < 0) {
			fprintf(stderr, "cannot open %s\n", lcdDev);
			exit(2);
		}
	}

	if (index == MAXCHR-1)
	{
		index = 0;
		memset(buf, 0x00, sizeof(buf));
	} 

	buf[index++] = ch;
	buf[index] = '\0';

	write(lcdFd, buf, MAXCHR);
}

char convert(char* buf)
{
	char ret;
	if (!strcmp(buf, "zx")) ret = 'A';
	if (!strcmp(buf, "xzzz")) ret = 'B';
	if (!strcmp(buf, "xzxz")) ret = 'C';
	if (!strcmp(buf, "xzz")) ret = 'D';
	if (!strcmp(buf, "z")) ret = 'E';
	if (!strcmp(buf, "zzxz")) ret = 'F';
	if (!strcmp(buf, "xxz")) ret = 'G';
	if (!strcmp(buf, "zzzz")) ret = 'H';
	if (!strcmp(buf, "zz")) ret = 'I';
	if (!strcmp(buf, "zxxx")) ret = 'J';
	if (!strcmp(buf, "xzx")) ret = 'K';
	if (!strcmp(buf, "zxzz")) ret = 'L';
	if (!strcmp(buf, "xx")) ret = 'M';
	if (!strcmp(buf, "xz")) ret = 'N';
	if (!strcmp(buf, "xxx")) ret = 'O';
	if (!strcmp(buf, "zxxz")) ret = 'P';
	if (!strcmp(buf, "xxzx")) ret = 'Q';
	if (!strcmp(buf, "zxz")) ret = 'R';
	if (!strcmp(buf, "zzz")) ret = 'S';
	if (!strcmp(buf, "x")) ret = 'T';
	if (!strcmp(buf, "zzx")) ret = 'U';
	if (!strcmp(buf, "zzzx")) ret = 'V';
	if (!strcmp(buf, "zxx")) ret = 'W';
	if (!strcmp(buf, "xzzx")) ret = 'X';
	if (!strcmp(buf, "xzxx")) ret = 'Y';
	if (!strcmp(buf, "xxzz")) ret = 'Z';
	if (!strcmp(buf, "zxxxx")) ret = '1';
	if (!strcmp(buf, "zzxxx")) ret = '2';
	if (!strcmp(buf, "zzzxx")) ret = '3';
	if (!strcmp(buf, "zzzzx")) ret = '4';
	if (!strcmp(buf, "zzzzz")) ret = '5';
	if (!strcmp(buf, "xzzzz")) ret = '6';
	if (!strcmp(buf, "xxzzz")) ret = '7';
	if (!strcmp(buf, "xxxzz")) ret = '8';
	if (!strcmp(buf, "xxxxz")) ret = '9';
	if (!strcmp(buf, "xxxxx")) ret = '0';

	return ret;
}

int getch()
{
	int ch;
	struct termios buf, save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON|ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}

/* EOF */
