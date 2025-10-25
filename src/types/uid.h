#pragma once

#include <types/types.h>

/**
 * @brief
 *
 * @api
 */
typedef u64 UID;

void uid_init(void);
void uid_exit(void);

/**
 * @brief
 *
 * @api
 */
UID uid_new(void);
