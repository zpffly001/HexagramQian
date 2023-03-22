#include <cassert>
#include <string>
#include <sstream>
#include "ByteBuffer.h"

/**
 * 将数据从data地址到data+size地址之间的值赋值给bytes
*/
ByteBuffer::ByteBuffer(void *data, size_t size)
{
    bytes.assign((UChar *)data, (UChar *)data + size);
}

/**
 * bytes指向已有的一个地址
*/
ByteBuffer::ByteBuffer(const std::vector<Byte> &buffer)
{
    bytes = buffer;
}

/**
 * 将字符串转换为字节缓冲区
*/
ByteBuffer ByteBuffer::str(const std::string &str)
{
    /* data()获取首地址，size为大小 */
    return ByteBuffer((void *)str.data(), str.size());
}

/**
 * 返回当前缓冲区大小
*/
size_t ByteBuffer::size() const
{
    return bytes.size();
}

/**
 * 在缓冲区末尾插入一个字节
*/
ByteBuffer &ByteBuffer::push_back(Byte byte)
{
    return push_back(&byte, 1u);
}

/**
 * 在缓冲区末尾插入一段连续内存
*/
ByteBuffer &ByteBuffer::push_back(void const *data, size_t size)
{
    bytes.insert(bytes.end(), (UChar *)data, (UChar *)data + size);
    return *this;
}

/**
 * 在缓冲区头插入一个字节
*/
ByteBuffer &ByteBuffer::push_front(Byte byte)
{
    return push_front(&byte, 1u);
}

/**
 * 在缓冲区头部插入一段连续内存
*/
ByteBuffer &ByteBuffer::push_front(void const *data, size_t size)
{
    bytes.insert(bytes.begin(), (UChar *)data, (UChar *)data + size);
}

/**
 * 拼接缓冲区 运算符重载(具体行为就是：新建一个ByteBuffer，把之前的bytes数据和传入的即做加法的buffer这两部分数据合并到新的ByteBuffer中)
 * @param buffer 下标，为负即从尾部取
 * @return 返回新的缓冲区
*/
ByteBuffer ByteBuffer::operator+(const ByteBuffer &buffer) const
{
    /* 新建一个ByteBuffer对象，这里和java语法不同， ByteBuffer newBuffer其实调用了默认构造函数，不需要在变量名后面+() */
    ByteBuffer newBuffer;
    newBuffer.bytes = bytes;
    newBuffer.bytes.insert(newBuffer.bytes.end(), buffer.bytes.begin(), buffer.bytes.end());
    return newBuffer;
}

/**
 * 运算符重载
*/
std::ostream &operator<<(std::ostream &out, const ByteBuffer &buffer)
{
    for (ULong i = 0; i < buffer.bytes.size(); i++)
    {
        if(i > 0)
            out << " ";
        out << buffer.bytes[i].toHexString().c_str();
    }
    return out;
}

/**
 * 将hex16进制字符串转换为字节缓冲区
*/
ByteBuffer ByteBuffer::hex(const std::string &str)
{
    std::stringstream sstream;
    UInt cur;
    /* 调用默认构造函数 新建一个ByteBuffer对象 */
    ByteBuffer result;
    sstream << std::hex << str;
    while (sstream >> cur)
    {
        result.push_back(cur);
    }
    return result;
}

/**
 * 查看某一索引下的Byte数据
*/
Byte &ByteBuffer::operator[](Index idx)
{
    size_t size = this->size();
    /* 索引小于0，从后往前*/
    if (idx < 0)
    {
        idx += size;
    }
    assert(0 < idx && (unsigned long) idx < size);
    return bytes[idx];
}

/**
 * 查看某一索引下的Byte数据
*/
Byte ByteBuffer::operator[](ByteBuffer::Index ind) const {
    size_t size = this->size();
    /* 索引小于0，从后往前*/
    if (ind < 0)
        ind += size;
    assert(0 <= ind && (unsigned long) ind < size);
    return bytes[ind];
}

/**
 * 将缓冲区数据转为vector
*/
std::vector<Byte> ByteBuffer::toVector() const 
{
    return bytes;
}

/**
* 返回缓冲区数据首指针
* 使用时应该结合ByteBuffer::size返回的尺寸
*/
Byte *ByteBuffer::data()
{
    return bytes.data();
}

/**
* 返回缓冲区数据首指针
* 使用时应该结合ByteBuffer::size返回的尺寸
*/
Byte const *ByteBuffer::data() const 
{
    return bytes.data();
}

/**
 * 创建缓冲区的分片
 * @param start 开始下标，从开头取至结尾，为负即从尾部取
 * @return 返回新的缓冲区，大小为size-start
*/
ByteBuffer ByteBuffer::slice(ByteBuffer::Index start)
{
    return this->slice(start, this->size());
}

/**
 * 创建缓冲区的分片
 * @param start 开始下标，从开头取至结尾，为负即从尾部取
 * @return 返回新的缓冲区，大小为size-start
*/
ByteBuffer ByteBuffer::slice(ByteBuffer::Index start, ByteBuffer::Index end)
{
    size_t size = this->size();
    /* 索引小于0，从后往前*/
    if (start < 0)
        start += size;
    assert(0 <= start && (unsigned long) start < size);
    if (end < 0)
        end += size;
    assert(0 < end && (unsigned long) end <= size);
    /* 创建新缓冲区 */
    return ByteBuffer(std::vector<Byte>(bytes.begin() + start, bytes.begin() + end))
}

/**
 * 赋值运算符重载
*/
ByteBuffer &ByteBuffer::operator=(const std::vector<Byte> &buffer)
{
    bytes = buffer;
    return *this;
}

/**
 * 相等 == 运算符重载
*/
bool operator==(const ByteBuffer &bufferA, const ByteBuffer &bufferB) {
    return bufferA.bytes == bufferB.bytes;
}

/**
 * 在缓冲区末尾以大端序插入一个8位无符号整数
*/
ByteBuffer &ByteBuffer::write(UInt byte)
{
    /* 重新解释类型，既不检查指向的内容，也不检查指针类型本身；但要求转换前后的类型所占用内存大小一致 */
    auto data = reinterpret_cast<Byte *>(&byte);
    return this->push_back(data[1].push_back(data[0]))
}

/**
 * 在缓冲区末尾以大端序插入一个16位无符号整数
*/
ByteBuffer &ByteBuffer::write(UInt byte) 
{
    auto data = reinterpret_cast<Byte *>(&byte);
    return this->push_back(data[3].push_back(data[2]))
            .push_back(data[1]).push_back(data[0]);
}

/**
 * 在缓冲区末尾以大端序插入一个64位无符号整数
*/
ByteBuffer &ByteBuffer::write(ULong byte) {
    auto data = reinterpret_cast<Byte *>(&byte);
    return this->push_back(data[7]).push_back(data[6])
            .push_back(data[5]).push_back(data[4])
            .push_back(data[3]).push_back(data[2])
            .push_back(data[1]).push_back(data[0]);
}

/**
 * 预分配缓冲区空间
*/
void ByteBuffer::allocate(size_t n)
{
    bytes.assign(n, 0);
}

/**
 * 清空缓冲区
*/
ByteBuffer &ByteBuffer::clear()
{
    bytes.clear();
    return *this;
}