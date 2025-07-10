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

int	sit_down_at_the_table(Philosopher **philosophers, t_table *table, char **argv)
{
    table->num_philos = ft_atol(argv[1]);
    table->time_to_die = ft_atol(argv[2]);
    table->time_to_eat = ft_atol(argv[3]);
    table->time_to_sleep = ft_atol(argv[4]);
    table->num_must_eat = INT_MAX;
    if (argv[5])
        table->num_must_eat = ft_atol(argv[5]);
    if (table->num_philos < 1 || table->time_to_die < 1 || table->time_to_eat < 1
    || table->time_to_sleep < 1 || table->num_must_eat < 1)
        return (printf("min value is 1\n"));
    *philosophers = ft_calloc(table->num_philos, sizeof(Philosopher));
    if (!(*philosophers))
        return (printf("Could not allocate memory\n"));
    sem_unlink("/forks");
    sem_unlink("/print");
    sem_unlink("/dead");
    table->forks = sem_open("/forks", O_CREAT, 0644, table->num_philos);
    table->print = sem_open("/print", O_CREAT, 0644, 1);
    table->dead = sem_open("/dead", O_CREAT, 0644, 0);
    if (table->forks == SEM_FAILED || table->print == SEM_FAILED || table->dead == SEM_FAILED)
        return (1);
    table->start_time = timestamp_ms();
    return (0);
}

void	clean_table(t_table table, Philosopher *philosophers)
{
    int index;

    index = 0;
    waitpid(-1, NULL, 0);
    while (index < table.num_philos)
        kill(philosophers[index++].pid, SIGKILL);
    free(philosophers);
    sem_close(table.forks);
    sem_close(table.print);
    sem_close(table.dead);
}

void	philosopher(Philosopher *philosophers, t_table table)
{
    int id;
    Philosopher *philosopher;

    id = 0;
    philosopher = philosophers;
	while (id < table.num_philos)
	{
		philosopher->id = id + 1;
		philosopher->last_meal = table.start_time;
		philosopher->pid = fork();
		if (philosopher->pid == 0)
		{
            table.philosopher = philosopher;
			philosopher_routine(&table);
			exit(0);
		}
		id++;
        philosopher++;
	}
}

int	main(int argc, char **argv)
{
	Philosopher	*philosophers;
	t_table		table;

	if (argc < 5)
	{
		write(2, "Usage: ./philo_bonus <num_philos> <time_to_die> "
			"<time_to_eat> <time_to_sleep>\n", 81);
		return (1);
	}
	if (sit_down_at_the_table(&philosophers, &table, argv))
		return (1);
    philosopher(philosophers, table);
	clean_table(table, philosophers);
	return (0);
}

