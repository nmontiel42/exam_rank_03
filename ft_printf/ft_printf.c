#include <unistd.h>
#include <stdarg.h>

void	put_string(char *string, int *len)
{
	if (!string)
		string = "(null)";
	while (*string)
		*len += write(1, string++, 1);
}

void	put_digit(long long int num, int base, int *len)
{
	char	*hexa = "0123456789abcdef";

	if (num < 0)
	{
		num *= -1;
		*len += write(1, "-", 1);
	}
	if (num >= base)
		put_digit((num / base), base, len);
	*len += write(1, &hexa[num % base], 1);
}

int	ft_printf(const char *format, ...)
{
	int len = 0;

	va_list pointer;
	va_start(pointer, format);

	while (*format)
	{
		if ((*format == '%') && ((*(format + 1) == 's') || (*(format + 1) == 'd') || (*(format + 1) == 'x')))
		{
			format++;
			if (*format == 's')
				put_string(va_arg(pointer, char *), &len);
			else if (*format == 'd')
				put_digit((long long int)va_arg(pointer, int), 10, &len);
			else if (*format == 'x')
				put_digit((long long int)va_arg(pointer, unsigned int), 16, &len);
		}
		else
			len += write(1, format, 1);
		format++;
	}
	return (va_end(pointer), len);
}
