/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-paul <dde-paul@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 01:43:11 by dde-paul          #+#    #+#             */
/*   Updated: 2026/05/14 17:02:56 by dde-paul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define MY_FIFO 1
# define MY_EDF 2

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;

struct					s_coder
{
	int					id;
	long				last_compile;
	volatile int		compiles_done;
	long				deadline;
	pthread_t			thread;
	t_dongle			*left;
	t_dongle			*right;
	t_data				*data;
	t_coder				*next;
};

struct					s_dongle
{
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	int					available;
	long				timestamp;
	int					held_by;
	t_coder				*wait_head;
	t_coder				*wait_tail;
	t_coder				**heap;
	int					heap_size;
	int					heap_capacity;
};

struct					s_data
{
	int					number_of_coders;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	int					number_of_compiles_required;
	long				dongle_cooldown;
	int					scheduler;
	volatile int		stop;

	long				start_time;

	pthread_t			monitor;
	pthread_mutex_t		print_mutex;

	t_coder				*coders;
	t_dongle			*dongles;
};

long					get_current_time(void);
void					log_action(t_data *d, int id, const char *msg);
void					log_burnout(t_data *d, int id);
void					free_dongles(t_dongle *dongles, int n);
long					ft_atol(const char *s);
int						parse_args(t_data *data, int argc, char **argv);
int						init_data(t_data *data);
void					start_threads(t_data *data);
void					join_threads(t_data *data);
t_dongle				*init_dongles_fifo(int amount);
t_dongle				*init_dongles_edf(int amount);
int						acquire_dongle(t_dongle *dongle, t_coder *coder);
int						release_dongle(t_dongle *dongle, t_coder *coder);
int						release_both_dongles(t_coder *coder);
int						acquire_both_dongles(t_coder *coder);
int						try_acquire_immediate(t_dongle *d, t_coder *c,
							long now);
int						wait_for_dongle(t_dongle *dongle, t_coder *coder);
void					enqueue_coder(t_dongle *d, t_coder *c);
t_coder					*dequeue_coder(t_dongle *d);
void					heap_push(t_dongle *d, t_coder *c);
t_coder					*heap_pop(t_dongle *d);
void					*coder_routine(void *arg);
void					*monitor_routine(void *arg);
int						init_dongle_basic(t_dongle *dongle);
int						init_dongle_edf_heap(t_dongle *dongle, int amount);

#endif
