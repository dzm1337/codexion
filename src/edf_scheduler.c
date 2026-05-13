/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf_scheduler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:28:00 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 00:28:01 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static void	swap_coders(t_coder **a, t_coder **b)
{
	t_coder	*t;

	t = *a;
	*a = *b;
	*b = t;
}

static void	heapify_up(t_dongle *dongle, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (dongle->heap[parent]->deadline <= dongle->heap[i]->deadline)
			break ;
		swap_coders(&dongle->heap[parent], &dongle->heap[i]);
		i = parent;
	}
}

static void	heapify_down(t_dongle *dongle, int i)
{
	int	smallest;
	int	right;
	int	left;

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
		swap_coders(&dongle->heap[i], &dongle->heap[smallest]);
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
