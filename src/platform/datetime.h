#pragma once

#include <types/types.h>

/**
 * DateTime handler for get UTC and local date and time
 *
 * @api
 */
typedef struct datetime_handle* datetime_handle;

/**
 * @brief Create time instance and init with current time
 * @warning You should free datetime_handle with 'time_free'
 *
 * @api
 */
datetime_handle datetime_new(void);

/**
 * @brief free datetime_handle
 *
 * @api
 */
void datetime_free(datetime_handle handle);

/**
 * @brief Update existed time instance
 *
 * @api
 */
void datetime_update(datetime_handle handle);

/**
 * @brief get local year
 *
 * @api
 */
u16 datetime_get_year(datetime_handle handle);

/**
 * @brief get local month
 *
 * @api
 */
u8 datetime_get_month(datetime_handle handle);

/**
 * @brief get local day
 *
 * @api
 */
u8 datetime_get_day(datetime_handle handle);

/**
 * @brief get local hour
 *
 * @api
 */
u8 datetime_get_hour(datetime_handle handle);

/**
 * @brief get local minute
 *
 * @api
 */
u8 datetime_get_minute(datetime_handle handle);

/**
 * @brief get local second
 *
 * @api
 */
u8 datetime_get_second(datetime_handle handle);

/**
 * @brief get local nanosecond
 *
 * @api
 */
u32 datetime_get_nanosecond(datetime_handle handle);

/* ======================> UTC <====================== */
/**
 * @brief get UTC year
 *
 * @api
 */
u16 datetime_get_utc_year(datetime_handle handle);

/**
 * @brief get UTC month
 *
 * @api
 */
u8 datetime_get_utc_month(datetime_handle handle);

/**
 * @brief get UTC day
 *
 * @api
 */
u8 datetime_get_utc_day(datetime_handle handle);

/**
 * @brief get UTC hour
 *
 * @api
 */
u8 datetime_get_utc_hour(datetime_handle handle);

/**
 * @brief get UTC minute
 *
 * @api
 */
u8 datetime_get_utc_minute(datetime_handle handle);

/**
 * @brief get UTC second
 *
 * @api
 */
u8 datetime_get_utc_second(datetime_handle handle);
