/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:10:06 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/08 16:21:16 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathematicians.h"

void	check_if_dead(t_args *args, struct timeval current)
{
	int	i;

	i = 0;
	while (i < args->info->all)
	{
		pthread_mutex_lock(&(args->info->timer_check[i]));
		if (current.tv_usec
			> args->info->death_timer[i].tv_usec + args->info->time_to_die)
		{
			pthread_mutex_lock(&(args->info->stdout_m));
			printf("Philosopher #%i died, skill issue if you ask me\n", i);
			pthread_mutex_unlock(&(args->info->stdout_m));
			clean(args);
			exit(0);
		}
		pthread_mutex_unlock(&(args->info->timer_check[i]));
		i++;
	}
}

void	*checkup(void *arg)
{
	t_args			*args;
	int				i;
	struct timeval	current;

	args = (t_args *) arg;
	gettimeofday(&current, NULL);
	check_if_dead(args, current);
	i = 0;
	while (i < args->info->all)
	{
		pthread_mutex_lock(&(args->info->timer_check[i]));
		if (args[i].self.times_eaten < args->info->needs_to_eat)
			break ;
		pthread_mutex_unlock(&(args->info->timer_check[i]));
		i++;
	}
	if (i == args->info->all)
	{
		pthread_mutex_lock(&(args->info->stdout_m));
		printf("Everybody has eaten enough times\n");
		pthread_mutex_unlock(&(args->info->stdout_m));
		clean(args);
		exit(0);
	}
	return (NULL);
}
