/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 11:27:30 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/20 11:51:13 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathematicians.h"

void	*philosoph(void *arg)
{
	t_args	*args;

	args = (t_args *) arg;
	gettimeofday(&(args->info->death_timer[args->self.index]), NULL);
	while (1)
	{
		pthread_mutex_lock(&(args->info->stdout_m));
		pthread_mutex_unlock(&(args->info->stdout_m));
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

void	clean(t_args *args)
{
	int	i;

	i = 0;
	while (i < args->info->all - 1)
	{
		pthread_mutex_destroy(&(args->info->forks[i]));
		pthread_mutex_destroy(&(args->info->timer_check[i]));
		args[i].self.isdead = 1;
		i++;
	}
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

	info.all = ft_atoi(argv[1]);
	info.time_to_die = ft_atoi(argv[2]);
	info.time_to_eat = ft_atoi(argv[3]);
	info.time_to_sleep = ft_atoi(argv[4]);
	if (info.all <= 0 || info.time_to_die <= 0
		|| info.time_to_eat <= 0 || info.time_to_sleep <= 0)
	{
		printf("Invalid input\n");
		info.all = -1;
		return (info);
	}
	if (argc == 5)
		info.needs_to_eat = -1;
	else
	{
		info.needs_to_eat = atoi(argv[5]);
		if (info.needs_to_eat <= 0)
		{
			printf("Everybody has eaten enough times\n");
			exit(0);
		}
	}
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
	if (info.all == -1)
		return (0);
	setup(&args, info.all, &info);
	while (i < info.all)
	{
		if (pthread_join(args->info->threads[i], NULL) == 0)
			i++;
		else
			clean(args);
	}
	pthread_detach(args->info->death_thread);
	return (0);
}
