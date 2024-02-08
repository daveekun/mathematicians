/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:20:37 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/08 15:22:34 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathematicians.h"

void	take_left(t_args *args)
{
	pthread_mutex_lock(&(args->info->forks[args->self.index]));
	pthread_mutex_lock(&(args->info->stdout_m));
	printf("%i took a fork\n", args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
}

void	take_right(t_args *args)
{
	pthread_mutex_lock(&(args->info->forks[args->self.right]));
	pthread_mutex_lock(&(args->info->stdout_m));
	printf("%i took a fork\n", args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
}

void	eat(t_args *args)
{
	pthread_mutex_lock(&(args->info->timer_check[args->self.index]));
	gettimeofday(&(args->info->death_timer[args->self.index]), NULL);
	pthread_mutex_unlock(&(args->info->timer_check[args->self.index]));
	pthread_mutex_lock(&(args->info->stdout_m));
	printf("%i is eating\n", args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
	usleep(args->info->time_to_eat);
	pthread_mutex_lock(&(args->info->timer_check[args->self.index]));
	args->self.times_eaten += 1;
	pthread_mutex_unlock(&(args->info->timer_check[args->self.index]));
	pthread_mutex_unlock(&(args->info->forks[args->self.index]));
	pthread_mutex_unlock(&(args->info->forks[args->self.right]));
}

void	sleep(t_args *args)
{
	pthread_mutex_lock(&(args->info->stdout_m));
	printf("%i is sleeping\n", args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
	usleep(args->info->time_to_sleep);
}

void	think(t_args *args)
{
	pthread_mutex_lock(&(args->info->stdout_m));
	printf("%i is thinking\n", args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
}
