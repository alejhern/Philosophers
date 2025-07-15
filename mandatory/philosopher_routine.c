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
	long	time_since_last_meal;

	philo = (t_philo *)ptr;
	while (1)
	{
		usleep(1000);
		pthread_mutex_lock(&philo->table->meal_check);
		time_since_last_meal = timestamp_ms() - philo->last_meal;
		pthread_mutex_unlock(&philo->table->meal_check);
		if (time_since_last_meal > philo->table->time_to_die)
		{
			pthread_mutex_lock(&philo->table->death_lock);
			if (!philo->table->philo_dead)
			{
				philo->table->philo_dead = 1;
				print_status(philo, philo->table, DEAD);
				pthread_mutex_unlock(philo->left_fork);
				pthread_mutex_unlock(philo->right_fork);
			}
			pthread_mutex_unlock(&philo->table->death_lock);
			return (NULL);
		}
	}
}

int	check_if_dead(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(&philo->table->death_lock);
	dead = philo->table->philo_dead;
	pthread_mutex_unlock(&philo->table->death_lock);
	return (dead);
}

void	*philosopher_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (philo->table->start_time == 0)
		usleep(100);
	if (philo->id % 2 == 0)
		usleep(100);
	pthread_create(&philo->death_thread, NULL, monitor_death, philo);
	while (!check_if_dead(philo))
	{
		if (try_get_forks(philo))
			break ;
		if (check_if_dead(philo))
			return (release_forks(philo), NULL);
		eat(philo);
		if (check_if_dead(philo))
			break ;
		print_status(philo, philo->table, SLEEPING);
		smart_sleep(philo->table->time_to_sleep);
		if (check_if_dead(philo))
			break ;
		print_status(philo, philo->table, THINKING);
	}
	pthread_join(philo->death_thread, NULL);
	return (NULL);
}
