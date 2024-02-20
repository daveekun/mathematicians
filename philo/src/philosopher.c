/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:20:37 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/13 16:32:10 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathematicians.h"

void	take_left(t_args *args)
{
	struct timeval	current;

	pthread_mutex_lock(&(args->info->forks[args->self.index]));
	pthread_mutex_lock(&(args->info->stdout_m));
	gettimeofday(&current, NULL);
	printf("%li %i has taken a fork\n", current.tv_sec * 1000
		+ current.tv_usec / 1000, args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
}

void	take_right(t_args *args)
{
	struct timeval	current;

	pthread_mutex_lock(&(args->info->forks[args->self.right]));
	pthread_mutex_lock(&(args->info->stdout_m));
	gettimeofday(&current, NULL);
	printf("%li %i has taken a fork\n", current.tv_sec * 1000
		+ current.tv_usec / 1000, args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
}

void	eat(t_args *args)
{
	struct timeval	current;

	pthread_mutex_lock(&(args->info->timer_check[args->self.index]));
	gettimeofday(&(args->info->death_timer[args->self.index]), NULL);
	pthread_mutex_unlock(&(args->info->timer_check[args->self.index]));
	pthread_mutex_lock(&(args->info->stdout_m));
	gettimeofday(&current, NULL);
	printf("%li %i is eating\n", current.tv_sec * 1000
		+ current.tv_usec / 1000, args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
	usleep(args->info->time_to_eat * 1000);
	pthread_mutex_lock(&(args->info->timer_check[args->self.index]));
	args->self.times_eaten += 1;
	pthread_mutex_unlock(&(args->info->timer_check[args->self.index]));
	pthread_mutex_unlock(&(args->info->forks[args->self.index]));
	pthread_mutex_unlock(&(args->info->forks[args->self.right]));
}

void	go_sleep(t_args *args)
{
	struct timeval	current;

	pthread_mutex_lock(&(args->info->stdout_m));
	gettimeofday(&current, NULL);
	printf("%li %i is sleeping\n", current.tv_sec * 1000
		+ current.tv_usec / 1000, args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
	usleep(args->info->time_to_sleep * 1000);
}

void	think(t_args *args)
{
	struct timeval	current;

	pthread_mutex_lock(&(args->info->stdout_m));
	gettimeofday(&current, NULL);
	printf("%li %i is thinking\n", current.tv_sec * 1000
		+ current.tv_usec / 1000, args->self.index);
	pthread_mutex_unlock(&(args->info->stdout_m));
}
