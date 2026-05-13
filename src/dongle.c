/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:29:37 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 00:29:38 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

t_dongle	*init_dongles_fifo(int amount)
{
	t_dongle	*dongles;
	int			i;

	if (amount <= 0)
		return (NULL);
	dongles = malloc(sizeof(t_dongle) * amount);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < amount)
	{
		if (pthread_mutex_init(&dongles[i].mutex, NULL) != 0
			|| pthread_cond_init(&dongles[i].cond, NULL) != 0)
			return (free_dongles(dongles, i), NULL);
		dongles[i].timestamp = 0;
		dongles[i].available = 1;
		dongles[i].held_by = -1;
		dongles[i].wait_head = NULL;
		dongles[i].wait_tail = NULL;
		dongles[i].heap = NULL;
		dongles[i].heap_size = 0;
		dongles[i].heap_capacity = 0;
		i++;
	}
	return (dongles);
}

t_dongle	*init_dongles_edf(int amount)
{
	t_dongle	*dongles;
	int			i;

	if (amount <= 0)
		return (NULL);
	dongles = malloc(sizeof(t_dongle) * amount);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < amount)
	{
		if (pthread_mutex_init(&dongles[i].mutex, NULL) != 0
			|| pthread_cond_init(&dongles[i].cond, NULL) != 0)
			return (free_dongles(dongles, i), NULL);
		dongles[i].timestamp = 0;
		dongles[i].available = 1;
		dongles[i].held_by = -1;
		dongles[i].heap = malloc(sizeof(t_coder *) * amount);
		if (!dongles[i].heap)
			return (free_dongles(dongles, i), NULL);
		dongles[i].heap_size = 0;
		dongles[i].heap_capacity = amount;
		i++;
	}
	return (dongles);
}

int	acquire_dongle(t_dongle *dongle, t_coder *coder)
{
	if (!dongle || !coder || coder->id < 1)
		return (0);
	pthread_mutex_lock(&dongle->mutex);
	if (try_acquire_immediate(dongle, coder, get_current_time()))
	{
		pthread_mutex_unlock(&dongle->mutex);
		log_action(coder->data, coder->id, "has taken a dongle");
		return (1);
	}
	if (!wait_for_dongle(dongle, coder))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	pthread_mutex_unlock(&dongle->mutex);
	log_action(coder->data, coder->id, "has taken a dongle");
	return (1);
}

int	release_dongle(t_dongle *dongle, t_coder *coder)
{
	if (!dongle || !coder || coder->id < 1)
		return (0);
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->held_by != coder->id || dongle->available == 1)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	dongle->available = 1;
	dongle->held_by = -1;
	dongle->timestamp = get_current_time();
	if (dongle->wait_head != NULL || dongle->heap_size > 0)
		pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}
