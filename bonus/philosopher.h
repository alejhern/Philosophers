/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:55:47 by alejhern          #+#    #+#             */
/*   Updated: 2025/07/09 13:55:51 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

enum			e_philo_state
{
	THINKING,
	DEAD,
	EATING,
	HUNGRY,
	SLEEPING,
	DONE,
	FORK
};

typedef struct s_table
{
	int			num_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_must_eat;
	long		start_time;
	sem_t		*forks;
	sem_t		*print;
	sem_t		*waiter;
}				t_table;

typedef struct s_philo
{
	int			id;
	long		last_meal;
	int			eat_count;
	pid_t		pid;
	pthread_t	monitor_thread;
	int			state;
	t_table		*table;
}				t_philo;

void			philosopher_routine(void *ptr);
void			*ft_calloc(size_t nmemb, size_t size);
long			ft_atol(const char *str);
long			timestamp_ms(void);
void			smart_sleep(int time);
void			print_status(t_philo *philo, t_table *table, int id);

#endif
