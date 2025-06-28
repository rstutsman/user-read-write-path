Absolute minimum example of memcpy data race similar to kernel read/write path:
```
$ make && ./simple
...
==================
WARNING: ThreadSanitizer: data race (pid=1362645)
  Read of size 1 at 0x55b7ac6bd024 by main thread:
```

Is it possible to preserve concurrent memcpy with memset without UB and without a substantial performance hit?

A somewhat fuller example with a shared page-sized, page-aligned buffer:
```
$ make && ./user-read-write-path
...
==================
WARNING: ThreadSanitizer: data race (pid=1362792)
  Read of size 8 at 0x7b8000000000 by thread T2:
    #0 memcpy sanitizer_common_interceptors.inc:827 (libtsan.so.0+0x6243e)
```

Same questions for this fuller example. Is it possible to suppress tsan's data race detector while keeping efficient overlapping reads/writes e.g. using [`atomic_thread_fence`](https://www.cppreference.com/w/c/atomic/atomic_thread_fence.html)?
