/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:29:55 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 00:30:14 by dde-paul         ###   ########.fr       */
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
