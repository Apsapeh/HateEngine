#pragma once

/*
 * Prefix H_ in defines is a HELPER_
 */


#define H_SET_SUCCESS_FALSE                                                                             \
    do {                                                                                                \
        if (success) {                                                                                  \
            *success = false;                                                                           \
        }                                                                                               \
    } while (0)

#define H_SET_SUCCESS_TRUE                                                                              \
    do {                                                                                                \
        if (success) {                                                                                  \
            *success = true;                                                                            \
        }                                                                                               \
    } while (0)
