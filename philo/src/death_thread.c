/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:10:06 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/20 11:58:54 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathematicians.h"

void	check_if_dead(t_args *args)
{
	int				i;
	struct timeval	current;

	i = 0;
	while (i < args->info->all)
	{
		pthread_mutex_lock(&(args->info->timer_check[i]));
		gettimeofday(&current, NULL);
		if (current.tv_usec / 1000 + current.tv_sec * 1000
			> args->info->death_timer[i].tv_usec / 1000
			+ args->info->death_timer[i].tv_sec * 1000
			+ args->info->time_to_die)
		{
			pthread_mutex_lock(&(args->info->stdout_m));
			printf("%li %i died\n", current.tv_usec / 1000
				+ current.tv_sec * 1000, i);
			clean(args);
			exit(0);
		}
		pthread_mutex_unlock(&(args->info->timer_check[i]));
		i++;
	}
}

void	check_if_done(t_args *args)
{
	int	i;
	int	isdone;

	isdone = 1;
	i = 0;
	while (i < args->info->all && isdone)
	{
		pthread_mutex_lock(&(args->info->timer_check[i]));
		if (args[i].self.times_eaten < args->info->needs_to_eat)
			isdone = 0;
		pthread_mutex_unlock(&(args->info->timer_check[i]));
		i++;
	}
	if (isdone)
	{
		pthread_mutex_lock(&(args->info->stdout_m));
		printf("Everybody has eaten enough times\n");
		clean(args);
		exit(0);
	}
}

void	*checkup(void *arg)
{
	t_args			*args;
	int				i;

	args = arg;
	while (1)
	{
		pthread_mutex_lock(&(args->info->stdout_m));
		pthread_mutex_unlock(&(args->info->stdout_m));
		check_if_dead(args);
		if (args->info->needs_to_eat != -1)
			check_if_done(args);
	}
	return (NULL);
}
