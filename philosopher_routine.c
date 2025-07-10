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
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (1)
	{
		if (timestamp_ms() - philo->last_meal > philo->table->time_to_die)
		{
			print_status(philo, philo->table, DEAD);
			exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}

void	philosopher_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	pthread_create(&philo->monitor_thread, NULL, monitor_death, philo);
	pthread_detach(philo->monitor_thread);
	while (1)
	{
		sem_wait(philo->table->forks);
		print_status(philo, philo->table, FORK);
		sem_wait(philo->table->forks);
		print_status(philo, philo->table, FORK);
		print_status(philo, philo->table, EATING);
		philo->last_meal = timestamp_ms();
		smart_sleep(philo->table->time_to_eat);
		philo->eat_count++;
		sem_post(philo->table->forks);
		sem_post(philo->table->forks);
		print_status(philo, philo->table, SLEEPING);
		smart_sleep(philo->table->time_to_sleep);
		print_status(philo, philo->table, THINKING);
		if (philo->eat_count >= philo->table->num_must_eat)
			exit(0);
	}
}
