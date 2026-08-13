#include "test.h"

#include "ek_static_alloc.h"
#include "ek_stack.h"
#include "ek_ringbuf.h"
#include "ek_picothread.h"
#include "ek_evoke.h"

EK_LOG_FILE_TAG("test_static_alloc.c");

static void _static_pt_cb(ek_pt_handle_t pt, void *arg)
{
    (void)arg;
    EK_PT_BEGIN(pt);
    EK_PT_END(pt);
}

static void _static_evoke_cb(ek_evoke_event_t *evt, void *arg)
{
    (void)evt;
    (void)arg;
}

EK_DEFINE_STACK(static_stack, int, 4);
EK_DEFINE_RINGBUF(static_ringbuf, int, 4);
EK_DEFINE_RINGBUF_SPSC(static_spsc, int, 5);
EK_DEFINE_PT_SEM(static_sem, 1);
EK_DEFINE_PT_MSG(static_msg, int, 3);
EK_DEFINE_PT(static_pt, "pt", _static_pt_cb, 10, NULL);
EK_DEFINE_EVOKE_EVENT(static_event, "event", 0);
EK_DEFINE_EVOKE_TASK(static_evoke_task, "event_task", _static_evoke_cb, NULL);

static void _fail(const char *msg)
{
    EK_LOG_ERROR("%s", msg);
    exit(1);
}

void static_alloc_test(void)
{
    EK_LOG_INFO("static alloc test start");

    if (ek_static_alloc_get_init_error() != EK_ERR_NONE)
    {
        _fail("static alloc init failed");
    }

    if (static_stack.buffer != static_stack_storage || static_stack.cap != 4U)
    {
        _fail("static stack not initialized");
    }
    if (static_ringbuf.buffer != (uint8_t *)static_ringbuf_storage || static_ringbuf.cap != 4U)
    {
        _fail("static ringbuf not initialized");
    }
    if (static_spsc.buffer != (uint8_t *)static_spsc_storage || static_spsc.cap != 5U)
    {
        _fail("static spsc not initialized");
    }
    if (static_msg.rb.buffer != (uint8_t *)static_msg_storage || static_msg.rb.cap != 3U)
    {
        _fail("static msg not initialized");
    }
    if (static_pt.cb != _static_pt_cb || static_pt.prio != 10U)
    {
        _fail("static pt not initialized");
    }
    if (static_sem.count != 1U)
    {
        _fail("static sem not initialized");
    }
    if (static_event.name == NULL || static_evoke_task.cb != _static_evoke_cb)
    {
        _fail("static evoke objects not initialized");
    }

    const size_t unused_before = ek_heap_unused();

    for (int i = 0; i < 4; i++)
    {
        if (ek_stack_push(&static_stack, &i) != EK_ERR_NONE) _fail("static stack push failed");
        if (ek_ringbuf_write(&static_ringbuf, &i) != EK_ERR_NONE) _fail("static ringbuf write failed");
        if (ek_ringbuf_write_spsc(&static_spsc, &i) != EK_ERR_NONE) _fail("static spsc write failed");
    }

    int extra = 4;
    if (ek_stack_push(&static_stack, &extra) != EK_ERR_FULL) _fail("static stack should be full");
    if (ek_ringbuf_write(&static_ringbuf, &extra) != EK_ERR_FULL) _fail("static ringbuf should be full");
    if (ek_ringbuf_write_spsc(&static_spsc, &extra) != EK_ERR_FULL) _fail("static spsc should be full");

    for (int i = 0; i < 4; i++)
    {
        int stack_item = -1;
        int ring_item = -1;
        int spsc_item = -1;

        if (ek_stack_pop(&static_stack, &stack_item) != EK_ERR_NONE) _fail("static stack pop failed");
        if (ek_ringbuf_read(&static_ringbuf, &ring_item) != EK_ERR_NONE) _fail("static ringbuf read failed");
        if (ek_ringbuf_read_spsc(&static_spsc, &spsc_item) != EK_ERR_NONE) _fail("static spsc read failed");

        if (stack_item != (3 - i)) _fail("static stack value mismatch");
        if (ring_item != i) _fail("static ringbuf value mismatch");
        if (spsc_item != i) _fail("static spsc value mismatch");
    }

    if (ek_heap_unused() != unused_before)
    {
        _fail("static alloc test used heap");
    }

    ek_pt_deinit_static(&static_pt);

    EK_LOG_INFO("static alloc test passed, unused heap:%u", (unsigned)ek_heap_unused());
}
