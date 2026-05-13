/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:28:14 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 00:35:51 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

int	parse_args(t_data *data, int argc, char **argv)
{
	if (argc != 9)
		return (0);
	data->number_of_coders = atoi(argv[1]);
	data->time_to_burnout = ft_atol(argv[2]);
	data->time_to_compile = ft_atol(argv[3]);
	data->time_to_debug = ft_atol(argv[4]);
	data->time_to_refactor = ft_atol(argv[5]);
	data->number_of_compiles_required = atoi(argv[6]);
	data->dongle_cooldown = ft_atol(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		data->scheduler = MY_FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		data->scheduler = MY_EDF;
	else
		return (0);
	if (data->number_of_coders <= 0 || data->time_to_burnout <= 0
		|| data->time_to_compile <= 0 || data->time_to_debug <= 0
		|| data->time_to_refactor <= 0 || data->number_of_compiles_required <= 0
		|| data->dongle_cooldown < 0)
		return (0);
	return (1);
}

static int	init_dongles_and_mutex(t_data *data)
{
	data->stop = 0;
	data->start_time = get_current_time();
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);
	if (data->scheduler == MY_FIFO)
		data->dongles = init_dongles_fifo(data->number_of_coders);
	else
		data->dongles = init_dongles_edf(data->number_of_coders);
	if (!data->dongles)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (0);
	}
	return (1);
}

static int	init_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
	{
		free_dongles(data->dongles, data->number_of_coders);
		pthread_mutex_destroy(&data->print_mutex);
		return (0);
	}
	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].last_compile = data->start_time;
		data->coders[i].compiles_done = 0;
		data->coders[i].deadline = 0;
		data->coders[i].left = &data->dongles[i];
		data->coders[i].right = &data->dongles[(i + 1)
			% data->number_of_coders];
		data->coders[i].data = data;
		data->coders[i].next = NULL;
		i++;
	}
	return (1);
}

int	init_data(t_data *data)
{
	if (!init_dongles_and_mutex(data))
		return (0);
	if (!init_coders(data))
		return (0);
	return (1);
}
