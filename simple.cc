#include <cassert>
#include <cstdlib>
#include <cstring>
#include <thread>

int b;
int *x = &b;

int main(void) {
  *x = 1;

  auto t = std::thread([] {
    while (true) {
      memset(x, 2, sizeof(*x));
    }
  });

  int r;
  while (true) {
    memcpy(&r, x, sizeof(*x));
  }

  t.join();

  return r;
}
