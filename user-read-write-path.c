#include <stdatomic.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>

static const size_t BUFFER_SIZE = 4096;
static const size_t BUFFER_ALIGNMENT = 4096;
static const size_t N_THREADS = 4;

static char *shared_buffer;
atomic_int running = 1;

void *writer_thread(void *arg) {
  unsigned char pattern = 0;
  while (atomic_load_explicit(&running, memory_order_relaxed)) {
    memset(shared_buffer, pattern, BUFFER_SIZE);
    pattern++;
  }
  return NULL;
}

void *reader_thread(void *arg) {
  char *private_buffer;
  int r = posix_memalign((void **)&private_buffer, BUFFER_ALIGNMENT, BUFFER_SIZE);
  assert(r == 0);

  while (atomic_load_explicit(&running, memory_order_relaxed)) {
    memcpy(private_buffer, shared_buffer, BUFFER_SIZE);
  }

  free(private_buffer);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t writer_tid;
  pthread_t reader_tids[N_THREADS];

  int r = posix_memalign((void **)&shared_buffer, BUFFER_ALIGNMENT, BUFFER_SIZE);
  memset(shared_buffer, 0, BUFFER_SIZE);

  r = pthread_create(&writer_tid, NULL, writer_thread, NULL);
  assert(r == 0);

  for (size_t i = 0; i < N_THREADS; i++) {
    r = pthread_create(&reader_tids[i], NULL, reader_thread, NULL);
    assert(r == 0);
  }

  sleep(1);
  atomic_store_explicit(&running, 0, memory_order_relaxed);

  pthread_join(writer_tid, NULL);
  for (size_t i = 0; i < N_THREADS; i++)
    pthread_join(reader_tids[i], NULL);

  free(shared_buffer);

  return 0;
}
