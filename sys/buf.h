/*
 * sys/buf.h: buffer structures.
 */

#ifndef __sys_buf_h
#define __sys_buf_h

typedef struct buffer {
    byte    *bf_ptr;
    size_t  bf_len;
} buffer;

typedef struct c_buffer {
    const byte  *bf_ptr;
    size_t      bf_len;
} c_buffer;

#define MKBUF(s) { bf_ptr: (const byte*)(s), bf_len: sizeof(s)-1 }

#endif
