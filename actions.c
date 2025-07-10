/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:07:49 by alejhern          #+#    #+#             */
/*   Updated: 2025/07/09 18:07:51 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	smart_sleep(int time)
{
	long	start;

	start = timestamp_ms();
	while (timestamp_ms() - start < time)
		usleep(100);
}

void	print_status(Philosopher *philo, t_table *table, char *msg)
{
	sem_wait(table->print);
	printf("%ld ", timestamp_ms() - table->start_time);
	printf("%d ", philo->id);
	printf("%s\n", msg);
	sem_post(table->print);
}