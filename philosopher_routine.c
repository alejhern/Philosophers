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

void *monitor_death(void *ptr) {
    t_table *table;
    Philosopher *philo;
    
    table = (t_table *)ptr;
    philo = table->philosophers;
    while (1) {
        if (timestamp_ms() - philo->last_meal > table->time_to_die) {
            sem_wait(table->print);
            printf("%ld %d died\n", timestamp_ms() - table->start_time, philo->id);
            sem_post(table->dead);
            exit(1);
        }
        usleep(1000);
    }
    return (NULL);
}

void philosopher_routine(Philosopher *philo, t_table *table) {
    pthread_create(&philo->monitor_thread, NULL, monitor_death, table);
    pthread_detach(philo->monitor_thread);

    while (1) {
        // tomar tenedores
        sem_wait(table->forks);
        print_status(philo, table, "has taken a fork");
        sem_wait(table->forks);
        print_status(philo, table, "has taken a fork");

        // comer
        print_status(philo, table, "is eating");
        philo->last_meal = timestamp_ms();
        smart_sleep(table->time_to_eat);
        philo->eat_count++;

        // soltar tenedores
        sem_post(table->forks);
        sem_post(table->forks);

        // dormir
        print_status(philo, table, "is sleeping");
        smart_sleep(table->time_to_sleep);

        // pensar
        print_status(philo, table, "is thinking");
    }
}
