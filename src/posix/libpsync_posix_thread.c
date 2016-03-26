/*

Copyright (c) 2007, Benbuck Nason

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  * Neither the name of the libpsync copyright owner nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#include "libpsync.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

/* Cygwin seems to be missing this */
#ifndef PTHREAD_STACK_MIN
#	define PTHREAD_STACK_MIN (64 * 1024)
#endif

typedef struct psync_thread_t_
{
	pthread_t pthread;
} psync_thread_t_;

psync_thread_t psync_thread_create(psync_thread_entry_t thread_entry, void * user_data, const psync_thread_param_t * thread_param)
{
	int res;
	int priority_min;
	int priority_max;
	int priority_mid;
	int priority;
	size_t stack_size;
	pthread_t pthread;
	pthread_attr_t attr;
	struct sched_param param;
	psync_thread_t thread;

	priority_min = sched_get_priority_min(SCHED_OTHER);
	priority_max = sched_get_priority_max(SCHED_OTHER);
	priority_mid = priority_min + (priority_max - priority_min + 1) / 2;
	if ((priority_min == -1) || (priority_max == -1))
	{
		return NULL;
	}

	if (thread_param == NULL)
	{
		priority = priority_mid;
	}
	else
	{
		if ((thread_param->priority.relative < -1.0f) || (thread_param->priority.relative > 1.0f))
		{
			return NULL;
		}

		priority = priority_min + (int)((priority_max - priority_min + 1) * ((1.0f + thread_param->priority.relative) / 2.0f)) + thread_param->priority.absolute;

		if (priority_min < priority_max)
		{
			if ((priority < priority_min) || (priority > priority_max))
			{
				return NULL;
			}
		}
		else
		{
			if ((priority < priority_max) || (priority > priority_min))
			{
				return NULL;
			}
		}
	}

	if (thread_param == NULL)
	{
		stack_size = 0;
	}
	else
	{
		if (thread_param->stack_size.relative < 0.0f)
		{
			return NULL;
		}

		stack_size = (PTHREAD_STACK_MIN * thread_param->stack_size.relative) + thread_param->stack_size.absolute;
	}

	res = pthread_attr_init(&attr);
	if (res != 0)
	{
		return NULL;
	}

	param.sched_priority = priority;
	res = pthread_attr_setschedparam(&attr, &param);
	if (res != 0)
	{
		return NULL;
	}

	res = pthread_attr_setstacksize(&attr, stack_size);
	if (res != 0)
	{
		pthread_attr_destroy(&attr);
		return NULL;
	}

	thread = (psync_thread_t)malloc(sizeof(psync_thread_t_));
	if (thread == NULL)
	{
		pthread_attr_destroy(&attr);
		return NULL;
	}

	res = pthread_create(&pthread, &attr, thread_entry, user_data);
	pthread_attr_destroy(&attr);
	if (res != 0)
	{
		free(thread);
		return NULL;
	}

	thread->pthread = pthread;

	return thread;
}

psync_bool_t psync_thread_join(psync_thread_t thread, void ** return_value)
{
	int res;

	if (thread == NULL)
	{
		return psync_bool_false;
	}

	res = pthread_join(thread->pthread, return_value);
	if (res != 0)
	{
		return psync_bool_false;
	}

	free(thread);

	return psync_bool_true;
}

void psync_thread_exit(void * return_value)
{
	pthread_exit(return_value);
}

void psync_thread_sleep(unsigned int microseconds)
{
	usleep(microseconds);
}

