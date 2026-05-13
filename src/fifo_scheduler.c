/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo_scheduler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:27:52 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 00:27:53 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

void	enqueue_coder(t_dongle *dongle, t_coder *coder)
{
	coder->next = NULL;
	if (!dongle->wait_head)
		dongle->wait_head = coder;
	else
		dongle->wait_tail->next = coder;
	dongle->wait_tail = coder;
}

t_coder	*dequeue_coder(t_dongle *dongle)
{
	t_coder	*first_coder;

	first_coder = dongle->wait_head;
	if (!first_coder)
		return (NULL);
	dongle->wait_head = first_coder->next;
	if (!dongle->wait_head)
		dongle->wait_tail = NULL;
	first_coder->next = NULL;
	return (first_coder);
}
