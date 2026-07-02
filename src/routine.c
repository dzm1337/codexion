/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:28:33 by dde-paul          #+#    #+#             */
/*   Updated: 2026/07/02 22:16:10 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static int	try_compile_cycle(t_coder *coder)
{
	t_data	*data;

	data = coder->data;
	if (!acquire_both_dongles(coder))
		return (0);
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile = get_current_time();
	pthread_mutex_unlock(&coder->state_mutex);
	log_action(data, coder->id, "is compiling");
	usleep(data->time_to_compile * 1000);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->state_mutex);
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
	while (!coder_done(coder, data))
	{
		if (!try_compile_cycle(coder))
		{
			usleep(1000);
			continue ;
		}
		if (coder_done(coder, data))
			break ;
		log_action(data, coder->id, "is debugging");
		usleep(data->time_to_debug * 1000);
		if (is_stopped(data))
			break ;
		log_action(data, coder->id, "is refactoring");
		usleep(data->time_to_refactor * 1000);
	}
	return (NULL);
}

static int	check_coder_burnout(t_data *data, int i, long now)
{
	int		done;
	long	last;

	pthread_mutex_lock(&data->coders[i].state_mutex);
	done = data->coders[i].compiles_done;
	last = data->coders[i].last_compile;
	pthread_mutex_unlock(&data->coders[i].state_mutex);
	if (done < data->number_of_compiles_required)
	{
		if (now > last + data->time_to_burnout)
		{
			log_burnout(data, data->coders[i].id);
			broadcast_stop(data);
			return (1);
		}
	}
	return (0);
}

static int	check_all_coders(t_data *data, long now, int *all_done)
{
	int	i;
	int	compiles_done;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].state_mutex);
		compiles_done = data->coders[i].compiles_done;
		pthread_mutex_unlock(&data->coders[i].state_mutex);
		if (compiles_done < data->number_of_compiles_required)
			*all_done = 0;
		if (check_coder_burnout(data, i, now))
			return (1);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	long	now;
	int		all_done;

	data = (t_data *)arg;
	while (!is_stopped(data))
	{
		usleep(5000);
		now = get_current_time();
		all_done = 1;
		if (check_all_coders(data, now, &all_done))
			return (NULL);
		if (all_done)
			return (broadcast_stop(data), NULL);
	}
	return (NULL);
}
