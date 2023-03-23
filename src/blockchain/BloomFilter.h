#ifndef HEXAGRAMQIAN_BLOOMFILTER_H
#define HEXAGRAMQIAN_BLOOMFILTER_H

#include <cstddef>
#include <cstdint>
#include <vector>
#include <util/ByteBuffer.h>
#include <util/Types.h>

/**
 * 布隆过滤器的简单实现(以太坊使用、比特币未使用)
 * 特点：hash未命中一定不存在，hash命中不一定存在(hash碰撞)
*/
class BloomFilter
{
    /**
     * 用于产生哈希的种子
    */
   std::vector<ULong> hashSeeds;

    /**
        * 存放比特位的缓冲区
    */
    ByteBuffer bitBuffer;

    /**
     * 比特数
     */
    size_t size = 0;

    BloomFilter() = default;

public:
    explicit BloomFilter(size_t capcity, float falsePositive = 0.02);

    /**
     * 向布隆过滤器中插入数据
    */
    void insert(const ByteBuffer &data);

    /**
     * 向布隆过滤器中插入数据
    */
    void insert(const std::string &str);

    /**
     * 判断布隆过滤器是否包含某个数据
    */
    bool contain(const ByteBuffer &data);

    /**    
     * 判断布隆过滤器是否包含某个数据
    */
    bool contain(const std::string &str);

    /**
     * 把数据写入 buffer
    */
    void writeToBuffer(ByteBuffer &buffer);

    /**
     * 从buffer中读取
    */
    static BloomFilter *readFromBuffer(const ByteBuffer &buffer);

    ~BloomFilter();

private:
    size_t hash(const ByteBuffer &data, ULong seed);
    
    size_t hash(const void *data, size_t len, ULong seed);
};

#endif