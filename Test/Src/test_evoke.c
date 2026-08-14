#include "test.h"
#include "ek_evoke.h"

EK_LOG_FILE_TAG("test_evoke.c");

static void _static_evoke_cb(ek_evoke_event_t *evt, void *arg)
{
    __EK_UNUSED(evt);
    __EK_UNUSED(arg);
}

EK_DEFINE_EVOKE_EVENT(static_event, 0);
EK_DEFINE_EVOKE_TASK(static_evoke_task, _static_evoke_cb, NULL);

void evoke_test(void)
{
    EK_LOG_INFO("evoke static test start");

    TEST_ASSERT(ek_static_alloc_get_init_error() == EK_ERR_NONE, "static alloc init failed");
    TEST_ASSERT_EQ(static_event.count, 0U, "static event count mismatch");
    TEST_ASSERT(static_evoke_task.cb == _static_evoke_cb, "static evoke task cb mismatch");

    const size_t unused_before = ek_heap_unused();
    TEST_ASSERT_EQ(ek_heap_unused(), unused_before, "static evoke used heap");

    EK_LOG_INFO("static evoke test passed, unused heap:%u", (unsigned)ek_heap_unused());
}
