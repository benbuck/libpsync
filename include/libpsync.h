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

/** libpsync - threading and syncronization primitives **/

#ifndef _libpsync_h_
#define _libpsync_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** Common **/

typedef enum psync_bool_t
{
	psync_bool_false,
	psync_bool_true
} psync_bool_t;


/** Mutex **/

typedef struct psync_mutex_t_ * psync_mutex_t;

psync_mutex_t psync_mutex_create(void);
void psync_mutex_destroy(psync_mutex_t mutex);
psync_bool_t psync_mutex_obtain(psync_mutex_t mutex);
psync_bool_t psync_mutex_try(psync_mutex_t mutex);
void psync_mutex_release(psync_mutex_t mutex);


/** Semaphore **/

typedef struct psync_semaphore_t_ * psync_semaphore_t;

psync_semaphore_t psync_semaphore_create(int initial_count, int max_count);
void psync_semaphore_destroy(psync_semaphore_t semaphore);
psync_bool_t psync_semaphore_signal(psync_semaphore_t semaphore);
psync_bool_t psync_semaphore_wait(psync_semaphore_t semaphore);
psync_bool_t psync_semaphore_try(psync_semaphore_t semaphore);

/** Thread **/

#define PSYNC_THREAD_PRIORITY_DEFAULT (1 << ((sizeof(int) * 8) - 1))
#define PSYNC_THREAD_STACK_SIZE_DEFAULT 0

typedef struct psync_thread_t_ * psync_thread_t;
typedef void * (* psync_thread_entry_t)(void * user_data);

psync_thread_t psync_thread_create(psync_thread_entry_t thread_entry, void * user_data, int priority, unsigned int stack_size, char const * name);
void psync_thread_join(psync_thread_t thread, void ** return_value);
void psync_thread_exit(void * return_value);
void psync_thread_sleep(unsigned int microseconds);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _libpsync_h_ */