/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:25:09 by alejhern          #+#    #+#             */
/*   Updated: 2025/07/09 15:25:12 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*monitor_death(void *ptr)
{
	t_table		*table;
	Philosopher	*philo;

	table = (t_table *)ptr;
	philo = table->philosopher;
	while (1)
	{
		if (timestamp_ms() - philo->last_meal > table->time_to_die)
		{
			sem_wait(table->print);
			printf("%ld %d died\n", timestamp_ms() - table->start_time,
				philo->id);
			sem_post(table->dead);
			exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}

void	philosopher_routine(t_table *table)
{
	pthread_create(&table->philosopher->monitor_thread, NULL, monitor_death,
		table);
	pthread_detach(table->philosopher->monitor_thread);
	while (1)
	{
		sem_wait(table->forks);
		print_status(table->philosopher, table, "has taken a fork");
		sem_wait(table->forks);
		print_status(table->philosopher, table, "has taken a fork");
		print_status(table->philosopher, table, "is eating");
		table->philosopher->last_meal = timestamp_ms();
		smart_sleep(table->time_to_eat);
		table->philosopher->eat_count++;
		sem_post(table->forks);
		print_status(table->philosopher, table, "has released a fork");
		sem_post(table->forks);
		print_status(table->philosopher, table, "has released a fork");
        if (table->philosopher->eat_count >= table->num_must_eat)
            exit(0);
		print_status(table->philosopher, table, "is sleeping");
		smart_sleep(table->time_to_sleep);
		print_status(table->philosopher, table, "is thinking");
	}
    printf("%ld %d is done\n", timestamp_ms() - table->start_time,  table->philosopher->id);
}
