#Memory model for the language

There is only ever one running process, all non-OS programs are loaded and excuted within userspace (why the language is a LISP)

The megabyte of RAM is the stack. The rest is the heap. Each stack entry is 1 kilobyte, starting with a four-byte integer that points to the RAM location it is executing.
