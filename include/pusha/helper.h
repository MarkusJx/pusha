#ifndef PUSHA_HELPER_H__
#define PUSHA_HELPER_H__

#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#   define PUSHA_WINDOWS
#endif //Windows

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

size_t host_path_separator(const char* endpoint, size_t endpoint_len, char** host_start);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PUSHA_HELPER_H__ */
