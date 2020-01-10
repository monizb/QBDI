#ifndef QBDI_OPTION_H_
#define QBDI_OPTION_H_

#include "stdint.h"

#ifdef __cplusplus
namespace QBDI {
#endif

typedef enum {
    _QBDI_EI(NO_OPT)                          = 0,          /*!< Default value */
    // general options between 0 and 23
    _QBDI_EI(DISABLE_FPR)                     = 1<<0,       /*!< Disable all operation on FPU (SSE, AVX, SIMD).
                                                                 Can increase performances if not used by the target */
    _QBDI_EI(DISABLE_OPTIONAL_FPR)            = 1<<1,       /*!< Disable context switch optimisation when the target
                                                                 execblock doesn't used FPR */

    // architecture specific option between 24 and 31
} Options;

_QBDI_ENABLE_BITMASK_OPERATORS(Options)

#ifdef __cplusplus
}
#endif

#endif /* QBDI_OPTION_H_ */
