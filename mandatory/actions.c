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

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, philo->table, FORK);
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, philo->table, FORK);

	pthread_mutex_lock(&philo->table->table_mtx);
	print_status(philo, philo->table, EATING);
	philo->last_meal = timestamp_ms();
	philo->n_meal++;
	pthread_mutex_unlock(&philo->table->table_mtx);

	usleep(philo->table->time_to_eat * 1000);

	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_act(t_philo *philo)
{
	print_status(philo, philo->table, SLEEPING);
	usleep(philo->table->time_to_sleep * 1000);
}

void	think(t_philo *philo)
{
	print_status(philo, philo->table, THINKING);
}

void	smart_sleep(int time)
{
	long	start;

	start = timestamp_ms();
	while (timestamp_ms() - start < time)
		usleep(100);
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
	if (id != DEAD)
		pthread_mutex_unlock(&table->print);
}
