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

typedef struct psync_thread_t_
{
	pthread_t pthread;
} psync_thread_t_;

psync_thread_t psync_thread_create(psync_thread_entry_t thread_entry, void * user_data, int priority, unsigned int stack_size, char const * name)
{
	int res;
	pthread_t pthread;
	pthread_attr_t attr;
	struct sched_param param;
	psync_thread_t thread;

	(void)name; // unused

	if (priority == PSYNC_THREAD_PRIORITY_DEFAULT)
	{
		priority = 0; // FIX - don't hardcode this magic number
	}

	if (stack_size == 0)
	{
		stack_size = 0x4000; // FIX - don't hardcode this magic number
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

void psync_thread_join(psync_thread_t thread, void ** return_value)
{
	if (thread == NULL)
	{
		return;
	}

	pthread_join(thread->pthread, return_value);
	free(thread);
}

void psync_thread_exit(void * return_value)
{
	pthread_exit(return_value);
}

void psync_thread_sleep(unsigned int microseconds)
{
	usleep(microseconds);
}

