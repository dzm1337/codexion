/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 03:12:05 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 03:12:38 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static int	is_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	validate_numbers(char **argv)
{
	if (!is_number(argv[1]) || !is_number(argv[2]) || !is_number(argv[3])
		|| !is_number(argv[4]) || !is_number(argv[5]) || !is_number(argv[6])
		|| !is_number(argv[7]))
		return (0);
	return (1);
}

static int	assign_values(t_data *data, char **argv)
{
	data->number_of_coders = atoi(argv[1]);
	data->time_to_burnout = ft_atol(argv[2]);
	data->time_to_compile = ft_atol(argv[3]);
	data->time_to_debug = ft_atol(argv[4]);
	data->time_to_refactor = ft_atol(argv[5]);
	data->number_of_compiles_required = atoi(argv[6]);
	data->dongle_cooldown = ft_atol(argv[7]);
	if (data->number_of_coders <= 0
		|| data->time_to_burnout <= 0
		|| data->time_to_compile <= 0
		|| data->time_to_debug <= 0
		|| data->time_to_refactor <= 0
		|| data->number_of_compiles_required <= 0
		|| data->dongle_cooldown < 0)
		return (0);
	return (1);
}

int	parse_args(t_data *data, int argc, char **argv)
{
	if (argc != 9)
		return (0);
	if (!validate_numbers(argv))
		return (0);
	if (!assign_values(data, argv))
		return (0);
	if (strcmp(argv[8], "fifo") == 0)
		data->scheduler = MY_FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		data->scheduler = MY_EDF;
	else
		return (0);
	return (1);
}
