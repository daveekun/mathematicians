/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:50:45 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/08 16:22:20 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathematicians.h"

static void	create_philo(t_args *args, int philosophers, t_information *info)
{
	int	i;

	i = 0;
	while (i < philosophers)
	{
		args[i].self.times_eaten = 0;
		args[i].self.index = i;
		args[i].self.right = (i + 1) % philosophers;
		args[i].self.isdead = 0;
		args[i].info = info;
		i++;
	}
}

static void	create_threads(pthread_t *threads, t_args *args, int philosophers)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&(args->info->stdout_m));
	while (i < philosophers)
	{
		pthread_create(&(threads[i]), NULL, &philosoph, &args[i]);
		i++;
	}
	pthread_mutex_unlock(&(args->info->stdout_m));
}

void	setup(t_args **args, int philosophers)
{
	int						i;
	static t_information	info;
	pthread_t				*threads;

	*args = malloc((philosophers + 1) * sizeof(t_args));
	threads = malloc(philosophers * sizeof(pthread_t));
	pthread_mutex_init(&(info.stdout_m), NULL);
	pthread_mutex_init(&(info.info_mutex), NULL);
	pthread_mutex_lock(&(info.info_mutex));
	info.all = philosophers;
	info.forks = malloc(philosophers * sizeof(pthread_mutex_t));
	info.timer_check = malloc(philosophers * sizeof(pthread_mutex_t));
	info.death_timer = malloc(philosophers * sizeof(struct timeval));
	i = 0;
	while (i < philosophers)
	{
		pthread_mutex_init(info.forks + i, NULL);
		pthread_mutex_init(info.timer_check + i, NULL);
		i++;
	}
	info.threads = threads;
	create_philo(*args, philosophers, &info);
	create_threads(threads, *args, philosophers);
	(*args)->info->death_thread = make_death_thread(*args);
}
