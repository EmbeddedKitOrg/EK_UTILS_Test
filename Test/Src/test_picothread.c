#include "test.h"
#include "ek_picothread.h"

EK_LOG_FILE_TAG("test_picothread.c");

/* ========================================================================
 * 信号量测试
 *
 * 场景：二元信号量 (count=1)，两个任务竞争 + 一个超时演示任务。
 * sem_taskA 和 sem_taskB 交替获取/释放信号量，验证互斥与优先级轮转。
 * sem_timeout 尝试 500 tick 内获取信号量，超时则上报；若成功则短暂持有后释放。
 * ======================================================================== */

#if EKCFG_PICOTHREAD_SEM == 1

static void _sem_taskA(ek_pt_handle_t pt, void *arg);
static void _sem_taskB(ek_pt_handle_t pt, void *arg);
static void _sem_timeout_demo(ek_pt_handle_t pt, void *arg);

static ek_pt_handle_t s_semA;
static ek_pt_handle_t s_semB;
static ek_pt_handle_t s_sem_timeout;
static ek_pt_sem_handle_t s_test_sem;

#endif /* EKCFG_PICOTHREAD_SEM */

/* ========================================================================
 * 消息队列测试
 *
 * 场景：容量 3 的 int 环形队列，三个任务：
 *   - msg_sender:       每 500 tick 发送一条递增消息
 *   - msg_receiver:     持续接收并打印消息内容
 *   - msg_sender_fast:  无延迟连续发送直到队列满，触发发送阻塞
 * 验证：缓冲区满时发送阻塞、缓冲区空时接收阻塞、唤醒后数据正确传递。
 * ======================================================================== */

#if EKCFG_PICOTHREAD_MSG == 1

static void _msg_sender(ek_pt_handle_t pt, void *arg);
static void _msg_receiver(ek_pt_handle_t pt, void *arg);
static void _msg_sender_fast(ek_pt_handle_t pt, void *arg);

static ek_pt_handle_t s_sender;
static ek_pt_handle_t s_receiver;
static ek_pt_handle_t s_sender_fast;
static ek_pt_msg_handle_t s_test_msg;

#endif /* EKCFG_PICOTHREAD_MSG */

/* ========================================================================
 * picothread_test_init
 * ======================================================================== */

void picothread_test_init(void)
{
    // #if EKCFG_PICOTHREAD_SEM == 1
    //     s_test_sem = ek_pt_sem_create(1);
    //     s_semA = ek_pt_create("sem_taskA", _sem_taskA, 10, NULL);
    //     s_semB = ek_pt_create("sem_taskB", _sem_taskB, 10, NULL);
    //     s_sem_timeout = ek_pt_create("sem_timeout", _sem_timeout_demo, 10, NULL);

    //     EK_LOG_INFO("[sem] created: count=1, tasks=sem_taskA, sem_taskB, sem_timeout");
    // #endif

#if EKCFG_PICOTHREAD_MSG == 1
    s_test_msg = ek_pt_msg_create(sizeof(int), 3);
    s_sender = ek_pt_create("msg_sender", _msg_sender, 10, NULL);
    s_receiver = ek_pt_create("msg_receiver", _msg_receiver, 10, NULL);
    s_sender_fast = ek_pt_create("msg_sender_fast", _msg_sender_fast, 10, NULL);

    EK_LOG_INFO("[msg] created: item_size=%u, capacity=3, tasks=msg_sender, msg_receiver, msg_sender_fast",
                (unsigned)sizeof(int));
#endif

    EK_LOG_INFO("picothread test init done");
}

/* ========================================================================
 * 信号量测试任务
 * ======================================================================== */

#if EKCFG_PICOTHREAD_SEM == 1

static void _sem_taskA(ek_pt_handle_t pt, void *arg)
{
    (void)arg;
    static unsigned iter = 0;
    EK_PT_BEGIN(pt);
    while (1)
    {
        ek_err_t err;

        EK_LOG_INFO("[semA] iter=%u: waiting on semaphore...", iter);
        EK_PT_SEM_TAKE(s_test_sem, (uint32_t)-1, err);
        (void)err;
        EK_LOG_INFO("[semA] iter=%u: ACQUIRED semaphore (err=%u)", iter, err);

        EK_PT_DELAY(300);
        EK_PT_SEM_GIVE(s_test_sem);
        EK_LOG_INFO("[semA] iter=%u: RELEASED semaphore", iter);

        iter++;
        EK_PT_DELAY(100);
    }
    EK_PT_END(pt);
}

static void _sem_taskB(ek_pt_handle_t pt, void *arg)
{
    (void)arg;
    static unsigned iter = 0;
    EK_PT_BEGIN(pt);
    while (1)
    {
        ek_err_t err;

        EK_LOG_INFO("[semB] iter=%u: waiting on semaphore...", iter);
        EK_PT_SEM_TAKE(s_test_sem, (uint32_t)-1, err);
        (void)err;
        EK_LOG_INFO("[semB] iter=%u: ACQUIRED semaphore (err=%u)", iter, err);

        EK_PT_DELAY(300);
        EK_PT_SEM_GIVE(s_test_sem);
        EK_LOG_INFO("[semB] iter=%u: RELEASED semaphore", iter);

        iter++;
        EK_PT_DELAY(100);
    }
    EK_PT_END(pt);
}

// 信号量超时演示
static void _sem_timeout_demo(ek_pt_handle_t pt, void *arg)
{
    (void)arg;
    static unsigned iter = 0;
    static unsigned n_ok = 0;
    static unsigned n_to = 0;
    EK_PT_BEGIN(pt);
    while (1)
    {
        ek_err_t err;

        EK_LOG_INFO("[timeout] iter=%u: waiting on semaphore (deadline=500 tick)...", iter);
        EK_PT_SEM_TAKE(s_test_sem, 500, err);

        if (err == EK_ERR_TIMEOUT)
        {
            n_to++;
            EK_LOG_INFO("[timeout] iter=%u: TIMEOUT (n_ok=%u, n_timeout=%u)", iter, n_ok, n_to);
            EK_PT_DELAY(2000);
        }
        else
        {
            n_ok++;
            EK_LOG_INFO("[timeout] iter=%u: ACQUIRED semaphore (n_ok=%u, n_timeout=%u)", iter, n_ok, n_to);
            EK_PT_DELAY(100);
            EK_PT_SEM_GIVE(s_test_sem);
            EK_LOG_INFO("[timeout] iter=%u: RELEASED semaphore", iter);
        }

        iter++;
    }
    EK_PT_END(pt);
}

#endif /* EKCFG_PICOTHREAD_SEM */

/* ========================================================================
 * 消息队列测试任务
 * ======================================================================== */

#if EKCFG_PICOTHREAD_MSG == 1

// 标准发送者：每 500 tick 发送一条消息
static void _msg_sender(ek_pt_handle_t pt, void *arg)
{
    (void)arg;
    static int counter = 0;
    static unsigned iter = 0;
    EK_PT_BEGIN(pt);
    while (1)
    {
        ek_err_t err;

        EK_LOG_INFO("[sender] iter=%u, val=%d: attempting send...", iter, counter);
        EK_PT_MSG_SEND(s_test_msg, &counter, (uint32_t)-1, err);
        (void)err;
        EK_LOG_INFO("[sender] iter=%u: SENT val=%d (err=%u)", iter, counter, err);

        counter++;
        iter++;
        EK_PT_DELAY(500);
    }
    EK_PT_END(pt);
}

// 标准接收者
static void _msg_receiver(ek_pt_handle_t pt, void *arg)
{
    (void)arg;
    int data;
    static unsigned iter = 0;
    EK_PT_BEGIN(pt);
    while (1)
    {
        ek_err_t err;

        EK_LOG_INFO("[recver] iter=%u: attempting recv...", iter);
        EK_PT_MSG_RECV(s_test_msg, &data, (uint32_t)-1, err);
        EK_LOG_INFO("[recver] iter=%u: RECEIVED val=%d (err=%u)", iter, data, err);

        iter++;
        EK_PT_DELAY(100);
    }
    EK_PT_END(pt);
}

// 快速发送者：无延迟连续发送，验证发送阻塞与唤醒
static void _msg_sender_fast(ek_pt_handle_t pt, void *arg)
{
    (void)arg;
    static int fast_counter = 100;
    static unsigned iter = 0;
    EK_PT_BEGIN(pt);
    while (1)
    {
        ek_err_t err;

        EK_LOG_INFO("[fast_sender] iter=%u, val=%d: attempting send...", iter, fast_counter);
        EK_PT_MSG_SEND(s_test_msg, &fast_counter, (uint32_t)-1, err);
        (void)err;
        EK_LOG_INFO("[fast_sender] iter=%u: SENT val=%d (err=%u)", iter, fast_counter, err);

        fast_counter++;
        iter++;
        // 无延迟：下一轮循环立即尝试发送，直到队列满阻塞
    }
    EK_PT_END(pt);
}

#endif /* EKCFG_PICOTHREAD_MSG */
