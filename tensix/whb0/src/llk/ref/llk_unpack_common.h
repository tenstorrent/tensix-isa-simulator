// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ckernel_core.h"

BEGIN_NS_LLK

//
//    No other contents so far
//

inline void llk_setup_operands() {
    // originally in "llk_io_unpack.h"
    // nothing to do in similator case
    // input CBs are set via simulator framework
}

END_NS_LLK

