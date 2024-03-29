/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathematicians.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 11:28:13 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/20 11:51:33 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHEMATICIANS_H
# define MATHEMATICIANS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philosopher
{
	unsigned char	index;
	unsigned char	right;
	unsigned int	times_eaten;
	int				isdead;
}	t_philosopher;

typedef struct s_information
{
	pthread_t		*threads;
	pthread_t		death_thread;
	pthread_mutex_t	info_mutex;
	pthread_mutex_t	*forks;
	pthread_mutex_t	stdout_m;
	pthread_mutex_t	*timer_check;
	suseconds_t		time_to_sleep;
	suseconds_t		time_to_eat;
	suseconds_t		time_to_die;
	int				needs_to_eat;
	int				all;
	struct timeval	*death_timer;
}	t_information;

typedef struct s_args
{
	t_information	*info;
	t_philosopher	self;
}	t_args;

void		take_left(t_args *args);
void		take_right(t_args *args);
void		eat(t_args *args);
void		go_sleep(t_args *args);
void		think(t_args *args);
void		clean(t_args *args);
void		*philosoph(void *arg);
void		*checkup(void *arg);
void		*checkup(void *arg);
void		setup(t_args **args, int philosophers, t_information *info);
pthread_t	make_death_thread(t_args *args);
int			ft_atoi(char *str);
#endif