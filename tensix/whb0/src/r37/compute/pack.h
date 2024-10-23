#pragma once

BEGIN_NS_COMPUTE

inline void pack_tile(uint32_t ifrom_dst, uint32_t icb) {
    PACK((llk_pack<false, SYNC, false >(ifrom_dst, icb)));
}

inline void pack_reconfig_data_format(uint32_t new_operand) {
    // NOTE: For wormhole_b0, packer data format reconfig functions don't yet exist. So skip.
}

END_NS_COMPUTE

