#ifndef HEXAGRAMQIAN_INT_H
#define HEXAGRAMQIAN_INT_H

#include <Byte.h>
#include <Types.h>
#include <vector>

/**
 * 无符号32位整数
*/
class UInt32
{

    Byte bytes[4];

    /**
    * unsigned int类型的数据转化为4Byte
    * Byte (&bytes)[4]表示访问一个由 bytes 数组的第四个元素开始的新数组
    * &bytes 将 bytes 数组的地址作为一个指针返回，然后在这个指针上进行偏移量的计算。
    * 因为 bytes 数组的类型是 char，所以这个偏移量是以字节为单位计算的。
    */
    static void uintToBytes(unsigned int, Byte (&bytes)[4]);

    /**
     * 4Byte类型的数据转化为unsigned int
    */
    static unsigned int byteToUint(const Byte (&bytes)[4]);

public:
    UInt32()
    {
        bytes[0] = bytes[1] = bytes[2] = bytes[3] = 0x00;
    }

    UInt32(unsigned int val);

    /**
     * 数据类型转换
    */
    operator unsigned int() const;

    UInt32 &operator=(unsigned int val);

    /**
     * 转换字节vector
     * 默认字节序不变，即大端
    */
    std::vector<Byte> toVector() const;

    ~UInt32();

};


/**
 * 无符号16位整数
*/
class UInt16
{
    Byte bytes[2];

    /**
     * unsigned short类型的数据转化为2Byte
    */
    static void uintToBytes(unsigned short, Byte (&bytes)[2]);

    static unsigned short byteToUint(const Byte (&bytes)[2]);
public:
    UInt16()
    {
        bytes[0] = bytes[1] = 0x00;
    }

    UInt16(unsigned short val);

    operator unsigned short() const;

    UInt16 &operator=(unsigned short val);

    /**
     * 转换为字节vector
     * 默认字节序不变，即大端序
    */
    std::vector<Byte> toVector() const;
};


/**
 * 常用整数函数
*/
class IntUtil
{
public:
    /**
     * 计算32位无符号整数下一个2次幂
    */
    static UInt netx32Pow2(UInt val);

    /**
     * 计算64位无符号整数下一个2次幂
    */
    static ULong next64Pow2(ULong val);
};


#endif