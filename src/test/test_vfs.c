#include "test.h"
#include "ek/vfs/ek_vfs_core.h"
#include "ek/core/ek_log.h"

EK_LOG_MODULE(__FILE_NAME__, EK_LOG_LEVEL_NONE);

void vfs_test(void)
{
    int err;
    ek_vfs_ops_t ops = { 0 };

    err = ek_vfs_register("/dev/a", EK_VFS_DEV, &ops, NULL);
    if (err < 0) EK_LOG_ERROR("%s.", strerror(-err));
    err = ek_vfs_register("/dev/a", EK_VFS_DEV, &ops, NULL);
    if (err < 0) EK_LOG_ERROR("%s.", strerror(-err));

    err = ek_vfs_register("/dev/b", EK_VFS_DEV, &ops, NULL);
    if (err < 0) EK_LOG_ERROR("%s.", strerror(-err));

    err = ek_vfs_register("/dev/c", EK_VFS_DEV, &ops, NULL);
    if (err < 0) EK_LOG_ERROR("%s.", strerror(-err));

    err = ek_vfs_register("/dev/d/e/f", EK_VFS_DEV, &ops, NULL);
    if (err < 0) EK_LOG_ERROR("%s.", strerror(-err));

    err = ek_vfs_register("/fs", EK_VFS_DEV, &ops, NULL);
    if (err < 0) EK_LOG_ERROR("%s.", strerror(-err));

    EK_LOG_INFO("OK");

    char buf[1024] = { 0 };
    ek_vfs_tree_dump(buf, 1024);
    printf("%s\n", buf);

    err = ek_vfs_unregister("/");
    if (err < 0) EK_LOG_ERROR("%s.", strerror(-err));

    EK_LOG_INFO("available node: %d", ek_vfs_node_avail());
}
