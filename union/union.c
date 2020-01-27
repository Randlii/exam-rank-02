/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   union.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarrete <mcarrete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 22:33:00 by mcarrete          #+#    #+#             */
/*   Updated: 2020/01/27 23:12:34 by mcarrete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/*
** Usamos esta funcion para ver si el char (argv[2][i]) se encuentra en el
** argv[1]
*/

int		find_char(char c, char *str)
{
	int j;

	j = 0;
	while(str[j] != '\0')
	{
		if (str[j] == c)
			return (1);
		j++;
	}
	return (0);
}

/*
** Esta funcion chequea que el char donde nos encontramos no haya salido ya en
** el argv. Si es asi, returna 1 y se mueve al siguiente char de argv[1];
** la usamos en para chequear los dos args contra si mismos. (que no se
** repita el char en ese mismo argv).
*/

int		find_repeated(char c, char *str, int i)
{
	int j;

	j = 0;
	while(j < i && str[j] != '\0')
	{
		if (str[j] == c)
			return (1);
		j++;
	}
	return (0);
}

/*
** Esta es la funcion principal, recorre el primer argv y si no se ha repetido
** anteriormente (find_repeated), devuelve el char. Despues hace lo mismo con
** el segundo argv, pero ahora chequea si tampoco se repite en el primero
** (find_char).
*/

void	ft_union(char** argv)
{
	int i;

	i = 0;
	while (argv[1][i] != '\0')
	{
		if(!(find_repeated(argv[1][i], argv[1], i)))
			write(1, &argv[1][i], 1);
		i++;
	}
	i = 0;
	while (argv[2][i] != '\0')
	{
		if(!(find_repeated(argv[2][i], argv[2], i)) &&
		!(find_char(argv[2][i], argv[1])))
			write(1, &argv[2][i], 1);
		i++;
	}
	write(1, "\n", 1);
}

/*
** Este main llama a la funcion union, y chequea si hay un numero distinto a
** 3 argumentos. Si es asi, devulve un salto de linea.
*/

int		main(int argc, char **argv)
{
	if (argc != 3)
	{
		write(1, "\n", 1);
		return (1);
	}
	else
		ft_union(argv);
	return (0);
}
