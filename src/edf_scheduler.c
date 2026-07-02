/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf_scheduler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:28:00 by dde-paul          #+#    #+#             */
/*   Updated: 2026/07/02 22:01:40 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

void	heapify_up(t_dongle *dongle, int i)
{
	int		parent;
	t_coder	*tmp;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (dongle->heap[parent]->deadline <= dongle->heap[i]->deadline)
			break ;
		tmp = dongle->heap[parent];
		dongle->heap[parent] = dongle->heap[i];
		dongle->heap[i] = tmp;
		i = parent;
	}
}

void	heapify_down(t_dongle *dongle, int i)
{
	int		smallest;
	int		left;
	int		right;
	t_coder	*tmp;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < dongle->heap_size
			&& dongle->heap[left]->deadline < dongle->heap[smallest]->deadline)
			smallest = left;
		if (right < dongle->heap_size
			&& dongle->heap[right]->deadline < dongle->heap[smallest]->deadline)
			smallest = right;
		if (smallest == i)
			break ;
		tmp = dongle->heap[i];
		dongle->heap[i] = dongle->heap[smallest];
		dongle->heap[smallest] = tmp;
		i = smallest;
	}
}

void	heap_push(t_dongle *dongle, t_coder *coder)
{
	if (dongle->heap_size >= dongle->heap_capacity)
	{
		write(2, "Heap overflow\n", 14);
		exit(1);
	}
	dongle->heap[dongle->heap_size] = coder;
	dongle->heap_size++;
	heapify_up(dongle, dongle->heap_size - 1);
}

t_coder	*heap_pop(t_dongle *dongle)
{
	t_coder	*min;

	if (dongle->heap_size == 0)
		return (NULL);
	min = dongle->heap[0];
	dongle->heap[0] = dongle->heap[dongle->heap_size - 1];
	dongle->heap_size--;
	heapify_down(dongle, 0);
	return (min);
}

void	remove_from_queue_edf(t_dongle *dongle, t_coder *coder)
{
	int		i;
	t_coder	*tmp;

	i = 0;
	while (i < dongle->heap_size)
	{
		if (dongle->heap[i] == coder)
		{
			tmp = dongle->heap[dongle->heap_size - 1];
			dongle->heap[i] = tmp;
			dongle->heap_size--;
			if (i < dongle->heap_size)
			{
				heapify_down(dongle, i);
				heapify_up(dongle, i);
			}
			return ;
		}
		i++;
	}
}
