/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 22:44:37 by pbongiov          #+#    #+#             */
/*   Updated: 2026/04/10 22:46:41 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_H
# define THREAD_H

# include <stdbool.h>
# include <pthread.h>

typedef struct s_thread			t_thread;
typedef struct s_thread_plus	t_thread_plus;

typedef struct s_job
{
	void						(*function)(void *arg);
	void						*arg;
}								t_job;

struct							s_thread
{
	void						(*deploy)(t_thread *this, void (*f)(void *),\
	void *arg);
	void						(*destroy)(t_thread *this);
	void						(*wait)(t_thread *this, int n);
};

struct							s_thread_plus
{
	void						(*deploy)(t_thread *this, void (*f)(void *),\
	void *arg);
	void						(*destroy)(t_thread *this);
	void						(*wait)(t_thread *this, int n);
	void						(*clean)(t_thread_plus *this);
	int							to_do;
	int							index;
	int							size;
	bool						quit;
	pthread_t					*thread_id;
	pthread_mutex_t				mutex;
	pthread_cond_t				working;
	t_job						*queue;
	int							pending;
	int							number;
	pthread_cond_t				done;
};

t_thread						*init_tpool(int n);

#endif