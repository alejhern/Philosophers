/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:41:32 by alejhern          #+#    #+#             */
/*   Updated: 2025/07/12 15:41:36 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
int sit_down_at_the_table(t_philo **philosophers, t_table *table, char **argv)
{
	int	i;

	table->num_philos = ft_atol(argv[1]);
	table->time_to_die = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	table->num_must_eat = (argv[5]) ? ft_atol(argv[5]) : INT_MAX;
	table->philo_dead = 0;
	table->start_time = 0;

	if (table->num_philos < 1 || table->time_to_die < 1 ||
		table->time_to_eat < 1 || table->time_to_sleep < 1 || table->num_must_eat < 1)
		return (printf("Invalid arguments: all values must be >= 1\n"), 1);

	*philosophers = ft_calloc(table->num_philos, sizeof(t_philo));
	if (!(*philosophers))
		return (printf("Memory allocation failed for philosophers\n"), 1);

	table->forks = ft_calloc(table->num_philos, sizeof(pthread_mutex_t));
	if (!table->forks)
		return (printf("Memory allocation failed for forks\n"), 1);

	for (i = 0; i < table->num_philos; i++)
		pthread_mutex_init(&table->forks[i], NULL);

	pthread_mutex_init(&table->print, NULL);
	pthread_mutex_init(&table->meal_check, NULL);
	pthread_mutex_init(&table->table_mtx, NULL);
	pthread_mutex_init(&table->death_lock, NULL);

	return (0);
}

void philosopher(t_philo *philosophers, t_table *table)
{
	int	id;

	id = 0;
	while (id < table->num_philos)
	{
		philosophers[id].id = id + 1;
		philosophers[id].last_meal = 0;
		philosophers[id].table = table;
		philosophers[id].right_fork = &table->forks[(id + 1) % table->num_philos];
		philosophers[id].left_fork = &table->forks[id];
		philosophers[id].n_meal = 0;
		pthread_create(&philosophers[id].thread, NULL, philosopher_routine, &philosophers[id]);
		id++;
	}

	table->start_time = timestamp_ms();

	id = 0;
	while (id < table->num_philos)
	{
		philosophers[id].last_meal = table->start_time;
		id++;
	}

	id = 0;
	while (id < table->num_philos)
	{
		pthread_join(philosophers[id].thread, NULL);
		id++;
	}
}

void clean_table(t_table table, t_philo *philosophers)
{
	int i = -1;

	while (++i < table.num_philos)
		pthread_mutex_destroy(&table.forks[i]);
	pthread_mutex_destroy(&table.print);
	pthread_mutex_destroy(&table.meal_check);
	pthread_mutex_destroy(&table.table_mtx);
	pthread_mutex_destroy(&table.death_lock);

	free(table.forks);
	free(philosophers);
}

int main(int argc, char **argv)
{
	t_philo	*philosophers;
	t_table	table;

	if (argc < 5)
	{
		printf("Usage: ./philo <num_philos> <time_to_die> <time_to_eat> <time_to_sleep>\n");
		return (1);
	}
	if (sit_down_at_the_table(&philosophers, &table, argv))
		return (1);
	philosopher(philosophers, &table);
	clean_table(table, philosophers);
	return (0);
}