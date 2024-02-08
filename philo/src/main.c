/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 11:27:30 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/08 22:01:38 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathematicians.h"

pthread_t	make_death_thread(t_args *args);

/* Todo spearate activities into the respective
 functions and add the times to the messages */
void	*philosoph(void *arg)
{
	t_args	*args;

	args = (t_args *) arg;
	pthread_mutex_lock(&(args->info->stdout_m));
	pthread_mutex_unlock(&(args->info->stdout_m));
	while (1)
	{
		take_left(args);
		take_right(args);
		eat(args);
		go_sleep(args);
		think(args);
		if (args->self.isdead == 1)
			return (NULL);
	}
	return (NULL);
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
	// pthread_detach(args->info->death_thread);
	pthread_mutex_destroy(&(args->info->stdout_m));
	free(args->info->forks);
	free(args->info->timer_check);
	free(args->info->death_timer);
	free(args->info->threads);
	free(args);
	return ;
}

/* separate into deathtime checks and required eating times */
pthread_t	make_death_thread(t_args *args)
{
	pthread_t	death_thread;

	pthread_create(&death_thread, NULL, &checkup, args);
	return (death_thread);
}

t_information	parse_args(int argc, char **argv)
{
	t_information	info;

	info.all = atoi(argv[1]);
	info.time_to_die = atoi(argv[2]);
	info.time_to_eat = atoi(argv[3]);
	info.time_to_sleep = atoi(argv[4]);
	if (argc == 5)
		info.needs_to_eat = -1;
	else if (argc == 6)
		info.needs_to_eat = atoi(argv[5]);
	return (info);
}

int	main(int argc, char **argv)
{
	t_args			*args;
	int				i;
	t_information	info;

	i = 0;
	if (argc > 6 || argc < 5)
		return (0);
	info = parse_args(argc, argv);
	setup(&args, info.all, &info);
	while (i < info.all)
		pthread_join(args->info->threads[i++], NULL);
	return (0);
}
