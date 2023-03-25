#include "Int.h"

/******************************************UInt32*******************************************/

/**
* unsigned int类型的数据转化为4Byte
* Byte (&bytes)[4]表示访问一个由 bytes 数组的第四个元素开始的新数组
* &bytes 将 bytes 数组的地址作为一个指针返回，然后在这个指针上进行偏移量的计算。
* 因为 bytes 数组的类型是 char，所以这个偏移量是以字节为单位计算的。
*/
void UInt32::uintToBytes(unsigned int uintVal, Byte (&bytes)[4])
{
    for (int i = 0; i < 4; i++)
    {
        bytes[3 -i] = static_cast<unsigned char>(uintVal);
        /* 每次右移8位(1字节) */
        uintVal >>= (unsigned) 8;
    }
}

/**
 * 4Byte类型的数据转化为unsigned int
*/
static unsigned int byteToUint(const Byte (&bytes)[4])
{
    unsigned int ret = 0;
    for (int i = 0; i < 4; i++)
    {
        /* 和上面uintToBytes相反，取出每个元素后进行左移，并且每次都进行|或操作，是为了把4字节元素刚好放进去 */
        ret |= static_cast<unsigned int>(bytes[3 - i] << 8 * i);
    }
    return ret;
}

UInt32::UInt32(unsigned int val)
{
    uintToBytes(val, bytes);
}

UInt32::operator unsigned int() const
{
    return byteToUint(bytes);
}

std::vector<Byte> UInt32::toVector() const
{
    /* 把bytes从起始指针到后面的4个元素都放入vector中 */
    return std::vector<Byte>(bytes, bytes + 4);
}



/******************************************UInt16*******************************************/

void UInt16::uintToBytes(unsigned short uintVal, Byte (&bytes)[2]) {
    for (int i = 0; i < 2; i++) {
        bytes[1 - i] = static_cast<unsigned char>(uintVal);
        uintVal >>= (unsigned) 8;
    }
}

unsigned short UInt16::byteToUint(const Byte (&bytes)[2]) {
    unsigned short ret = 0;
    for (int i = 0; i < 2; i++) {
        ret |= static_cast<unsigned short>(bytes[1 - i] << 8 * i);
    }
    return ret;
}

UInt16::UInt16(unsigned short val) {
    uintToBytes(val, bytes);
}

UInt16::operator unsigned short() const {
    return byteToUint(bytes);
}

UInt16 &UInt16::operator=(unsigned short val) {
    uintToBytes(val, bytes);
    return *this;
}

std::vector<Byte> UInt16::toVector() const {
    return std::vector<Byte>(bytes, bytes + 2);
}



/******************************************IntUtil*******************************************/
/**
 * 计算32位无符号整数下一个2次幂
*/
UInt IntUtil::next32Pow2(UInt v)
{
    /* << 是位运算符中的左移运算符。对于两个无符号整数 a 和 b，表达式 a << b 的结果是将 a 左移 b 位后得到的新值。 */
    /* 表达式 1u << 1u  1u 的值为 0b00000001，将其左移一位后得到的值为 0b00000010，即十进制的 2 */
    v--;
    v |= v >> (1u << 0u); /* (1u << 0u) 0*/
    v |= v >> (1u << 1u); /* (1u << 1u) 2*/
    v |= v >> (1u << 2u);
    v |= v >> (1u << 3u);
    v |= v >> (1u << 4u);
    v++;
    return v;
}

/**
 * 计算64位无符号整数下一个2次幂
*/
ULong IntUtil::next64Pow2(ULong v)
{
    v--;
    v |= v >> (1u << 0u);
    v |= v >> (1u << 1u);
    v |= v >> (1u << 2u);
    v |= v >> (1u << 3u);
    v |= v >> (1u << 4u);
    v |= v >> (1u << 5u);
    v++;
    return v;
}