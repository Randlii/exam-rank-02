/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarrete <mcarrete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 08:43:36 by mcarrete          #+#    #+#             */
/*   Updated: 2020/02/02 17:40:06 by mcarrete         ###   ########.fr       */
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

void				ft_putnbr(int n)
{
	long int	nb;
	char		c;

	nb = n;
	if (nb < 0)
		nb = -nb;
	if (nb > 9)
		ft_putnbr(nb / 10);
	c = nb % 10 + '0';
	write(1, &c, 1);
}

void	ft_puthex(unsigned int n) //intentar hacerlo para cualquier base
{
	char			c;
	unsigned int	rem;

	if (n > 15)
		ft_puthex(n / 16);
	rem = n % 16;
	c = rem < 10 ? (char)rem + '0' : (char)rem + 'W';
	write(1, &c, 1);
}

static int			ft_len_calc(unsigned int nb) //cuidado con el tipo!!!!
{
	int i;

	i = 0;
	while (nb > 9)
	{
		nb = nb / 10;
		i++;
	}
	i++;
	return (i);
}

static int			ft_len_calc_hex(unsigned long int nb) //cuidado con el tipo!!!!
{
	int i;

	i = 0;
	while (nb > 15)
	{
		nb = nb / 16;
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

int			hex_output(int ret, int width, int precision, va_list args)
{
	unsigned long int	hex_arg; // cuidado el tipo
	int					len;
	int					is_null;

	is_null = 0;
	hex_arg = va_arg(args, unsigned int);
	if (precision == 0 && hex_arg == 0)
		is_null = -1;
	len = ft_len_calc_hex(hex_arg);
	ret += ft_putspaceandzero(len, precision, width, 0);
	if (is_null != -1)
		ft_puthex(hex_arg);
	ret += len + is_null; //xq ya le dijimos que es el max.
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
	len = ft_len_calc(arg);
	ret = ret + ft_putspaceandzero(len, precision, width, is_negative);// este imprime el ' ', el '-' y el 0
	if (is_null != -1)
		ft_putnbr(arg); //no imprime el -
	ret += len + is_null;
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
		{
			ret += write(1, &str[i], 1);
			i++;
		}
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
			{
				ret = int_output(ret, width, precision, args);
				i++;
			}
			else if (str[i] == 'x')
			{
				ret = hex_output(ret, width, precision, args);
				i++;
			}
		/*
			else if (str[i] == 's')
			{
				ret = str_output(ret, width, precision, args);
				i++;
			}
		*/
		}

	}
	if (ret <= 0)
		write(1, "", 0);
	va_end(args);
	return (ret);
}

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

/*
	my_ret = ft_printf("%10.5d", -216);
	original_ret = printf("%10.5d", -216);
	printf("\nMy ret: %d\n Original_ret %d\n", my_ret, original_ret);
*/


	int x = 0;
	int y = 0;

	char *s = "GONZALO";
    int d = -2147483648;
    int hex = 1583;

	x = printf("hola|%10.10d|%15.14d|%4.30d|%0.d|%.0d|\n", d, d, d, d, 0);
	printf("%d\n", x);

	y = ft_printf("hola|%10.10d|%15.14d|%4.30d|%0.d|%.0d|\n", d, d, d, d, 0);
	ft_printf("%d\n", y);


    x = printf("hola|%10.10x|%15.14x|%8.15x|%0.x|%.x|\n", d, d, d, d, 0);
    printf("%d\n", x);

    y = ft_printf("hola|%10.10x|%15.14x|%8.15x|%0.x|%.x|\n", d, d, d, d, 0);
    ft_printf("%d\n", y);
/*
    x = printf("hola|%10s|%15.14s|%8.10s|%.s|%.4s\n", s, s, s, s, NULL);
    printf("%d\n", x);

    y = ft_printf("hola|%10s|%15.14s|%8.10s|%.s|%.4s\n", s, s, s, s, NULL);
    ft_printf("%d\n", y);
*/

/*
	my_ret = ft_printf("hola|%15.14d|\n", d);
	original_ret = printf("hola|%15.14d|\n", d);
	printf("\nMy ret: %d\nOriginal_ret %d\n", my_ret, original_ret);
*/

	//ft_printf("MY FT_PRINTF:\nThis is my first int: %d\nAnd this is my second int: %d\n\n", one, four);
	//printf("PRINTF:\nThis is my first int: %d\nAnd this is my second int: %d\n", one, four);

	//ft_printf("MY FT_PRINTF:\nThis is my int: %d\nAnd this is my char: %c\n\n", one, A);
	//printf("PRINTF:\nThis is my int: %d\nAnd this is my char: %c\n\n", one, A);

	//ft_printf("\n MYPRINTF:\nDecimal: %2dhola\nInt: %10i\nChar: %03c\nString: %12s\nFloat: %.8f\nUnsigned Int: %u\nHex: %#x\nHEX: %#X\nPointer: %p\nPercent: %3%\n\n", four, one, A, str, e, un, four,four, ptr);
	//printf("\nPRINTF:\nDecimal: %2dhola\nInt: %10i\nChar: %03c\nString: %10s\nFloat: %.8f\nUnsigned Int: %u\nHex: %#x\nHEX: %#X\nPointer: %p\nPercent: %3%\n\n", four, one, A, str, e, un, four,four, ptr);

}



