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
		pthread_mutex_lock(&philo->table->meal_check);
		if ((timestamp_ms() - philo->last_meal) > philo->table->time_to_die)
		{
			pthread_mutex_unlock(&philo->table->meal_check);
			pthread_mutex_lock(&philo->table->death_lock);
			if (!philo->table->philo_dead)
			{
				philo->table->philo_dead = 1;
				print_status(philo, philo->table, DEAD);
			}
			pthread_mutex_unlock(&philo->table->death_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->table->meal_check);
		usleep(1000);
	}
	return (NULL);
}

void	*philosopher_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	pthread_mutex_lock(&philo->table->table_mtx);
	pthread_mutex_unlock(&philo->table->table_mtx);
	pthread_create(&philo->death_thread, NULL, monitor_death, philo);
	pthread_detach(philo->death_thread);
	while (philo->table->start_time == 0)
		usleep(100);

	if (philo->id % 2 == 0)
		usleep(1000); // ev
	while (!philo->table->philo_dead
		&& philo->eat_count < philo->table->num_must_eat)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, philo->table, FORK);
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, philo->table, FORK);
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, philo->table, FORK);
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, philo->table, FORK);
		}
		print_status(philo, philo->table, EATING);
		pthread_mutex_lock(&philo->table->meal_check);
		philo->last_meal = timestamp_ms();
		pthread_mutex_unlock(&philo->table->meal_check);
		smart_sleep(philo->table->time_to_eat);
		philo->eat_count++;
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		print_status(philo, philo->table, SLEEPING);
		smart_sleep(philo->table->time_to_sleep);
		print_status(philo, philo->table, THINKING);
	}
	return (NULL);
}
