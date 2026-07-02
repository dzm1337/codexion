/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 01:34:10 by dde-paul          #+#    #+#             */
/*   Updated: 2026/07/02 22:15:46 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

void	start_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_create(&data->coders[i].thread, NULL, coder_routine,
			&data->coders[i]);
		i++;
	}
	pthread_create(&data->monitor, NULL, monitor_routine, data);
}

void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
}

int	is_stopped(t_data *data)
{
	int	val;

	pthread_mutex_lock(&data->stop_mutex);
	val = data->stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (val);
}

void	broadcast_stop(t_data *data)
{
	int	j;

	pthread_mutex_lock(&data->stop_mutex);
	data->stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);
	j = 0;
	while (j < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[j].mutex);
		pthread_cond_broadcast(&data->dongles[j].cond);
		pthread_mutex_unlock(&data->dongles[j].mutex);
		j++;
	}
}
