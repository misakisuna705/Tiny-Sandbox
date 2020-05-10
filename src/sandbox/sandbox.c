#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include<limits.h>


#define SIZE 4096

#define LOAD_FUNC(func)                       \
    handler = dlopen("libc.so.6", RTLD_LAZY); \
    sys_##func = dlsym(handler, #func);       \

#define PRINT_PATH_LOG(path)                                                    \
    int fd = dup(STDIN_FILENO);                                                 \
    FILE * fp = fdopen(fd, "w");                                                \
                                                                                \
    fprintf(fp, "[sandbox] %s: access to %s is not allowed\n", __func__, path);

#define PRINT_CMD_LOG(cmd)                                            \
    int fd = dup(STDIN_FILENO);                                       \
    FILE * fp = fdopen(fd, "w");                                      \
                                                                      \
    fprintf(fp,"[sandbox] %s(\"%s\"): not allowed\n", __func__, cmd);

#define HIJACK_1_ARG_RET_INT(ret_type, func, arg_type, arg) \
    ret_type (* sys_##func)(arg_type);                      \
                                                            \
    ret_type func(arg_type arg) {                           \
        if (isvalid(arg)) {                                 \
            LOAD_FUNC(func)                                 \
                                                            \
            return sys_##func(arg);                         \
        } else {                                            \
            PRINT_PATH_LOG(arg)                             \
                                                            \
            return -1;                                      \
        }                                                   \
    }

#define HIJACK_1_ARG_RET_PTR(ret_type, func, arg_type, arg) \
    ret_type (* sys_##func)(arg_type);                      \
                                                            \
    ret_type func(arg_type arg) {                           \
        if (isvalid(arg)) {                                 \
            LOAD_FUNC(func)                                 \
                                                            \
            return sys_##func(arg);                         \
        } else {                                            \
            PRINT_PATH_LOG(arg)                             \
                                                            \
            return NULL;                                    \
        }                                                   \
    }


#define HIJACK_2_ARG_RET_INT(ret_type, func, arg1_type, arg1, arg2_type, arg2) \
    ret_type (* sys_##func)(arg1_type, arg2_type);                             \
                                                                               \
    ret_type func(arg1_type arg1, arg2_type arg2) {                            \
        if (isvalid(arg1)) {                                                   \
            LOAD_FUNC(func)                                                    \
                                                                               \
            return sys_##func(arg1, arg2);                                     \
        } else {                                                               \
            PRINT_PATH_LOG(arg1)                                               \
                                                                               \
            return -1;                                                         \
        }                                                                      \
    }

#define HIJACK_2_ARG_RET_PTR(ret_type, func, arg1_type, arg1, arg2_type, arg2) \
    ret_type (* sys_##func)(arg1_type, arg2_type);                             \
                                                                               \
    ret_type func(arg1_type arg1, arg2_type arg2) {                            \
        if (isvalid(arg1)) {                                                   \
            LOAD_FUNC(func)                                                    \
                                                                               \
            return sys_##func(arg1, arg2);                                     \
        } else {                                                               \
            PRINT_PATH_LOG(arg1)                                               \
                                                                               \
            return NULL;                                                       \
        }                                                                      \
    }

#define HIJACK_2_ARG_RET_INT_2_PATH(ret_type, func, arg1_type, arg1, arg2_type, arg2) \
    ret_type (* sys_##func)(arg1_type, arg2_type);                                    \
                                                                                      \
    ret_type func(arg1_type arg1, arg2_type arg2) {                                   \
        if (isvalid(arg1) && isvalid(arg2)) {                                         \
            LOAD_FUNC(func)                                                           \
                                                                                      \
            return sys_##func(arg1, arg2);                                            \
        } else {                                                                      \
            PRINT_PATH_LOG(arg1)                                                      \
                                                                                      \
            return -1;                                                                \
        }                                                                             \
    }

#define HIJACK_3_ARG_RET_INT(ret_type, func, arg1_type, arg1, arg2_type, arg2, arg3_type, arg3, path) \
    ret_type (* sys_##func)(arg1_type, arg2_type, arg3_type);                                         \
                                                                                                      \
    ret_type func(arg1_type arg1, arg2_type arg2, arg3_type arg3) {                                   \
        if (isvalid(path)) {                                                                          \
            LOAD_FUNC(func)                                                                           \
                                                                                                      \
            return sys_##func(arg1, arg2, arg3);                                                      \
        } else {                                                                                      \
            PRINT_PATH_LOG(path)                                                                      \
                                                                                                      \
            return -1;                                                                                \
        }                                                                                             \
    }

bool isvalid(const char * path);

#define HIJACK_4_ARG_RET_INT(ret_type, func, arg1_type, arg1, arg2_type, arg2, arg3_type, arg3, arg4_type, arg4) \
    ret_type (* sys_##func)(arg1_type, arg2_type, arg3_type, arg4_type);                                         \
                                                                                                                 \
    ret_type func(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4) {                              \
        if (isvalid(arg2)) {                                                                                     \
            LOAD_FUNC(func)                                                                                      \
                                                                                                                 \
            return sys_##func(arg1, arg2, arg3, arg4);                                                           \
        } else {                                                                                                 \
            PRINT_PATH_LOG(arg2)                                                                                 \
                                                                                                                 \
            return -1;                                                                                           \
        }                                                                                                        \
    }

#define HIJACK_EXEC(ret_type, func, arg_type, arg, ...) \
    ret_type func(arg_type arg, ##__VA_ARGS__) {       \
        PRINT_CMD_LOG(arg)                    \
                                               \
        return -1;\
    }

static void * handler;

bool isvalid(const char * path);

HIJACK_1_ARG_RET_INT(int,     chdir, const char *, path)
HIJACK_1_ARG_RET_INT(int,     rmdir, const char *, pathname)
HIJACK_1_ARG_RET_INT(int,    remove, const char *, pathname)
HIJACK_1_ARG_RET_INT(int,    unlink, const char *, pathname)

HIJACK_1_ARG_RET_PTR(DIR *, opendir, const char *, name)

HIJACK_2_ARG_RET_INT(int, mkdir,   const char *, pathname, mode_t, mode)
HIJACK_2_ARG_RET_INT(int, creat,   const char *, pathname, mode_t, mode)
HIJACK_2_ARG_RET_INT(int, creat64, const char *, pathname, mode_t, mode)
HIJACK_2_ARG_RET_INT(int, chmod,   const char *, pathname, mode_t, mode)

HIJACK_2_ARG_RET_PTR(FILE *,   fopen, const char *, pathname, const char *, mode)
HIJACK_2_ARG_RET_PTR(FILE *, fopen64, const char *, pathname, const char *, mode)

HIJACK_2_ARG_RET_INT_2_PATH(int,  rename, const char *, oldpath, const char *, newpath)
HIJACK_2_ARG_RET_INT_2_PATH(int, symlink, const char *,  target, const char *, linkpath)
HIJACK_2_ARG_RET_INT_2_PATH(int,    link, const char *, oldpath, const char *, newpath)

HIJACK_3_ARG_RET_INT(int,         open, const char *, pathname,          int, flags,        mode_t,     mode, pathname)
HIJACK_3_ARG_RET_INT(int,       open64, const char *, pathname,          int, flags,        mode_t,     mode, pathname)
HIJACK_3_ARG_RET_INT(int,        chown, const char *, pathname,        uid_t, owner,         gid_t,    group, pathname)
HIJACK_3_ARG_RET_INT(ssize_t, readlink, const char *, pathname,       char *,   buf,        size_t,   bufsiz, pathname)
HIJACK_3_ARG_RET_INT(int,      __xstat,          int,      ver, const char *,  path, struct stat *, stat_buf, path)
HIJACK_3_ARG_RET_INT(int,    __xstat64,          int,      ver, const char *,  path, struct stat *, stat_buf, path)

HIJACK_4_ARG_RET_INT(int, openat,   int, dirfd, const char *, pathname, int, flags, mode_t, mode);
HIJACK_4_ARG_RET_INT(int, openat64, int, dirfd, const char *, pathname, int, flags, mode_t, mode);

HIJACK_EXEC(int, system, const char *,  command)
HIJACK_EXEC(int,  execv, const char *,     path, char * const argv[])
HIJACK_EXEC(int, execvp, const char *,     file, char * const argv[])
HIJACK_EXEC(int, execve, const char *, filename, char * const argv[], char * const envp[])
HIJACK_EXEC(int,  execl, const char *,     path,    const char * arg, ...)
HIJACK_EXEC(int, execlp, const char *,     file,    const char * arg, ...)
HIJACK_EXEC(int, execle, const char *,     path,    const char * arg, ...)

bool isvalid(const char * path) {
    char * sandbox_dir = getenv("SANDBOX_DIR");
    char sandbox_path[SIZE] = "";
    char target_path[SIZE] = "";

    realpath(sandbox_dir, sandbox_path);
    realpath(path, target_path);

    if (!strncmp(sandbox_path, target_path, strlen(sandbox_path))) {
        return true;
    } else {
        return false;
    }
}
