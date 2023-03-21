#ifndef HEXAGRAMQIAN_BYTE_H
#define HEXAGRAMQIAN_BYTE_H

#include <iostream>

/**
 * 单比特
 * FIXME C++中Class类和sturct的区别
*/
struct Bit
{
    /* FIXME :号在这里有什么作用*/
    /* 位于操作https://blog.csdn.net/weixin_47586883/article/details/120949023 */
    unsigned char b : 1;

    /* 类似于b = val */
    Bit(unsigned char val) : b(val){}

    /* FIXME */
    operator unsigned char();

    /* FIXME 友元的作用*/
    friend std::ostream & operator<<(std::ostream &, const Bit&);
};

/**
 * 小端字节
*/
union LittleEndianByte
{
    /**
     * 值
    */
    unsigned char val;

    /**
        * 包含比特
    */
    struct BitPattern
    {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;

        /* FIXME const修饰的为常函数 常函数的函数体，只能获取(读数据)，不可以写数据，不可修饰类中属性。常函数一般用在获取数据时，保证数据安全*/
        Bit operator[](int idx) const;
    }bits;
  
    LittleEndianByte() : val(0) {};

    LittleEndianByte(unsigned char val) : val(val) {}

    Bit operator[](int idx) const;

    /**
     * FIXME
     * C++ 类型转换函数
     * https://blog.csdn.net/liyazhen2011/article/details/86349255
    */
    operator unsigned char() const;

    /**
     * 将字节转化为十六进制字符串
    */
    std::string toHexString() const;

    friend std::ostream & operator<<(std::ostream &, const LittleEndianByte&);
};

/**
 * 单字节
 * 
 * 比特序为小端，即低比特在低地址，高比特在高地址
 * 即大部分内存的存储格式
*/
typedef LittleEndianByte Byte;

#endif