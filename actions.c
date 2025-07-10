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

void	print_status(t_philo *philo, t_table *table, int id)
{
	unsigned int	time;

	time = timestamp_ms() - table->start_time;
	sem_wait(table->print);
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
		sem_post(table->print);
}
