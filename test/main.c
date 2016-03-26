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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static volatile int test_thread_counter /* = 0 */;

static void test_mutex(void);
static void test_semaphore(void);
static void test_thread(void);
static void * test_thread_entry(void * user_data);

void test_mutex(void)
{
	psync_bool_t res;
	psync_mutex_t mutex;

	printf("testing mutex\n");

	mutex = psync_mutex_create();
	assert(mutex);

	res = psync_mutex_obtain(mutex);
	assert(res);

	psync_mutex_release(mutex);

	psync_mutex_destroy(mutex);
}

void test_semaphore(void)
{
	psync_bool_t res;
	psync_semaphore_t semaphore;

	printf("testing semaphore\n");

	semaphore = psync_semaphore_create(0, 1);
	assert(semaphore);

	res = psync_semaphore_signal(semaphore);
	assert(res);

	res = psync_semaphore_wait(semaphore);
	assert(res);

	psync_semaphore_destroy(semaphore);
}

static void test_thread(void)
{
	psync_thread_t thread;
	void * return_value;

	printf("testing thread\n");

	thread = psync_thread_create(test_thread_entry, (void *)&thread, PSYNC_THREAD_PRIORITY_DEFAULT, PSYNC_THREAD_STACK_SIZE_DEFAULT, "test_thread");
	assert(thread);

	assert(test_thread_counter == 0);
	psync_thread_join(thread, &return_value);
	assert(test_thread_counter == 1);
	assert(return_value == (void *)&thread);
}

static void * test_thread_entry(void * user_data)
{
	printf(" inside test_thread_entry\n");
	++test_thread_counter;

	printf(" sleeping for 5 seconds\n");
	psync_thread_sleep(5 * 1000 * 1000);

	printf(" exiting\n");
	psync_thread_exit(user_data);

	assert(0);
	return user_data;
}

int main(void)
{
	test_mutex();
	test_semaphore();
	test_thread();

	printf("success\n");
	return EXIT_SUCCESS;
}
