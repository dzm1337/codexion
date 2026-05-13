/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 01:42:45 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 01:43:48 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/includes.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!parse_args(&data, argc, argv))
	{
		write(2, "Error: invalid arguments\n", 25);
		return (1);
	}
	if (!init_data(&data))
	{
		write(2, "Error: initialization failed\n", 29);
		return (1);
	}
	start_threads(&data);
	join_threads(&data);
	free_dongles(data.dongles, data.number_of_coders);
	free(data.coders);
	pthread_mutex_destroy(&data.print_mutex);
	return (0);
}
