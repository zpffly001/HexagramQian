#include <cassert>
#include <sstream>
#include <iomanip>

#include "Byte.h"
#include "Types.h"

/**
 * 类型转换函数
*/
Bit::operator unsigend char()
{
    return b;
}

Bit LittleEndianByte::BitPattern::operator[](int idx) const
{
    assert(idx >= 0 && idx <= 7);

#ifdef MEM_BIT_BIG_ENDIAN
    idx = 7 - idx;
#endif // MEM_BIT_BIG_ENDIAN

    switch (idx)
    {
        case 0:
            return b0;
        case 1:
            return b1;
        case 2:
            return b2;
        case 3:
            return b3;
        case 4:
            return b4;
        case 5:
            return b5;
        case 6:
            return b6;
        case 7:
            return b7;
        default:
            return b0;
    }
}

/**
 * 重载[]运算符
*/
Bit LittleEndianByte::operator[](int idx) const
{
    return bits[idx];
}

/**
 * 重载<<运算符
*/
std::ostream &operator<<(std::ostream &out, const LittleEndianByte &byte)
{
    for (int i = 7; i >= 0; i--)
    {
        out << byte.bits[i];
    }
    return out;
}

/**
 * 类型转换函数
*/
LittleEndianByte::operator unsigned char() const
{
    return val;
}

/**
 * 将字节转化为十六进制字符串
*/
std::string LittleEndianByte::toHexString() const
{
    std::stringstream sstream;
    std::string ret;
    sstream << std::setw(2) << std::setfill('0') << std::hex << (UInt) val;
    sstream >> ret;
    return ret;
}

/**
 * 重载<<运算符
*/
std::ostream &operator<<(std::ostream &out, const Bit &bit)
{
    out << (bit.b != 0);
    return out;
}

