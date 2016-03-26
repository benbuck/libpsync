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

// this is just to avoid casting issues between a pointer and a DWORD
typedef union psync_thread_return_t
{
	void * return_value;
	DWORD exit_code;
} psync_thread_return_t;

static DWORD WINAPI _psync_thread_entry(LPVOID psync_user_data);

psync_thread_t psync_thread_create(psync_thread_entry_t thread_entry, void * user_data, const psync_thread_param_t * thread_param)
{
	psync_thread_t thread;
	SYSTEM_INFO system_info;
	HANDLE thread_handle;
	DWORD stack_size;
	int priority;
	DWORD flags;
	DWORD thread_id;

	thread = (psync_thread_t)malloc(sizeof(psync_thread_t_));
	if (thread == NULL)
	{
		return NULL;
	}

	thread->thread_entry = thread_entry;
	thread->user_data = user_data;

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

		GetSystemInfo(&system_info);
		stack_size = (DWORD)(system_info.dwAllocationGranularity * thread_param->stack_size.relative) + thread_param->stack_size.absolute;
	}

	if (thread_param == NULL)
	{
		priority = THREAD_PRIORITY_NORMAL;
	}
	else
	{
		if ((thread_param->priority.relative < -1.0f) || (thread_param->priority.relative > 1.0f))
		{
			return NULL;
		}

		priority = (int)(2 * thread_param->priority.relative) + thread_param->priority.absolute;
	}

	flags = 0;
	if (priority != THREAD_PRIORITY_NORMAL)
	{
        flags |= CREATE_SUSPENDED;
	}

	thread_handle = CreateThread(NULL, stack_size, _psync_thread_entry, thread, flags, &thread_id);
	if (thread_handle == NULL)
	{
		free(thread);
		return NULL;
	}

	thread->thread_handle = thread_handle;

	if (priority != THREAD_PRIORITY_NORMAL)
	{
		if ((SetThreadPriority(thread_handle, priority) != TRUE) || (ResumeThread(thread_handle) == (DWORD)-1))
		{
			TerminateThread(thread_handle, 0);
			CloseHandle(thread_handle);
			free(thread);
			return NULL;
		}
	}

	return thread;
}

void psync_thread_join(psync_thread_t thread, void ** return_value)
{
	psync_thread_return_t thread_return;

	if (thread == NULL)
	{
		return;
	}

	do
	{
		if (GetExitCodeThread(thread->thread_handle, &thread_return.exit_code) != TRUE)
		{
			return;
		}

		if (thread_return.exit_code == STILL_ACTIVE)
		{
			Sleep(1);
		}
	} while (thread_return.exit_code == STILL_ACTIVE);

	if (return_value != NULL)
	{
		*return_value = thread_return.return_value;
	}

	CloseHandle(thread->thread_handle);
	free(thread);
}

void psync_thread_exit(void * return_value)
{
	psync_thread_return_t thread_return;
	thread_return.return_value = return_value;
	ExitThread(thread_return.exit_code);
}

DWORD WINAPI _psync_thread_entry(LPVOID psync_user_data)
{
	psync_thread_return_t thread_return;

	psync_thread_t thread = (psync_thread_t)psync_user_data;
	thread_return.return_value = thread->thread_entry(thread->user_data);

	return thread_return.exit_code;
}

void psync_thread_sleep(unsigned int microseconds)
{
	DWORD milliseconds = microseconds / 1000; // rounds down
	Sleep(milliseconds);
}
