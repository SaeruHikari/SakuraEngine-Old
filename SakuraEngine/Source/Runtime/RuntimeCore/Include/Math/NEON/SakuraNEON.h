#pragma once

constexpr const uint32_t  cElementIndex[4]{1,2,4,8};

// NEON version of SSE _mm_movemask_ps
static inline uint32_t vmaskq_u32(uint32x4_t& CR)
{
    static const uint32x4_t Mask = vld1q_u32(cElementIndex);
        // extract element index bitmask from compare result.
    uint32x4_t vTemp = vandq_u32(CR, Mask);
    uint32x2_t vL = vget_low_u32(vTemp);    // get low 2 uint32 
    uint32x2_t vH = vget_high_u32(vTemp);  // get high 2 uint32
    vL = vorr_u32(vL, vH);  
    vL = vpadd_u32(vL, vL);
    return vget_lane_u32(vL, 0); 
}