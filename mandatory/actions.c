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

int	try_get_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		if (check_if_dead(philo))
			return (pthread_mutex_unlock(philo->left_fork), 1);
		print_status(philo, philo->table, FORK);
		pthread_mutex_lock(philo->right_fork);
		if (check_if_dead(philo))
			return (pthread_mutex_unlock(philo->left_fork),
				pthread_mutex_unlock(philo->right_fork), 1);
		print_status(philo, philo->table, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		if (check_if_dead(philo))
			return (pthread_mutex_unlock(philo->right_fork), 1);
		print_status(philo, philo->table, FORK);
		pthread_mutex_lock(philo->left_fork);
		if (check_if_dead(philo))
			return (pthread_mutex_unlock(philo->right_fork),
				pthread_mutex_unlock(philo->left_fork), 1);
		print_status(philo, philo->table, FORK);
	}
	return (0);
}

void	eat(t_philo *philo)
{
	print_status(philo, philo->table, EATING);
	update_last_meal(philo);
	smart_sleep(philo->table->time_to_eat);
	release_forks(philo);
}

void	print_status(t_philo *philo, t_table *table, int id)
{
	unsigned int	time;

	time = timestamp_ms() - table->start_time;
	pthread_mutex_lock(&table->print);
	if (id == FORK)
		printf("%u\t%d has taken a fork\n", time, philo->id);
	else if (id == EATING)
		printf("%u\t%d is eating\n", time, philo->id);
	else if (id == SLEEPING)
		printf("%u\t%d is sleeping\n", time, philo->id);
	else if (id == THINKING)
		printf("%u\t%d is thinking\n", time, philo->id);
	else if (id == DEAD)
		printf("%u\t%d died\n", time, philo->id);
	else if (id == DONE)
		printf("Simulation is Done :)\n");
	pthread_mutex_unlock(&table->print);
}
