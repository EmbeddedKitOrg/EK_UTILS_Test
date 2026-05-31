#ifndef TEST_H
#define TEST_H

#include "main.h"

#include "ek_log.h"
#include "ek_heap.h"
#include "ek_export.h"

#define PI (3.1415926f)

typedef struct
{
    uint8_t name;
    float score;
} student_t;

void picothread_test_init(void);
void ringbuf_test(void);
void stack_test(void);
void str_test(void);
void vec_test(void);

#endif // TEST_H
