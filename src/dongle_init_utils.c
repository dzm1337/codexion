/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:30:24 by dde-paul          #+#    #+#             */
/*   Updated: 2026/07/02 22:31:01 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

int	try_acquire_immediate(t_dongle *dongle, t_coder *coder, long now)
{
	if (dongle->available == 1 && (now
			- dongle->timestamp) >= coder->data->dongle_cooldown)
	{
		dongle->available = 0;
		dongle->held_by = coder->id;
		dongle->timestamp = now;
		return (1);
	}
	return (0);
}

int	init_dongle_basic(t_dongle *dongle)
{
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0
		|| pthread_cond_init(&dongle->cond, NULL) != 0)
		return (0);
	dongle->timestamp = 0;
	dongle->available = 1;
	dongle->held_by = -1;
	dongle->wait_head = NULL;
	dongle->wait_tail = NULL;
	return (1);
}

int	init_dongle_edf_heap(t_dongle *dongle, int amount)
{
	dongle->heap = malloc(sizeof(t_coder *) * amount);
	if (!dongle->heap)
		return (0);
	dongle->heap_size = 0;
	dongle->heap_capacity = amount;
	return (1);
}

int	check_stop_and_relock(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_unlock(&dongle->mutex);
	pthread_mutex_lock(&coder->data->stop_mutex);
	if (coder->data->stop)
	{
		pthread_mutex_unlock(&coder->data->stop_mutex);
		pthread_mutex_lock(&dongle->mutex);
		if (coder->data->scheduler == MY_FIFO)
			remove_from_queue_fifo(dongle, coder);
		else
			remove_from_queue_edf(dongle, coder);
		return (1);
	}
	pthread_mutex_unlock(&coder->data->stop_mutex);
	pthread_mutex_lock(&dongle->mutex);
	return (0);
}

void	add_to_queue(t_dongle *dongle, t_coder *coder)
{
	if (coder->data->scheduler == MY_FIFO)
		enqueue_coder(dongle, coder);
	else
		heap_push(dongle, coder);
}
