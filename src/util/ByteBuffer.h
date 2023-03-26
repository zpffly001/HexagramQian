#ifndef HEXAGRAMQIAN_BYTEBUFFER_H
#define HEXAGRAMQIAN_BYTEBUFFER_H

#include <util/Types.h>
#include <vector>
#include <string>

/**
 * 字节缓冲区
*/
class ByteBuffer
{
    std::vector<Byte> bytes;

public:
    typedef long Index;

    /* FIXME? C++11允许我们使用=default来要求编译器生成一个默认构造函数 */
    ByteBuffer() = default;

    ByteBuffer(void *data, size_t size);

    /**
     * 将字节vector转换为字节缓冲区
    */
    /* FIXME? onst std::vector<Byte> &buffer是地址不能变还是地址指向的值不能变？ */
    ByteBuffer(const std::vector<Byte> &buffer);

    /**
    * 将字符串转换为字节缓冲区
    */
    static ByteBuffer str(const std::string &str);

    /**
     * 将hex16进制字符串转换为字节缓冲区
     */
    static ByteBuffer hex(const std::string &str);

    /* FIXME? */
    ByteBuffer &operator=(const std::vector<Byte> &buffer);

    /**
     * 返回当前缓冲区大小
    */
    /* FIXME? */
    size_t size() const;

    /**
     * 在缓冲区末尾插入一个字节
    */
    ByteBuffer &push_back(Byte byte);

    /**
     * 在缓冲区末尾插入一段连续内存
    */
    /* FIXME?为什么要在方法名前加&;  void const*和const void*的区别 */
    ByteBuffer &push_back(void const *data, size_t size);

    /**
     * 在缓冲区头插入一个字节
    */
    ByteBuffer &push_front(Byte byte);

    /**
     * 在缓冲区头插入一段连续内存
    */
    ByteBuffer &push_front(void const *data, size_t size);

    /**
     * 拼接缓冲区
     * @param buffer 下标，为负即从尾部取
     * @return 返回新的缓冲区
    */
    /* FIXME? 方法后的const的作用；operator+是重载运算符吗？*/
    ByteBuffer operator+(const ByteBuffer &buffer) const;

    /**
     * 取缓冲区一个字节
    */
    /* FIXME? */
    Byte &operator[](Index idx);
    Byte operator[](Index idx) const;

    /**
     * 将缓冲区数据转为vector
     * @return 复制数据为新vector
    */
    std::vector<Byte> toVector() const;

    /**
    * 返回缓冲区数据首指针
    * 使用时应该结合ByteBuffer::size返回的尺寸
    */
    Byte *data();

    /**
     * 返回缓冲区数据首指针
     * 使用时应该结合ByteBuffer::size返回的尺寸
    */
    /* FIXME? */
    Byte const *data() const;

    /**
     * 创建缓冲区的分片
     * @param start 开始下标，从开头取至结尾，为负即从尾部取
     * @return 返回新的缓冲区，大小为size-start
    */
    ByteBuffer slice(Index start);
    
    /**
     * 创建缓冲区的分片
     *
     * @param start 开始下标，为负即从尾部取
     * @param end 结束下标，即结束元素下标+1，为负即从尾部取
     * @return 返回新的缓冲区，大小为end-start
     */
    ByteBuffer slice(Index start, Index end);

    /**
     * 在缓冲区末尾以大端序插入一个16位无符号整数
    */
    ByteBuffer &write(UShort uShort);

    /**
     * 在缓冲区末尾以大端序插入一个64位无符号整数
    */
    ByteBuffer &write(ULong uLong);

    /**
     * 预分配缓冲区空间
    */
    void allocate(size_t size);

    /**
     * 清空缓冲区
     * @return
    */
    ByteBuffer &clear();

    /* FIXME? */
    friend bool operator==(const ByteBuffer &, const ByteBuffer &);

    /* FIXME? */
    friend std::ostream &operator<<(std::ostream &, const ByteBuffer &);

    ~ByteBuffer() = default;
};

/**
 * 写字节缓冲区接口
*/
class IByteBufferWriter
{
public:

    /**
     * 向字节缓冲区中写入对象数据
    */
    /* FIXME? vitual表明是接口？ const=0的作用？*/
    virtual ByteBuffer &writeBuffer(ByteBuffer &buffer) const = 0;
}

#endif