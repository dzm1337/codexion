/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:29:55 by dde-paul          #+#    #+#             */
/*   Updated: 2026/07/02 20:05:37 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

int	acquire_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left < coder->right)
	{
		first = coder->left;
		second = coder->right;
	}
	else
	{
		first = coder->right;
		second = coder->left;
	}
	if (!acquire_dongle(first, coder))
		return (0);
	if (!acquire_dongle(second, coder))
	{
		release_dongle(first, coder);
		return (0);
	}
	return (1);
}

int	release_both_dongles(t_coder *coder)
{
	return (release_dongle(coder->left, coder) && release_dongle(coder->right,
			coder));
}

int	coder_done(t_coder *coder, t_data *data)
{
	int	done;

	pthread_mutex_lock(&coder->state_mutex);
	done = coder->compiles_done;
	pthread_mutex_unlock(&coder->state_mutex);
	return (done >= data->number_of_compiles_required || is_stopped(data));
}
