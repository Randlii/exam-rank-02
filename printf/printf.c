/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarrete <mcarrete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 08:43:36 by mcarrete          #+#    #+#             */
/*   Updated: 2020/02/03 19:27:40 by mcarrete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

unsigned long		ft_strlen(const char *s)
{
	unsigned long	i;

	i = 0;
	while (s[i] != 0)
		i++;
	return (i);
}

void				ft_putnbr_base(int n, int base)
{
	long int	nb;
	char		c;
	unsigned int rem;

	nb = n;
	if (nb < 0)
		nb = -nb;
	if (nb >= base)
		ft_putnbr_base(nb / base, base);
	rem = nb % base;
	c = rem < 10 ? rem + '0' : rem + 'W';
	write(1, &c, 1);
}

static int			ft_len_calc(unsigned long int nb, unsigned long int base) //cuidado con el tipo!!!!
{
	int i;

	i = 0;
	while (nb >= base)
	{
		nb = nb / base;
		i++;
	}
	i++;
	return (i);
}

int			ft_putspaceandzero(int len, int precision, int width, int is_negative)
{
	int i;
	int num;
	precision = len > precision ? len : precision;
	width = width > precision ? width : precision;
	i = 0;
	num = 0;
	while (i < (width - precision))
	{
		write(1, " ", 1);
		i++;
		num++;
	}
	if (is_negative == 1)
	{
		write(1,"-", 1); //para q qede despues de los espacios y antes de los 0
		num++;
	}
	i = 0;
	while (i < (precision - len))
	{
		write(1, "0", 1);
		i++;
		num++;
	}
	return (num);
}

int			str_output(int ret, int width, int precision, va_list args)
{
	char	*str;
	int		len;
	int i;

	str = va_arg(args, char *);
	if (str == NULL)
		str = "(null)";
	len = ft_strlen(str);
	len = (precision >= 0 && precision < len) ? precision : len;
	width = width > len ? (width - len) : 0; //si width no es mayor q len entonces se imprimen 0 espacios
	i = 0;
	while (i < width)
	{
		write(1, " ", 1);
		i++;
		ret++;
	}
	i = 0;
	while (i < len)
	{
		write(1, &str[i], 1);
		i++;
		ret++;
	}
	return (ret);
}

int			hex_output(int ret, int width, int precision, va_list args)
{
	unsigned long int	hex_arg; // cuidado el tipo
	int					len;
	int					is_null;

	is_null = 0;
	hex_arg = va_arg(args, unsigned int);
	if (precision == 0 && hex_arg == 0)
		is_null = -1;
	len = is_null == -1 ? 0 : ft_len_calc(hex_arg, 16);
	ret += ft_putspaceandzero(len, precision, width, 0);
	if (is_null != -1)
		ft_putnbr_base(hex_arg, 16);
	ret += len; //xq ya le dijimos que es el max.
	return (ret);
}

int			int_output(int ret, int width, int precision, va_list args)
{
	int	len;
	long int	arg;  //tipo!!!!
	int			is_negative;
	int			is_null;

	is_negative = 0;
	is_null = 0;
	arg = (long int)va_arg(args, int); //cuidado los tipos!!!!!
	if (precision == 0 && arg == 0)
		is_null = -1;
	if (arg < 0)
	{
		is_negative = 1;
		arg *= -1; //para que ya no se pase como negativo.
		width += -1; //para que no cuente el menos!!
	}
	len = is_null == -1 ? 0 : ft_len_calc(arg, 10); //si precision es 0 y el arg es 0, no imprime nada, osea len = 0
	ret = ret + ft_putspaceandzero(len, precision, width, is_negative);// este imprime el ' ', el '-' y el 0
	if (is_null != -1)
		ft_putnbr_base(arg, 10); //no imprime el -
	ret += len;
	return (ret);
}

int			ft_printf(const char *str, ...)
{
	int			ret;
	int			width;
	int			precision;
	int			i;
	va_list		args;

	ret = 0;
	width = 0;
	precision = -1;
	i = 0;
	va_start(args, str);
	while (str[i] != '\0')
	{
		if (str[i] != '%' && str[i])
			ret += write(1, &str[i], 1);
		if (str[i] == '%')
		{
			width = 0;
			precision = -1;
			i++;
			while (str[i] >= '0' && str[i] <= '9')
			{
				width = width * 10 + (str[i] - '0');
				i++;
			}
			if (str[i] == '.')
			{
				i++;
				precision = 0;
				while (str[i] >= '0' && str[i] <= '9')
				{
					precision = precision * 10 + (str[i] - '0');
					i++;
				}
			}
			if (str[i] == 'd')
				ret = int_output(ret, width, precision, args);
			else if (str[i] == 'x')
				ret = hex_output(ret, width, precision, args);
			else if (str[i] == 's')
				ret = str_output(ret, width, precision, args);
		}
		i++;
	}
	if (ret <= 0) //error
		write(1, "", 0);
	va_end(args);
	return (ret);
}

/*
int		main()
{
	int				one;
	int				four;
	char			A;
	char			*str;
	float			e;
	unsigned int	un;
	int				*ptr;
	int				starwidth;
	int				starprec;
	int				original_ret;
	int				my_ret;

	starprec = 3;
	starwidth = 7;
	ptr = &four;
	un = 10;
	e = 3.1415;
	A = 'A';
	str = "hi low";
	one = 10;
	four = 0;



	my_ret = ft_printf("%10.5d", -216);
	original_ret = printf("%10.5d", -216);
	printf("\nMy ret: %d\n Original_ret %d\n", my_ret, original_ret);



	int x = 0;
	int y = 0;

	char *s = "marina";
    int d = -2147483648;
    int hex = 1583;

	x = printf("hola|%10.10d|%15.14d|%4.30d|%0.d|%.0d|\n", d, d, d, d, 0);
	printf("%d\n", x);

	y = ft_printf("hola|%10.10d|%15.14d|%4.30d|%0.d|%.0d|\n", d, d, d, d, 0);
	ft_printf("%d\n", y);


    x = printf("hola|%10.10x|%15.14x|%8.15x|%0.x|%.x|\n", hex, hex, hex, hex, 0);
    printf("%d\n", x);

    y = ft_printf("hola|%10.10x|%15.14x|%8.15x|%0.x|%.x|\n", hex, hex, hex, hex, 0);
    ft_printf("%d\n", y);

    x = printf("hola|%10s|%15.14s|%8.10s|%.s|%.4s\n", s, s, s, s, NULL);
    printf("%d\n", x);

    y = ft_printf("hola|%10s|%15.14s|%8.10s|%.s|%.4s\n", s, s, s, s, NULL);
    ft_printf("%d\n", y);


	my_ret = ft_printf("d0w %0d %0d %0d %0d %0d %0d %0d %0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	printf("\n");
	original_ret = printf("d0w %0d %0d %0d %0d %0d %0d %0d %0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	printf("\n");
	printf("\nMy ret: %d\nOriginal_ret %d\n", my_ret, original_ret);


	//ft_printf("MY FT_PRINTF:\nThis is my first int: %d\nAnd this is my second int: %d\n\n", one, four);
	//printf("PRINTF:\nThis is my first int: %d\nAnd this is my second int: %d\n", one, four);

	//ft_printf("MY FT_PRINTF:\nThis is my int: %d\nAnd this is my char: %c\n\n", one, A);
	//printf("PRINTF:\nThis is my int: %d\nAnd this is my char: %c\n\n", one, A);

	//ft_printf("\n MYPRINTF:\nDecimal: %2dhola\nInt: %10i\nChar: %03c\nString: %12s\nFloat: %.8f\nUnsigned Int: %u\nHex: %#x\nHEX: %#X\nPointer: %p\nPercent: %3%\n\n", four, one, A, str, e, un, four,four, ptr);
	//printf("\nPRINTF:\nDecimal: %2dhola\nInt: %10i\nChar: %03c\nString: %10s\nFloat: %.8f\nUnsigned Int: %u\nHex: %#x\nHEX: %#X\nPointer: %p\nPercent: %3%\n\n", four, one, A, str, e, un, four,four, ptr);

}


int main (void)
{
    int    r;
    r = 0;
    ft_printf("Simple test\n");
    ft_printf("");
    ft_printf("--Format---");
    ft_printf("\n");
    ft_printf("%d", 0);
    ft_printf("%d", 42);
    ft_printf("%d", 1);
    ft_printf("%d", 5454);
    ft_printf("%d", (int)2147483647);
    ft_printf("%d", (int)2147483648);
    ft_printf("%d", (int)-2147483648);
    ft_printf("%d", (int)-2147483649);
    ft_printf("\n");
    ft_printf("%x", 0);
    ft_printf("%x", 42);
    ft_printf("%x", 1);
    ft_printf("%x", 5454);
    ft_printf("%x", (int)2147483647);
    ft_printf("%x", (int)2147483648);
    ft_printf("%x", (int)-2147483648);
    ft_printf("%x", (int)-2147483649);
    ft_printf("%s", "");
    ft_printf("%s", "toto");
    ft_printf("%s", "wiurwuyrhwrywuier");
    ft_printf("%s", NULL);
    ft_printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL);
    ft_printf("\n--Mixed---\n");
    ft_printf("%d%x%d%x%d%x%d%x\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648);
    ft_printf("-%d-%x-%d-%x-%d-%x-%d-%x-\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648);
    ft_printf("\n");
    ft_printf("%s%s%s%s\n", "", "toto", "wiurwuyrhwrywuier", NULL);
    ft_printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL);
    ft_printf("--1 ft_printflag--\n");
    ft_printf("d0w %0d %0d %0d %0d %0d %0d %0d %0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d4w %4d %4d %4d %4d %4d %4d %4d %4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d10w %10d %10d %10d %10d %10d %10d %10d %10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d0p %.0d %.0d %.0d %.0d %.0d %.0d %.0d %.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d4p %.4d %.4d %.4d %.4d %.4d %.4d %.4d %.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d10p %.10d %.10d %.10d %.10d %.10d %.10d %.10d %.10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x0w %0x %0x %0x %0x %0x %0x %0x %0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x4w %4x %4x %4x %4x %4x %4x %4x %4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x10w %10x %10x %10x %10x %10x %10x %10x %10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x0p %.0x %.0x %.0x %.0x %.0x %.0x %.0x %.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x4p %.4x %.4x %.4x %.4x %.4x %.4x %.4x %.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x10p %.10x %.10x %.10x %.10x %.10x %.10x %.10x %.10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("s0p ~%.0s` ~%.0s` ~%.0s` ~%.0s` ~%.0s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("s4w ~%4s` ~%4s` ~%4s` ~%4s` ~%4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("s4p ~%.4s` ~%.4s` ~%.4s` ~%.4s` ~%.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("s10w ~%10s` ~%10s` ~%10s` ~%10s` ~%10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("s10p ~%.10s` ~%.10s` ~%.10s` ~%.10s` ~%.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("--2 ft_printflags--\n");
    ft_printf("d0w0p %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x0w0p %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("--Precision--\n");
    ft_printf("d0w4p %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d0w10p %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x0w4p %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x0w10p %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("--Width--\n");
    ft_printf("d4w0p %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d10w0p %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x4w0p %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x10w0p %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("s4w0p ~%4.s` ~%4.s` ~%4.s` ~%4.s` ~%4.s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("s10w0p ~%10.0s` ~%10.0s` ~%10.0s` ~%10.0s` ~%10.0s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("--Width and Precision--\n");
    ft_printf("d10w4p %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d10w10p %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d\n", 0, 1, 42, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d4w4p %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("d4w10p %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x10w4p %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x10w10p %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x\n", 0, 1, 42, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x4w4p %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("x4w10p %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    ft_printf("s10w4p ~%10.4s` ~%10.4s` ~%10.4s` ~%10.4s` ~%10.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("s10w10p ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("s4w4p ~%4.4s` ~%4.4s` ~%4.4s` ~%4.4s` ~%4.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    ft_printf("s4w10p ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
}
    //  printf("written: %d\n", r);

 //printf("%s", "---------------------");
*/

int main (void)
{
    int    r;
    r = 0;
    printf("Simple test\n");
    printf("");
    printf("--Format---");
    printf("\n");
    printf("%d", 0);
    printf("%d", 42);
    printf("%d", 1);
    printf("%d", 5454);
    printf("%d", (int)2147483647);
    printf("%d", (int)2147483648);
    printf("%d", (int)-2147483648);
    printf("%d", (int)-2147483649);
    printf("\n");
    printf("%x", 0);
    printf("%x", 42);
    printf("%x", 1);
    printf("%x", 5454);
    printf("%x", (int)2147483647);
    printf("%x", (int)2147483648);
    printf("%x", (int)-2147483648);
    printf("%x", (int)-2147483649);
    printf("%s", "");
    printf("%s", "toto");
    printf("%s", "wiurwuyrhwrywuier");
    printf("%s", NULL);
    printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL);
    printf("\n--Mixed---\n");
    printf("%d%x%d%x%d%x%d%x\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648);
    printf("-%d-%x-%d-%x-%d-%x-%d-%x-\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648);
    printf("\n");
    printf("%s%s%s%s\n", "", "toto", "wiurwuyrhwrywuier", NULL);
    printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL);
    printf("--1 ft_printflag--\n");
    printf("d0w %0d %0d %0d %0d %0d %0d %0d %0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d4w %4d %4d %4d %4d %4d %4d %4d %4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d10w %10d %10d %10d %10d %10d %10d %10d %10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d0p %.0d %.0d %.0d %.0d %.0d %.0d %.0d %.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d4p %.4d %.4d %.4d %.4d %.4d %.4d %.4d %.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d10p %.10d %.10d %.10d %.10d %.10d %.10d %.10d %.10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x0w %0x %0x %0x %0x %0x %0x %0x %0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x4w %4x %4x %4x %4x %4x %4x %4x %4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x10w %10x %10x %10x %10x %10x %10x %10x %10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x0p %.0x %.0x %.0x %.0x %.0x %.0x %.0x %.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x4p %.4x %.4x %.4x %.4x %.4x %.4x %.4x %.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x10p %.10x %.10x %.10x %.10x %.10x %.10x %.10x %.10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("s0p ~%.0s` ~%.0s` ~%.0s` ~%.0s` ~%.0s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("s4w ~%4s` ~%4s` ~%4s` ~%4s` ~%4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("s4p ~%.4s` ~%.4s` ~%.4s` ~%.4s` ~%.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("s10w ~%10s` ~%10s` ~%10s` ~%10s` ~%10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("s10p ~%.10s` ~%.10s` ~%.10s` ~%.10s` ~%.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("--2 ft_printflags--\n");
    printf("d0w0p %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x0w0p %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("--Precision--\n");
    printf("d0w4p %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d0w10p %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x0w4p %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x0w10p %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("--Width--\n");
    printf("d4w0p %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d10w0p %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x4w0p %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x10w0p %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("s4w0p ~%4.s` ~%4.s` ~%4.s` ~%4.s` ~%4.s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("s10w0p ~%10.0s` ~%10.0s` ~%10.0s` ~%10.0s` ~%10.0s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("--Width and Precision--\n");
    printf("d10w4p %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d10w10p %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d\n", 0, 1, 42, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d4w4p %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("d4w10p %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x10w4p %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x10w10p %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x\n", 0, 1, 42, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x4w4p %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("x4w10p %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
    printf("s10w4p ~%10.4s` ~%10.4s` ~%10.4s` ~%10.4s` ~%10.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("s10w10p ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("s4w4p ~%4.4s` ~%4.4s` ~%4.4s` ~%4.4s` ~%4.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
    printf("s4w10p ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
}
