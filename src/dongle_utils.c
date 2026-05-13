/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:30:24 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 01:30:52 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

int	try_acquire_dongle_nonblock(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->available == 1 && (get_current_time()
			- dongle->timestamp) >= coder->data->dongle_cooldown)
	{
		dongle->available = 0;
		dongle->held_by = coder->id;
		dongle->timestamp = get_current_time();
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

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
