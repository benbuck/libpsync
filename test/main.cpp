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
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_mutex(void);
static void test_mutex_cxx(void);
static void test_semaphore(void);
static void test_semaphore_cxx(void);
static void test_thread(void);
static void test_thread_cxx(void);
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

void test_mutex_cxx(void)
{
	bool res;

	printf("testing mutex c++\n");

	psyncMutex mutex1;
	assert(mutex1.IsValid());

	psyncMutex mutex2(mutex1);
	assert(mutex2.IsValid());

	res = mutex1.Obtain();
	assert(res);

	res = mutex2.Try();
	assert(res);

	{
		psyncMutexAuto mutex3(mutex1);
		assert(mutex3.IsObtained());
	}

	mutex2.Release();

	mutex1.Release();
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

void test_semaphore_cxx(void)
{
	bool res;

	printf("testing semaphore c++\n");

	psyncSemaphore semaphore1(0, 1);
	assert(semaphore1.IsValid());

	psyncSemaphore semaphore2(semaphore1);
	assert(semaphore2.IsValid());

	res = semaphore1.Signal();
	assert(res);

	res = semaphore2.Wait();
	assert(res);

	res = semaphore2.Signal();
	assert(res);

	res = semaphore1.Wait();
	assert(res);
}

static void test_thread(void)
{
	psync_bool_t res;
	psync_thread_t thread;
	void * return_value;
	volatile int test_thread_counter;

	printf("testing thread\n");

	test_thread_counter = 0;
	thread = psync_thread_create(test_thread_entry, (void *)&test_thread_counter, NULL);
	assert(thread);

	res = psync_thread_join(thread, &return_value);
	assert(res);
	assert(test_thread_counter == 1);
	assert(return_value == (void *)&test_thread_counter);
}

static void test_thread_cxx(void)
{
	printf("testing thread c++\n");

	volatile int test_thread_counter = 0;
	psync_thread_param_t thread_param;
	memset(&thread_param, 0, sizeof(thread_param));
	thread_param.stack_size.relative = 2.0f;
	thread_param.stack_size.absolute = 16 * 1024;
	thread_param.priority.relative = 0.0f;
	thread_param.name = "test_thread";
	psyncThread thread(test_thread_entry, (void *)&test_thread_counter, &thread_param);
	assert(thread.IsValid());

	void * return_value;
	thread.Join(&return_value);
	assert(test_thread_counter == 1);
	assert(return_value == (void *)&test_thread_counter);
}

static void * test_thread_entry(void * user_data)
{
	int * test_thread_counter = (int *)user_data;

	printf(" inside test_thread_entry\n");
	++(*test_thread_counter);

	printf(" sleeping for 2 seconds\n");
	psync_thread_sleep(2 * 1000 * 1000);

	printf(" exiting\n");
	psync_thread_exit(user_data);

	assert(0);
	return user_data;
}

int main(void)
{
	test_mutex();
	test_mutex_cxx();
	test_semaphore();
	test_semaphore_cxx();
	test_thread();
	test_thread_cxx();

	printf("success\n");
	return EXIT_SUCCESS;
}
