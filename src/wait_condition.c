/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_condition.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 01:34:03 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 01:34:04 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static void	set_abs_timeout(struct timespec *ts, long wait_ms)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	tv.tv_usec += wait_ms * 1000;
	tv.tv_sec += tv.tv_usec / 1000000;
	tv.tv_usec %= 1000000;
	ts->tv_sec = tv.tv_sec;
	ts->tv_nsec = tv.tv_usec * 1000;
}

static int	can_take_dongle(t_dongle *dongle, t_coder *coder, long now)
{
	long	cooldown;

	cooldown = coder->data->dongle_cooldown;
	if (dongle->available != 1 || (now - dongle->timestamp) < cooldown)
		return (0);
	if (coder->data->scheduler == MY_FIFO && dongle->wait_head == coder)
		return (1);
	if (coder->data->scheduler == MY_EDF && dongle->heap_size > 0
		&& dongle->heap[0] == coder)
		return (1);
	return (0);
}

static void	add_to_queue(t_dongle *dongle, t_coder *coder)
{
	if (coder->data->scheduler == MY_FIFO)
		enqueue_coder(dongle, coder);
	else
		heap_push(dongle, coder);
}

static void	take_dongle(t_dongle *dongle, t_coder *coder, long now)
{
	if (coder->data->scheduler == MY_FIFO)
		dequeue_coder(dongle);
	else
		heap_pop(dongle);
	dongle->available = 0;
	dongle->held_by = coder->id;
	dongle->timestamp = now;
}

int	wait_for_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;
	long			now;
	long			wait_ms;
	long			cooldown;

	cooldown = coder->data->dongle_cooldown;
	coder->deadline = coder->last_compile + coder->data->time_to_burnout;
	add_to_queue(dongle, coder);
	while (1)
	{
		now = get_current_time();
		wait_ms = cooldown - (now - dongle->timestamp);
		if (wait_ms < 0)
			wait_ms = 0;
		set_abs_timeout(&ts, wait_ms);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		if (coder->data->stop)
			return (0);
		now = get_current_time();
		if (can_take_dongle(dongle, coder, now))
			break ;
	}
	take_dongle(dongle, coder, now);
	return (1);
}
