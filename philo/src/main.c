/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 11:27:30 by dhorvath          #+#    #+#             */
/*   Updated: 2024/01/12 17:18:43 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathematicians.h"

pthread_t	make_death_thread(t_args *args);

void	take_left_fork(t_args *args)
{

}

/* Todo spearate activities into the respective functions and add the times to the messages */
void	*philosoph(void *arg)
{
	t_args *args;

	args = (t_args *) arg;
	pthread_mutex_lock(&(args->info->stdout_m));
	pthread_mutex_unlock(&(args->info->stdout_m));
	while (1)
	{
		if (args->self.index % 2)
		{
			pthread_mutex_lock(&(args->info->forks[(args->self.index) % 4]));
			pthread_mutex_lock(&(args->info->stdout_m));
			printf("%i took a fork\n", args->self.index);
			pthread_mutex_unlock(&(args->info->stdout_m));

			pthread_mutex_lock(&(args->info->forks[(args->self.index + 1) % 4]));
			pthread_mutex_lock(&(args->info->stdout_m));
			printf("%i took a fork\n", args->self.index);
			pthread_mutex_unlock(&(args->info->stdout_m));
		}
		else
		{
			pthread_mutex_lock(&(args->info->forks[(args->self.index + 1) % 4]));
			pthread_mutex_lock(&(args->info->stdout_m));
			printf("%i took a fork\n", args->self.index);
			pthread_mutex_unlock(&(args->info->stdout_m));

			pthread_mutex_lock(&(args->info->forks[(args->self.index) % 4]));
			pthread_mutex_lock(&(args->info->stdout_m));
			printf("%i took a fork\n", args->self.index);
			pthread_mutex_unlock(&(args->info->stdout_m));
		}
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
		pthread_mutex_unlock(&(args->info->forks[args->self.index % 4]));
		pthread_mutex_unlock(&(args->info->forks[(args->self.index + 1) % 4]));
		pthread_mutex_lock(&(args->info->stdout_m));
		printf("%i is sleeping\n", args->self.index);
		pthread_mutex_unlock(&(args->info->stdout_m));
		usleep(args->info->time_to_sleep);
		pthread_mutex_lock(&(args->info->stdout_m));
		printf("%i is thinking\n", args->self.index);
		pthread_mutex_unlock(&(args->info->stdout_m));
		if (args->self.isdead == 1)
			return (NULL);
	}
	return (NULL);
}

void	setup(t_args **args, int philosophers)
{
	int				i;
	static t_information	info;
	pthread_t		*threads;

	*args = malloc((philosophers + 1) * sizeof(t_args));
	threads = malloc(philosophers * sizeof(pthread_t));
	pthread_mutex_init(&(info.stdout_m), NULL);
	pthread_mutex_init(&(info.info_mutex), NULL);
	pthread_mutex_lock(&(info.info_mutex));
	info.all = philosophers;
	info.time_to_eat = 3000000;
	info.time_to_die = 2000000;
	info.time_to_sleep = 2000000;
	info.needs_to_eat = 10000;
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
	i = 0;
	while (i < philosophers)
	{
		(*args)[i].self.times_eaten = 0;
		(*args)[i].self.index = i;
		(*args)[i].self.isdead = 0;
		(*args)[i].info = &info;
		i++;
	}
	pthread_mutex_lock(&(info.stdout_m));	
	i = 0;
	while (i < philosophers)
	{
		pthread_create(&(threads[i]), NULL, &philosoph, &((*args)[i]));
		i++;
	}	
	pthread_mutex_unlock(&(info.stdout_m));
	(*args)->info->death_thread = make_death_thread(*args);
}

/* complete freeing the memory, detaching threads and destroying mutexes */
void	clean(t_args *args)
{
	int	i;

	i = 0;
	while (i < args->info->all)
	{
		pthread_detach(args->info->threads[i]);
		pthread_mutex_destroy(&(args->info->forks[i]));
		pthread_mutex_destroy(&(args->info->timer_check[i]));
		i++;
	}
	pthread_detach(args->info->death_thread);
	pthread_mutex_destroy(&(args->info->stdout_m));
	free(args->info->forks);
	free(args->info->timer_check);
	free(args->info->death_timer);
	free(args->info->threads);
	free(args);
	return ;
}

/* separate into deathtime checks and required eating times */
void	*checkup(void *arg)
{
	t_args			*args;
	int				i;
	struct timeval	current;

	args = (t_args *) arg;
	gettimeofday(&current, NULL);
	i = 0;
	while (i < args->info->all)
	{
		pthread_mutex_lock(&(args->info->timer_check[i]));
		if (current.tv_usec > args->info->death_timer[i].tv_usec + args->info->time_to_die)
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

pthread_t	make_death_thread(t_args *args)
{
	pthread_t	death_thread;

	pthread_create(&death_thread, NULL, &checkup, args);
	return (death_thread);
}

int	main(void)
{
	t_args		*args;
	int			i;
	int			count = 4;

	setup(&args, count);
	pthread_join(args->info->threads[0], NULL);
	pthread_join(args->info->threads[1], NULL);
	pthread_join(args->info->threads[2], NULL);
	pthread_join(args->info->threads[3], NULL);
	return (0);
}
