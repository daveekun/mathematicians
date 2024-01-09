/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 11:27:30 by dhorvath          #+#    #+#             */
/*   Updated: 2024/01/08 16:25:52 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathematicians.h"
void *philosoph(void *thing)
{
	(void)thing;
	printf("weird shit\n");
	usleep(1000000);
	printf("eat\n");
	usleep(1000000);
	printf("think and die\n");
	return (0);
}

int	main(void)
{
	pthread_t	t1;
	pthread_t	t2;

	pthread_create(&t1, NULL, &philosoph, NULL);
	pthread_create(&t2, NULL, &philosoph, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
