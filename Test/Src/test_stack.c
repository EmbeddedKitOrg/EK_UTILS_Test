#include "test.h"
#include "ek_stack.h"

EK_LOG_FILE_TAG("test_stack.c");

EK_DEFINE_STACK(static_stack, int, 4);

void stack_test(void)
{
    TEST_ASSERT(ek_static_alloc_get_init_error() == EK_ERR_NONE, "static alloc init failed");
    TEST_ASSERT(static_stack.buffer == static_stack_storage, "static stack storage mismatch");
    TEST_ASSERT_EQ(static_stack.cap, 4U, "static stack cap mismatch");

    const size_t unused_before = ek_heap_unused();

    for (int i = 0; i < 4; i++)
    {
        TEST_ASSERT(ek_stack_push(&static_stack, &i) == EK_ERR_NONE, "static stack push failed");
    }

    int extra = 4;
    TEST_ASSERT(ek_stack_push(&static_stack, &extra) == EK_ERR_FULL, "static stack should be full");

    for (int i = 0; i < 4; i++)
    {
        int stack_item = -1;
        TEST_ASSERT(ek_stack_pop(&static_stack, &stack_item) == EK_ERR_NONE, "static stack pop failed");
        TEST_ASSERT_EQ(stack_item, (3 - i), "static stack value mismatch");
    }

    TEST_ASSERT_EQ(ek_heap_unused(), unused_before, "static stack used heap");
    EK_LOG_INFO("static stack test passed");

    EK_LOG_INFO("start creating a stack");
    const size_t unused_before_dyn = ek_heap_unused();
    ek_stack_t *stack = ek_stack_create(sizeof(student_t), 5);
    TEST_ASSERT(stack != NULL, "creating stack fail");
    TEST_ASSERT(stack->buffer == stack->data, "dynamic stack buffer not inline");
    EK_LOG_INFO("create successfully");

    student_t items[5];
    for (uint8_t i = 0; i < 5; i++)
    {
        items[i].name = i;
        items[i].score = i * PI;
        TEST_ASSERT(ek_stack_push(stack, &items[i]) == EK_ERR_NONE, "stack push failed");
        EK_LOG("i:%u push item{name:%u,score:%.2f} sp:%u", i, items[i].name, items[i].score, stack->sp);
    }

    student_t overflow = {.name = 99, .score = 99.0f};
    TEST_ASSERT(ek_stack_push(stack, &overflow) == EK_ERR_FULL, "stack should be full");

    for (uint8_t i = 0; i < 5; i++)
    {
        student_t item = {0};
        TEST_ASSERT(ek_stack_pop(stack, &item) == EK_ERR_NONE, "stack pop failed");
        TEST_ASSERT_EQ(item.name, (uint8_t)(4 - i), "stack pop name mismatch");
        EK_LOG("i:%u pop item{name:%u,score:%.2f} sp:%u", i, item.name, item.score, stack->sp);
    }

    student_t empty_item = {0};
    TEST_ASSERT(ek_stack_pop(stack, &empty_item) == EK_ERR_EMPTY, "stack should be empty");

    ek_stack_destroy(stack);
    TEST_ASSERT_EQ(ek_heap_unused(), unused_before_dyn, "stack destroy leaked memory");
    EK_LOG_INFO("stack test passed, unused heap:%u", (unsigned)ek_heap_unused());
}
