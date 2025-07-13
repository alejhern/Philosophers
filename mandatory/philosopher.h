
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:42:39 by alejhern          #+#    #+#             */
/*   Updated: 2025/07/12 15:42:42 by alejhern         ###   ########.fr       */
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

enum				e_philo_state
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
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_must_eat;
	int 			philo_dead;    // bandera de muerte
	pthread_mutex_t	table_mtx;
	pthread_mutex_t	print;
	pthread_mutex_t	*forks;
	pthread_mutex_t	meal_check;
	pthread_mutex_t	death_lock;
	long			start_time;
}					t_table;

typedef struct s_philo
{
	int				id;
	pid_t			pid;
	long			last_meal;
	int				n_meal;
	pthread_t		death_thread;
	pthread_t		thread;
	t_table			*table;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				eat_count;
}					t_philo;

void				*philosopher_routine(void *ptr);
void				*ft_calloc(size_t nmemb, size_t size);
long				ft_atol(const char *str);
long				timestamp_ms(void);
void				smart_sleep(int time);
void				print_status(t_philo *philo, t_table *table, int id);

#endif