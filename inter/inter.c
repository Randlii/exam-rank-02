/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarrete <mcarrete@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 15:09:01 by mcarrete          #+#    #+#             */
/*   Updated: 2020/01/27 20:29:02 by mcarrete         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

/*
**Este programa recibe DOS (y solo dos) argumentos y devuelve los carácteres
**comunes a las dos cadenas (sin duplicados)
**/

/*
** Esta funcion chequea que el char donde nos encontramos no haya salido ya en
** el argv[1]. Si es asi, returna 1 y se mueve al siguiente char de argv[1];
*/
int		find_repeated(char *rep , int i)
{
	int j;

	j = 0;
	while (rep[j] != '\0' && j < i)
	{
		if (rep[j] == rep[i])
			return (1);
		j++;
	}
	return (0);
}

/*
** Esta es la funcion principal, recorre el primer argv y compara cada caracter
** con todos los del segundo argv. Si es igual (pero no se ha repetido antes -
** para ello la funcion de arriba), devuelve el char.
*/
void	inter(int argc, char **argv)
{
	int i;
	int j;
	char *rep;

	rep = argv[1];
	i = 0;
	while (argv[1][i] != '\0' && argv[2][i] != '\0')
	{
		j = 0;
		while (argv[2][j] != '\0')
		{
			if (argv[1][i] == argv[2][j] && find_repeated(rep, i) != 1)
			{
				write(1, &argv[1][i], 1);
				break;
			}
			j++;
		}
		i++;
	}
	write(1, "\n", 1);
}


/*
** Este main llama a la funcion inter, y chequea si hay un numro distinto a
** 3 argumentos. Si es asi, devulve un salto de linea.
*/
int		main(int argc, char **argv)
{
	if (argc != 3)
	{
		write(1, "\n", 1);
		return (0);
	}
	else
		inter(argc, argv);
	return (0);
}

