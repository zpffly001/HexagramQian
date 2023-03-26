#ifndef HEXAGRAMQIAN_CHAINBLOCK_H
#define HEXAGRAMQIAN_CHAINBLOCK_H

#include "util/Types.h"
#include "util/ByteBuffer.h"
#include "BloomFilter.h"

#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif // DEBUG


/**
 * 64位时间戳
*/
typedef ULong Timestamp;

/**
 * 区块类
 * 区块头、区块body、区块下标
*/
class ChainBlock : public IByteBufferWriter
{

public:

    /**
     * 数据块下标
     * Genesis Block创世区块(0块)存放Huffman压缩的字典
    */
    typedef ULong DataBlockIndex;

    /**
     * 哈希树下标
     * 平展哈希树数组的下标
    */
    typedef ULong HashTreeIndex;

    /**
     * 区块Header结构体
     * 
     * 二进制格式(大端)
     * 4字节 区块ID
     * 4字节 前一区块的哈希值
     * 4字节 哈希树树根
     * 8字节 区块创建时的时间戳
     * 8字节 数据块数量 block_size
    */
    struct Header
    {
        /**
         * 区块ID
        */
        UInt blockId;

        /**
         * 前一块区块的哈希值
         * 创世区块填充0x00
        */
        UInt32 prevBlockHash;

        /**
         * 哈希树树根
        */
        UInt32 hashRoot;

        /**
         * 建块时间戳
        */
        Timestamp timestamp;

        /**
         * 区块数量
        */
        DataBlockIndex size;
    };
    
    /**
     * 区块体结构体
     * 二进制格式（大端）
     * 8字节                哈希树长度(不包含树根)hash_size
     * 4 * hash_size字节    哈希树(不包含树根)
     * 区块数据：
     *      4字节           数据块内容长度 length
     *      length字节      数据块内容
    */
    struct Body
    {
        /**
         * 平展后的哈希树叶子节点（数据块）开始的偏移
         * 注意哈希树有约定：所有叶子节点均在同一层
        */
        HashTreeIndex blockHashOffset;

        /**
         * 平展后的哈希树
         * 存放了平展后的哈希树，并且包括哈希树根（但是计算快哈希值和存储时不会包含哈希树根）
         * 根的左右子节点分别为下标位置位0，1的哈希值。某节点i的左右子节点可以通过2*i和2*i+1
         * 计算，父节点可以使用i/2计算。某数据块的哈希下标可以通过ChainBlock::getHashTreeIndexById方法计算
        */
        std::vector<UInt32> hashTree;

        /**
         * 数据块
        */
        std::vector<ByteBuffer> dataBlocks;
    };
    

private:
    
    /**
     * 区块头部
    */
    Header blockHeader;

    /**
     * 区块体
    */
    Body blockBody;

    /**
     * 关键字筛选用布隆过滤器
    */
    BloomFilter *keywordFilter = nullptr;

public:
    /**
     * 建立区块
    */
    ChainBlock(const std::string &data, UInt32 prevHash, UInt blockId);

    /**
     * 计算区块哈希
    */
    UInt32 getBlockHash() const;

    /**
     * 获得区块头
    */
    const Header &getHeader() const;

    /**
     * 获得平展哈希树
    */
    const std::vector<UInt32> &getHashTree() const;

    /**
     * 获得区块数量（包括字典）
    */
    DataBlockIndex size() const;

    /**
     * 获得所有数据块（包括字典）
    */
    const std::vector<ByteBuffer> &getAllDataBlock() const;

    /**
     * 获得某个区块数据
    */
    ByteBuffer &operator[](DataBlockIndex idx);

    const ByteBuffer &operator[](DataBlockIndex idx) const;

    /**
     * 获得某个数据快的解压数据
     * @param idx 数据块id，为0则返回原数据块内容
    */
    ByteBuffer getDecomposeDataBlock(DataBlockIndex idx) const;

    /**
     * 获得所有解压后的数据块
    */
    std::vector<ByteBuffer> getAllDecomposedDataBlock() const;

    /**
     * 获得某个数据快的哈希值
    */
    UInt32 getDataBlockHashById(DataBlockIndex idx) const;

    /**
     * 获得某数据块对应的哈希树下标
    */
    HashTreeIndex getHashTreeIndexById(DataBlockIndex idx) const;

    /**
     * 判断哈希树下标位置的节点是否是填充空白
    */
    bool isHashTreePadding(HashTreeIndex idx) const;

    /**
     * 写区块数据为字节缓冲区
     * 格式为：区块头 + 区块体。区块头、区块体格式参考对应结构体的注释。
     * FIXME const override 关键字有什么作用
    */
    ByteBuffer &writeBuffer(ByteBuffer &buffer) const override;

    /**
     * 获取前一区块的哈希值
    */
    UInt32 getPrevBlockHash() const;

    /**
     * 在O(m)时间内判断是否包含m个关键字
    */
    bool hashKeyword(const std::vector<std::string> &keywords);

    /**
     * FIXME 区块析构
    */
    virtual ~ChainBlock();

#ifdef UNIT_TEST

    FRIEND_TEST(TestChainBlock, testConstruct);

    FRIEND_TEST(TestChainBlock, testHashTree1);

    FRIEND_TEST(TestChainBlock, testHashTree2);

    FRIEND_TEST(TestChainBlock, testHashTreeIndexCalc);

#endif

private:
    
    /**
     * 获得字典数据块
    */
    const ByteBuffer &getDicBlock() const;

    /**
     * 从区块体构建哈希树
    */
    void buildHashTree();

    /**
     * 从区块体构建布隆过滤器
    */
    void buildBloomFilter(const std::vector<ByteBuffer> &data, size_t length);

    /**
     * 计算blockHashOffset
    */
    static HashTreeIndex calcBlockHashOffset(DataBlockIndex size);

    /**
     * 判断哈希树下标位置的节点是否是填充空白
    */
    static bool isHashTreePadding(DataBlockIndex size, HashTreeIndex idx);

    /**
     * FIXME 这个友元有什么作用
    */
    friend class Serializer;
};

#define HTREE_LF(x) (2*(x)+1)
#define HTREE_RT(x) (2*(x)+2)
#define HTREE_PRT(x) (((x)-1)/2)

#endif