#ifndef MBEDTLS_THREADING_ALT_H
#define MBEDTLS_THREADING_ALT_H

#include <stdlib.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"
#include "task.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        SemaphoreHandle_t mutex;
    } mbedtls_threading_mutex_t;

    void threading_mutex_init(mbedtls_threading_mutex_t *mutex);
    void threading_mutex_free(mbedtls_threading_mutex_t *mutex);
    int threading_mutex_lock(mbedtls_threading_mutex_t *mutex);
    int threading_mutex_unlock(mbedtls_threading_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_THREADING_ALT_H */
