/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 22:45:32 by pbongiov          #+#    #+#             */
/*   Updated: 2026/04/10 22:45:35 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread.h"

void	*swimming(void *p)
{
	t_job			job;
	t_thread_plus	*this;

	this = p;
	while (1)
	{
		pthread_mutex_lock(&this->mutex);
		while (!this->to_do && !this->quit)
			pthread_cond_wait(&this->working, &this->mutex);
		if (this->quit)
			return (pthread_mutex_unlock(&this->mutex), NULL);
		job = this->queue[this->index];
		this->index = (this->index + 1) % 10;
		this->to_do--;
		pthread_mutex_unlock(&this->mutex);
		if (job.function)
			job.function(job.arg);
		pthread_mutex_lock(&this->mutex);
		this->pending++;
		pthread_cond_signal(&this->done);
		pthread_mutex_unlock(&this->mutex);
	}
	return (NULL);
}

static void	_deploy(t_thread *this, void (*f)(void *), void *arg)
{
	t_thread_plus	*new;

	new = (t_thread_plus *)this;
	pthread_mutex_lock(&new->mutex);
	if (new->to_do == 10)
	{
		pthread_mutex_unlock(&new->mutex);
		return ;
	}
	new->queue[new->size].function = f;
	new->queue[new->size].arg = arg;
	new->size = (new->size + 1) % 10;
	new->to_do++;
	pthread_cond_signal(&new->working);
	pthread_mutex_unlock(&new->mutex);
}

void	_destroy(t_thread *this)
{
	int				i;
	t_thread_plus	*new;

	if (!this)
		return ;
	new = (t_thread_plus *)this;
	pthread_mutex_lock(&new->mutex);
	new->quit = true;
	pthread_cond_broadcast(&new->working);
	pthread_cond_broadcast(&new->done);
	pthread_mutex_unlock(&new->mutex);
	i = 0;
	while (i < new->number)
		pthread_join(new->thread_id[i++], NULL);
	pthread_mutex_destroy(&new->mutex);
	pthread_cond_destroy(&new->done);
	pthread_cond_destroy(&new->working);
	free(new->thread_id);
	free(new->queue);
	free(new);
}

void	_wait(t_thread *this, int n)
{
	t_thread_plus	*new;

	new = (t_thread_plus *)this;
	pthread_mutex_lock(&new->mutex);
	while (new->pending < n)
		pthread_cond_wait(&new->done, &new->mutex);
	new->pending = 0;
	pthread_mutex_unlock(&new->mutex);
}

t_thread	*init_tpool(int n)
{
	int				i;
	t_thread_plus	*new;

	i = -1;
	new = ft_calloc(1, sizeof(t_thread_plus));
	if (!new)
		parse_exit("Memory Allocation\n", NULL, -1, 0);
	new->number = n;
	new->deploy = _deploy;
	new->destroy = _destroy;
	new->wait = _wait;
	pthread_cond_init(&new->working, NULL);
	pthread_cond_init(&new->done, NULL);
	pthread_mutex_init(&new->mutex, NULL);
	new->queue = ft_calloc(10, sizeof(t_job));
	new->thread_id = ft_calloc(n, sizeof(pthread_t));
	if (!new->queue || !new->thread_id)
	{
		(free(new->queue), free(new->thread_id), free(new));
		parse_exit("Memory Allocation\n", NULL, -1, 0);
	}
	while (++i < n)
		pthread_create(&new->thread_id[i], NULL, swimming, new);
	return ((t_thread *)new);
}
