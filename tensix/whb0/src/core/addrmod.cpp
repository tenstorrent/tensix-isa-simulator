
#include <cstdint>
#include <cassert>

#include "core/instr_params.hpp"
#include "core/tensix.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace core {

//
//    Tensix
//

//
//    SETADC: Set address counter for one channel and one dimension,
//        but supports full address counter precision
//
//    cnt_set_mask       001 - unpacker0, 010 - unpacker1, 100 - packer0 
//                           p_setadc::UNP0
//                           p_setadc::UNP1
//                           p_setadc::PAC
//    channel_index      0 - channel 0, 1 - channel 1
//                           p_setadc::CH_0
//                           p_setadc::CH_1
//    dimension_index    0 for X, 1 for Y, 2 for Z, 3 for W 
//                           p_setadc::SET_X
//                           p_setadc::SET_Y
//                           p_setadc::SET_Z
//                           p_setadc::SET_W
//    value              Address counter value
//

void Tensix::SETADC(
        uint32_t cnt_set_mask,
        uint32_t channel_index,
        uint32_t dimension_index,
        uint32_t value) {
#define KEY(c, d) ((uint32_t(c) << 2) | uint32_t(d))
    for (int i = 0; i < 3; i++) {
        uint32_t mask = uint32_t(1) << i;
        if ((cnt_set_mask & mask) == 0) {
            continue;
        }
        AddrCtrl &adc = m_adc[i];
        switch (KEY(channel_index, dimension_index)) {
        case KEY(p_setadc::CH_0, p_setadc::SET_X):
            adc.ch0_x = value;
            adc.ch0_x_cr = value;
            break;
        case KEY(p_setadc::CH_0, p_setadc::SET_Y):
            adc.ch0_y = value;
            adc.ch0_y_cr = value;
            break;
        case KEY(p_setadc::CH_0, p_setadc::SET_Z):
            adc.ch0_z = value;
            adc.ch0_z_cr = value;
            break;
        case KEY(p_setadc::CH_0, p_setadc::SET_W):
            adc.ch0_w = value;
            adc.ch0_w_cr = value;
            break;
        case KEY(p_setadc::CH_1, p_setadc::SET_X):
            adc.ch1_x = value;
            adc.ch1_x_cr = value;
            break;
        case KEY(p_setadc::CH_1, p_setadc::SET_Y):
            adc.ch1_y = value;
            adc.ch1_y_cr = value;
            break;
        case KEY(p_setadc::CH_1, p_setadc::SET_Z):
            adc.ch1_z = value;
            adc.ch1_z_cr = value;
            break;
        case KEY(p_setadc::CH_1, p_setadc::SET_W):
            adc.ch1_w = value;
            adc.ch1_w_cr = value;
            break;
        default:
            assert(false);
            break;
        }
    }
#undef KEY
}

//
//    TODO: Figure out what happens if 'cnt_set_mask' is not explicitly set for Ch1.
//        Will values specified for Ch0 be used or the setting will be skipped?
//

//
//    SETADCXY: Set address counters for X and Y dimensions. Choose counter set based on the cnt_set_mask.
//
//    cnt_set_mask    001 - unpacker0, 010 - unpacker1, 100 - packer0 
//                        p_setadc::UNP0
//                        p_setadc::UNP1
//                        p_setadc::PAC
//    ch1_y           Ch1_Y value
//    ch1_x           Ch1_X value 
//    ch0_y           Ch0_Y value
//    ch0_x           Ch0_X value
//    bit_mask        bit mask (Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)
//

void Tensix::SETADCXY(
        uint32_t cnt_set_mask,
        uint32_t ch1_y,
        uint32_t ch1_x,
        uint32_t ch0_y,
        uint32_t ch0_x,
        uint32_t bit_mask) {
    for (int i = 0; i < 3; i++) {
        uint32_t mask = uint32_t(1) << i;
        if ((cnt_set_mask & mask) == 0) {
            continue;
        }
        AddrCtrl &adc = m_adc[i];
        if ((bit_mask & 0x1) != 0) {
            adc.ch0_x = ch0_x;
            adc.ch0_x_cr = ch0_x;
       }
        if ((bit_mask & 0x2) != 0) {
            adc.ch0_y = ch0_y;
            adc.ch0_y_cr = ch0_y;
        }
        if ((bit_mask & 0x4) != 0) {
            adc.ch1_x = ch1_x;
            adc.ch1_x_cr = ch1_x;
        }
        if ((bit_mask & 0x8) != 0) {
            adc.ch1_y = ch1_y;
            adc.ch1_y_cr = ch1_y;
        }
    }
}

//
//    INCADCXY: Increment address counters for X and Y dimensions, for both channels (0/1).
//        Channel 0 = src, Channel 1 = dest
//
//    cnt_set_mask    001 - unpacker0, 010 - unpacker1, 100 - packer0 
//                        p_setadc::UNP0
//                        p_setadc::UNP1
//                        p_setadc::PAC
//    ch1_y           Channel 1 Y value 
//    ch1_x           Channel 1 X value
//    ch0_y           Channel 0 Y value 
//    ch0_x           Channel 0 X value
//

void Tensix::INCADCXY(
        uint32_t cnt_set_mask,
        uint32_t ch1_y,
        uint32_t ch1_x,
        uint32_t ch0_y,
        uint32_t ch0_x) {
    for (int i = 0; i < 3; i++) {
        uint32_t mask = uint32_t(1) << i;
        if ((cnt_set_mask & mask) == 0) {
            continue;
        }
        AddrCtrl &adc = m_adc[i];
        adc.ch0_x += ch0_x;
        adc.ch0_y += ch0_y;
        adc.ch1_x += ch1_x;
        adc.ch1_y += ch1_y;
    }
}

//
//    ADDRCRXY: Address carriage return
//
//    Arguments: see SETADCXY above
//

void Tensix::ADDRCRXY(
        uint32_t cnt_set_mask,
        uint32_t ch1_y,
        uint32_t ch1_x,
        uint32_t ch0_y,
        uint32_t ch0_x,
        uint32_t bit_mask) {
    for (int i = 0; i < 3; i++) {
        uint32_t mask = uint32_t(1) << i;
        if ((cnt_set_mask & mask) == 0) {
            continue;
        }
        AddrCtrl &adc = m_adc[i];
        if ((bit_mask & 0x1) != 0) {
            adc.ch0_x_cr += ch0_x;
            adc.ch0_x = adc.ch0_x_cr;
        }
        if ((bit_mask & 0x2) != 0) {
            adc.ch0_y_cr += ch0_y;
            adc.ch0_y = adc.ch0_y_cr;
        }
        if ((bit_mask & 0x4) != 0) {
            adc.ch1_x_cr += ch1_x;
            adc.ch1_x = adc.ch1_x_cr;
        }
        if ((bit_mask & 0x8) != 0) {
            adc.ch1_y_cr += ch1_y;
            adc.ch1_y = adc.ch1_y_cr;
        }
    }
}

//
//    SETADCZW: Set address counters for X and Y dimensions. Choose counter set based on the CntSetMask.
//
//    cnt_set_mask    001 - unpacker0, 010 - unpacker1, 100 - packer0 
//                        p_setadc::UNP0
//                        p_setadc::UNP1
//                        p_setadc::PAC
//    ch1_w           Ch1_W value
//    ch1_z           Ch1_Z value 
//    ch0_w           Ch0_W value
//    ch0_z           Ch0_Z value
//    bit_mask        bit mask (Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)
//

void Tensix::SETADCZW(
        uint32_t cnt_set_mask,
        uint32_t ch1_w,
        uint32_t ch1_z,
        uint32_t ch0_w,
        uint32_t ch0_z,
        uint32_t bit_mask) {
    for (int i = 0; i < 3; i++) {
        uint32_t mask = uint32_t(1) << i;
        if ((cnt_set_mask & mask) == 0) {
            continue;
        }
        AddrCtrl &adc = m_adc[i];
        if ((bit_mask & 0x1) != 0) {
            adc.ch0_z = ch0_z;
            adc.ch0_z_cr = ch0_z;
        }
        if ((bit_mask & 0x2) != 0) {
            adc.ch0_w = ch0_w;
            adc.ch0_w_cr = ch0_w;
        }
        if ((bit_mask & 0x4) != 0) {
            adc.ch1_z = ch1_z;
            adc.ch1_z_cr = ch1_z;
        }
        if ((bit_mask & 0x8) != 0) {
            adc.ch1_w = ch1_w;
            adc.ch1_w_cr = ch1_w;
        }
    }
}

//
//    INCADCZW: Increment address counters for Z and W dimensions, for both channels (0/1).
//        Channel 0 = src, Channel 1 = dest
//
//    cnt_set_mask    001 - unpacker0, 010 - unpacker1, 100 - packer0 
//                        p_setadc::UNP0
//                        p_setadc::UNP1
//                        p_setadc::PAC
//    ch1_w           Channel 1 W value 
//    ch1_z           Channel 1 Z value
//    ch0_w           Channel 0 W value 
//    ch0_z           Channel 0 Z value
//

void Tensix::INCADCZW(
        uint32_t cnt_set_mask,
        uint32_t ch1_w,
        uint32_t ch1_z,
        uint32_t ch0_w,
        uint32_t ch0_z) {
    for (int i = 0; i < 3; i++) {
        uint32_t mask = uint32_t(1) << i;
        if ((cnt_set_mask & mask) == 0) {
            continue;
        }
        AddrCtrl &adc = m_adc[i];
        adc.ch0_z += ch0_z;
        adc.ch0_w += ch0_w;
        adc.ch1_z += ch1_z;
        adc.ch1_w += ch1_w;
    }
}

//
//    ADDRCRZW: Address carriage return
//
//    Arguments: see SETADCZW above
//

void Tensix::ADDRCRZW(
        uint32_t cnt_set_mask,
        uint32_t ch1_w,
        uint32_t ch1_z,
        uint32_t ch0_w,
        uint32_t ch0_z,
        uint32_t bit_mask) {
    for (int i = 0; i < 3; i++) {
        uint32_t mask = uint32_t(1) << i;
        if ((cnt_set_mask & mask) == 0) {
            continue;
        }
        AddrCtrl &adc = m_adc[i];
        if ((bit_mask & 0x1) != 0) {
            adc.ch0_z_cr += ch0_z;
            adc.ch0_z = adc.ch0_z_cr;
        }
        if ((bit_mask & 0x2) != 0) {
            adc.ch0_w_cr += ch0_w;
            adc.ch0_w = adc.ch0_w_cr;
        }
        if ((bit_mask & 0x4) != 0) {
            adc.ch1_z_cr += ch1_z;
            adc.ch1_z = adc.ch1_z_cr;
        }
        if ((bit_mask & 0x8) != 0) {
            adc.ch1_w_cr += ch1_w;
            adc.ch1_w = adc.ch1_w_cr;
        }
    }
}

//
//    SETADCXX: Set start and end address counters in X dimension
//
//    cnt_set_mask    001 - unpacker0, 010 - unpacker1, 100 - packer0 
//    x_end2          x_end set value (inclusive)
//    x_start         x_start set value 
//

void Tensix::SETADCXX(
        uint32_t cnt_set_mask,
        uint32_t x_end2,
        uint32_t x_start) {
    for (int i = 0; i < 3; i++) {
        uint32_t mask = uint32_t(1) << i;
        if ((cnt_set_mask & mask) == 0) {
            continue;
        }
        // no setting for ch1_x?
        // set x_cr?
        AddrCtrl &adc = m_adc[i];
        adc.ch0_x = x_start;
        adc.x_end = x_end2;
    }
}

} // namespace core
} // namespace whb0
} // namespace iss
} // namespace ronin

