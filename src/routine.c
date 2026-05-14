/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:28:33 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 17:00:38 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static int	try_compile_cycle(t_coder *coder)
{
	t_data	*data;

	data = coder->data;
	if (!acquire_both_dongles(coder))
		return (0);
	coder->last_compile = get_current_time();
	log_action(data, coder->id, "is compiling");
	usleep(data->time_to_compile * 1000);
	coder->compiles_done++;
	release_both_dongles(coder);
	usleep(1000);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_data	*data;

	coder = (t_coder *)arg;
	data = coder->data;
	while (!data->stop
		&& coder->compiles_done < data->number_of_compiles_required)
	{
		if (!try_compile_cycle(coder))
		{
			usleep(1000);
			continue ;
		}
		if (coder->compiles_done >= data->number_of_compiles_required
			|| data->stop)
			break ;
		log_action(data, coder->id, "is debugging");
		usleep(data->time_to_debug * 1000);
		if (data->stop)
			break ;
		log_action(data, coder->id, "is refactoring");
		usleep(data->time_to_refactor * 1000);
	}
	return (NULL);
}

static void	broadcast_stop(t_data *data)
{
	int	j;

	j = 0;
	while (j < data->number_of_coders)
	{
		pthread_cond_broadcast(&data->dongles[j].cond);
		j++;
	}
	data->stop = 1;
}

static int	check_coder_burnout(t_data *data, int i, long now)
{
	if (data->coders[i].compiles_done < data->number_of_compiles_required)
	{
		if (now > data->coders[i].last_compile + data->time_to_burnout)
		{
			log_burnout(data, data->coders[i].id);
			broadcast_stop(data);
			return (1);
		}
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	now;
	int		all_done;

	data = (t_data *)arg;
	while (!data->stop)
	{
		usleep(5000);
		now = get_current_time();
		all_done = 1;
		i = 0;
		while (i < data->number_of_coders)
		{
			if (data->coders[i].compiles_done
				< data->number_of_compiles_required)
				all_done = 0;
			if (check_coder_burnout(data, i, now))
				return (NULL);
			i++;
		}
		if (all_done)
			return (broadcast_stop(data), NULL);
	}
	return (NULL);
}
