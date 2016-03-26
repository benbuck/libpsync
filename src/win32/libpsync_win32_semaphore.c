/*

Copyright (c) 2007-2009, Benbuck Nason

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
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0403
#include <windows.h>
#include <malloc.h>

typedef struct psync_semaphore_t_
{
	HANDLE m_semaphore;
} psync_semaphore_t_;

psync_semaphore_t psync_semaphore_create(int initial_count, int max_count)
{
	HANDLE handle_semaphore;
	psync_semaphore_t semaphore;

	handle_semaphore = CreateSemaphore(NULL, initial_count, max_count, NULL);
	if (handle_semaphore == NULL)
	{
		return NULL;
	}

	semaphore = (psync_semaphore_t)malloc(sizeof(psync_semaphore_t_));
	if (semaphore == NULL)
	{
		return NULL;
	}

	semaphore->m_semaphore = handle_semaphore;

	return semaphore;
}

void psync_semaphore_destroy(psync_semaphore_t semaphore)
{
	if (semaphore == NULL)
	{
		return;
	}

	CloseHandle(semaphore->m_semaphore);
	free(semaphore);
}

psync_bool_t psync_semaphore_signal(psync_semaphore_t semaphore)
{
	if (semaphore == NULL)
	{
		return psync_bool_false;
	}

	return ReleaseSemaphore(semaphore->m_semaphore, 1, NULL) ? psync_bool_true : psync_bool_false;
}

psync_bool_t psync_semaphore_wait(psync_semaphore_t semaphore)
{
	DWORD result;

	if (semaphore == NULL)
	{
		return psync_bool_false;
	}

	result = WaitForSingleObject(semaphore->m_semaphore, INFINITE);
	if (result != WAIT_OBJECT_0)
	{
		return psync_bool_false;
	}

	return psync_bool_true;
}

psync_bool_t psync_semaphore_try(psync_semaphore_t semaphore)
{
	DWORD result;

	if (semaphore == NULL)
	{
		return psync_bool_false;
	}

	result = WaitForSingleObject(semaphore->m_semaphore, 0);
	if (result != WAIT_OBJECT_0)
	{
		return psync_bool_false;
	}
	
	return psync_bool_true;
}
