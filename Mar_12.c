#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

void display(int n)
{
	if (n > 9)
	{
		display(n / 10);
	}
	putchar(n % 10 + '0');
}

void print(const char *format, ...)
{
	va_list arg;
	va_start(arg, format);
	while (*format)
	{
		switch (*format)
		{
		case 's':
		{
					char *ptr = va_arg(arg, char*);
					while (*ptr)
					{
						putchar(*ptr);
						ptr++;
					}
		}
			break;
		case 'd':
			display(va_arg(arg, int));
			break;
		case 'c':
			putchar(va_arg(arg, char));
			break;
		default:
			putchar(*format);
			break;
		}
		format++;
	}
	va_end(arg);
}

int main()
{
	print("s d c", "abcdef", 10, 'w');
	system("pause");
	return 0;
}