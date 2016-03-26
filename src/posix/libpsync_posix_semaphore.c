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
#include <semaphore.h>
#include <stdlib.h>

typedef struct psync_semaphore_t_
{
	sem_t sem;
} psync_semaphore_t_;

psync_semaphore_t psync_semaphore_create(int initial_count, int max_count)
{
	int res;
	sem_t sem;
	psync_semaphore_t semaphore;

	res = sem_init(&sem, initial_count, max_count);
	if (res != 0)
	{
		return NULL;
	}

	semaphore = (psync_semaphore_t)malloc(sizeof(psync_semaphore_t_));
	if (semaphore == NULL)
	{
		return NULL;
	}

	semaphore->sem = sem;

	return semaphore;
}

void psync_semaphore_destroy(psync_semaphore_t semaphore)
{
	if (semaphore == NULL)
	{
		return;
	}

	sem_destroy(&semaphore->sem);
	free(semaphore);
}

psync_bool_t psync_semaphore_signal(psync_semaphore_t semaphore)
{
	int res;

	if (semaphore == NULL)
	{
		return psync_bool_false;
	}

	res = sem_post(&semaphore->sem);
	if (res != 0)
	{
		return psync_bool_false;
	}

	return psync_bool_true;
}

psync_bool_t psync_semaphore_wait(psync_semaphore_t semaphore)
{
	int res;

	if (semaphore == NULL)
	{
		return psync_bool_false;
	}

	res = sem_wait(&semaphore->sem);
	if (res != 0)
	{
		return psync_bool_false;
	}

	return psync_bool_true;
}

psync_bool_t psync_semaphore_try(psync_semaphore_t semaphore)
{
	int res;

	if (semaphore == NULL)
	{
		return psync_bool_false;
	}

	res = sem_trywait(&semaphore->sem);
	if (res != 0)
	{
		return psync_bool_false;
	}

	return psync_bool_true;
}
