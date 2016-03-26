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
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0403
#include <windows.h>
#include <stdlib.h>

typedef struct psync_thread_t_
{
	HANDLE thread_handle;
	psync_thread_entry_t thread_entry;
	void * user_data;
} psync_thread_t_;

static DWORD WINAPI _psync_thread_entry(LPVOID psync_user_data);

psync_thread_t psync_thread_create(psync_thread_entry_t thread_entry, void * user_data, int priority, unsigned int stack_size, char const * name)
{
	HANDLE thread_handle;
	DWORD thread_id;
	DWORD flags;
	psync_thread_t thread;

	(void)name;

	thread = (psync_thread_t)malloc(sizeof(psync_thread_t_));
	if (thread == NULL)
	{
		return NULL;
	}

	/* not needed
	if (stack_size == 0)
	{
		stack_size = 0;
	}
	*/

	flags = 0;
	if (priority != PSYNC_THREAD_PRIORITY_DEFAULT)
	{
        flags |= CREATE_SUSPENDED;
	}

	thread_handle = CreateThread(NULL, stack_size, _psync_thread_entry, thread, 0, &thread_id);
	if (thread_handle == NULL)
	{
		free(thread);
		return NULL;
	}

	if (priority != PSYNC_THREAD_PRIORITY_DEFAULT)
	{
		if ((SetThreadPriority(thread_handle, priority) != TRUE) || (ResumeThread(thread_handle) == (DWORD)-1))
		{
			TerminateThread(thread_handle, 0);
			CloseHandle(thread_handle);
			free(thread);
			return NULL;
		}
	}

	thread->thread_handle = thread_handle;
	thread->thread_entry = thread_entry;
	thread->user_data = user_data;

	return thread;
}

void psync_thread_join(psync_thread_t thread, void ** return_value)
{
	DWORD exit_code;

	if (thread == NULL)
	{
		return;
	}

	do
	{
		if (GetExitCodeThread(thread->thread_handle, &exit_code) != TRUE)
		{
			return;
		}

		if (exit_code == STILL_ACTIVE)
		{
			Sleep(1);
		}
	} while (exit_code == STILL_ACTIVE);

	if (return_value != NULL)
	{
		*return_value = (void *)exit_code;
	}

	CloseHandle(thread->thread_handle);
	free(thread);
}

void psync_thread_exit(void * return_value)
{
	ExitThread((DWORD)return_value);
}

DWORD WINAPI _psync_thread_entry(LPVOID psync_user_data)
{
	void * return_value;

	psync_thread_t thread = (psync_thread_t)psync_user_data;
	return_value = thread->thread_entry(thread->user_data);

	return (DWORD)return_value;
}

void psync_thread_sleep(unsigned int microseconds)
{
	DWORD milliseconds = microseconds / 1000; // rounds down
	Sleep(milliseconds);
}

