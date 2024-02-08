/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 11:27:30 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/08 16:10:35 by dhorvath         ###   ########.fr       */
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
		if (args->self.index % 2)
		{
			take_left(args);
			take_right(args);
		}
		else
		{
			take_right(args);
			take_left(args);
		}
		eat(args);
		sleep(args);
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

	setup(&args, 4);
	args->info->time_to_eat = 3000000;
	args->info->time_to_die = 2000000;
	args->info->time_to_sleep = 2000000;
	args->info->needs_to_eat = 10000;
	pthread_join(args->info->threads[0], NULL);
	pthread_join(args->info->threads[1], NULL);
	pthread_join(args->info->threads[2], NULL);
	pthread_join(args->info->threads[3], NULL);
	return (0);
}
