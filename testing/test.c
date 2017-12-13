#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* itoa(int value, char* buffer, int radix)
{
	sprintf(buffer, "%i", value);
	return buffer;
}

int main()
{
	char buffer[124];
	memset(buffer, 0, sizeof(buffer));

	puts(itoa(0, buffer, 10));
	puts(itoa(1, buffer, 10));
	puts(itoa(9, buffer, 10));
	puts(itoa(10, buffer, 10));
	puts(itoa(11, buffer, 10));
	puts(itoa(99, buffer, 10));
	puts(itoa(100, buffer, 10));
	puts(itoa(101, buffer, 10));
	puts(itoa(109, buffer, 10));
	puts(itoa(110, buffer, 10));
	puts(itoa(111, buffer, 10));
	puts(itoa(999, buffer, 10));
	puts(itoa(1000, buffer, 10));
	puts(itoa(-0, buffer, 10));
	puts(itoa(-1, buffer, 10));
	puts(itoa(-9, buffer, 10));
	puts(itoa(-10, buffer, 10));
	puts(itoa(-11, buffer, 10));
	puts(itoa(-99, buffer, 10));
	puts(itoa(-100, buffer, 10));
	puts(itoa(-101, buffer, 10));
	puts(itoa(-109, buffer, 10));
	puts(itoa(-110, buffer, 10));
	puts(itoa(-111, buffer, 10));
	puts(itoa(-999, buffer, 10));
	puts(itoa(-1000, buffer, 10));
}
