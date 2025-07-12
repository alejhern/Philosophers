/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:57:35 by alejhern          #+#    #+#             */
/*   Updated: 2025/07/09 13:57:40 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	sit_down_at_the_table(t_philo **philosophers, t_table *table, char **argv)
{
	table->num_philos = ft_atol(argv[1]);
	table->time_to_die = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	table->num_must_eat = INT_MAX;
	if (argv[5])
		table->num_must_eat = ft_atol(argv[5]);
	if (table->num_philos < 1 || table->time_to_die < 1
		|| table->time_to_eat < 1 || table->time_to_sleep < 1
		|| table->num_must_eat < 1)
		return (printf("min value is 1\n"));
	*philosophers = ft_calloc(table->num_philos, sizeof(t_philo));
	if (!(*philosophers))
		return (printf("Could not allocate memory\n"));
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/waiter");
	table->forks = sem_open("/forks", O_CREAT, 0644, table->num_philos);
	table->print = sem_open("/print", O_CREAT, 0644, 1);
	table->waiter = sem_open("/waiter", O_CREAT, 0644, 0);
	if (table->forks == SEM_FAILED || table->print == SEM_FAILED)
		return (1);
	table->start_time = timestamp_ms();
	return (0);
}

void	clean_table(t_table table, t_philo *philosophers)
{
	int		index;
	int		status;
	pid_t	child;

	index = 0;
	child = waitpid(-1, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	index = 0 - status;
	if (status)
	{
		while (++index < table.num_philos)
			if (philosophers[index].pid != child)
				kill(philosophers[index].pid, SIGKILL);
	}
	else
		while (++index < table.num_philos)
			waitpid(-1, NULL, 0);
	free(philosophers);
	sem_close(table.forks);
	sem_close(table.print);
	sem_close(table.waiter);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/waiter");
}

void	philosopher(t_philo *philosophers, t_table table)
{
	int		id;
	t_philo	*philosopher;

	id = 0;
	philosopher = philosophers;
	sem_post(table.waiter);
	while (id < table.num_philos)
	{
		philosopher->id = id + 1;
		philosopher->last_meal = table.start_time;
		philosopher->table = &table;
		philosopher->pid = fork();
		if (philosopher->pid == 0)
		{
			philosopher_routine(philosopher);
			exit(0);
		}
		id++;
		philosopher++;
		usleep(100);
	}
}

int	main(int argc, char **argv)
{
	t_philo	*philosophers;
	t_table	table;

	if (argc < 5)
	{
		printf("Usage: ./philo <num_philos> <time_to_die> "
			"<time_to_eat> <time_to_sleep>\n");
		return (1);
	}
	if (sit_down_at_the_table(&philosophers, &table, argv))
		return (1);
	philosopher(philosophers, table);
	clean_table(table, philosophers);
	return (0);
}
