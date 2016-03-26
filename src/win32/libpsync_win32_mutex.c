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
#include <malloc.h>

typedef struct psync_mutex_t_
{
	CRITICAL_SECTION m_critical_section;
} psync_mutex_t_;

psync_mutex_t psync_mutex_create(void)
{
	BOOL success;
	CRITICAL_SECTION critical_section;
	psync_mutex_t mutex;
	
	success = InitializeCriticalSectionAndSpinCount(&critical_section, 5000);
	if (!success)
	{
		return NULL;
	}

	mutex = (psync_mutex_t)malloc(sizeof(psync_mutex_t_));
	if (mutex == NULL)
	{
		return NULL;
	}

	mutex->m_critical_section = critical_section;

	return mutex;
}

void psync_mutex_destroy(psync_mutex_t mutex)
{
	if (mutex == NULL)
	{
		return;
	}

	DeleteCriticalSection(&mutex->m_critical_section);
	free(mutex);
}

psync_bool_t psync_mutex_obtain(psync_mutex_t mutex)
{
	if (mutex == NULL)
	{
		return psync_bool_false;
	}

	EnterCriticalSection(&mutex->m_critical_section);
	return psync_bool_true;
}

void psync_mutex_release(psync_mutex_t mutex)
{
	if (mutex == NULL)
	{
		return;
	}

	LeaveCriticalSection(&mutex->m_critical_section);
}

psync_bool_t psync_mutex_try(psync_mutex_t mutex)
{
	if (mutex == NULL)
	{
		return psync_bool_false;
	}

	return TryEnterCriticalSection(&mutex->m_critical_section) ? psync_bool_true : psync_bool_false;
}
