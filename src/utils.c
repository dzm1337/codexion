/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:28:24 by dde-paul          #+#    #+#             */
/*   Updated: 2026/07/02 22:03:15 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	log_action(t_data *data, int coder_id, const char *message)
{
	long	elapsed;
	int		stopped;

	pthread_mutex_lock(&data->print_mutex);
	pthread_mutex_lock(&data->stop_mutex);
	stopped = data->stop;
	pthread_mutex_unlock(&data->stop_mutex);
	if (!stopped)
	{
		elapsed = get_current_time() - data->start_time;
		printf("%ld %d %s\n", elapsed, coder_id, message);
	}
	pthread_mutex_unlock(&data->print_mutex);
}

void	free_dongles(t_dongle *dongles, int amount_of_dongles)
{
	int	i;

	if (!dongles || amount_of_dongles <= 0)
		return ;
	i = 0;
	while (i < amount_of_dongles)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_cond_destroy(&dongles[i].cond);
		if (dongles[i].heap)
			free(dongles[i].heap);
		i++;
	}
	free(dongles);
}

long	ft_atol(const char *s)
{
	long	res;
	int		sign;

	if (!s)
		return (-1);
	res = 0;
	sign = 1;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
		if (*s++ == '-')
			sign = -1;
	while (*s >= '0' && *s <= '9')
		res = res * 10 + (*s++ - '0');
	return (res * sign);
}

void	log_burnout(t_data *data, int id)
{
	long	now;
	int		stopped;

	now = get_current_time();
	pthread_mutex_lock(&data->print_mutex);
	pthread_mutex_lock(&data->stop_mutex);
	stopped = data->stop;
	pthread_mutex_unlock(&data->stop_mutex);
	if (!stopped)
		printf("%ld %d burned out\n", now - data->start_time, id);
	pthread_mutex_unlock(&data->print_mutex);
}
