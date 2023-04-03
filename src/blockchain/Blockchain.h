#ifndef HEXAGRAMQIAN_BLOCKCHAIN_H
#define HEXAGRAMQIAN_BLOCKCHAIN_H
#include <vector>
#include <string>
#include <functional>
#include "util/Types.h"
#include "blockchain/ChainBlock.h"
#include "util/StringUtil.h"
#include "huffman/Huffman.h"
#include "hash/Hash.h"

#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif


/**
 * 区块链管理类
*/
class Blockchain
{
    enum EditOperation
    {
        Copy,
        Add,
        Remove,
        Replace
    }

    struct Difference
    {
        std::string operation;
        ByteBuffer originContent;
        ByteBuffer nowContent;

        Difference(const std::string &operation, const ByteBuffer &originContent, const ByteBuffer &correctContent)
        {
            this->operation = operation;
            this->originContent = originContent;
            this->nowContent = correctContent;
        }

        Difference(const std::string &operation, const ByteBuffer &originContent)
        {
            this->operation = operation;
            this->originContent = this->nowContent = originContent;
        }

        ~Difference() = default;
    };

    /**
     * 保存所有区块链的引用
    */
    static std::vector<std::reference_wrapper<ChainBlock>> GLOBAL_CHAIN;

public:
    /**
     * 创建区块至区块链
     * @return 该区块的ID
    */
   static UInt create(const std::string &data);

   /**
    * 通过区块ID获得一个区块
   */
   static const ChainBlock &get(UInt blockId);

    /**
     * 当前区块链大小
     */
    static size_t size();

    /**
     * 获得所有区块的引用
    */
    static std::vector<std::reference_wrapper<const ChainBlock>> getGlobalChain();

    /**
     * 检测区块头中包含的前一区块的哈希值和真实的前一区块的哈希值是否相同
    */
    static bool check();

    /**
     * 判断给定新闻是否篡改过
     * @return 获取篡改新闻所在自然段
     * copy abc abc
     * replace a b
     * add c c  原文变为篡改后的文章需要在第二段和第三段之间添加一个c
     * remove b b  原文变为篡改后的文章需要删除第四段的c
    */
    static std::vector<Difference> validateNews(const std::string &data, UInt blockId);

#ifdef UNIT_TEST

    /**
     * 清空区块链
     *
     * 仅在单元测试中可见。
     * @return
     */
    static void clear();

    FRIEND_TEST(TestBlockchain, testCreate);

    FRIEND_TEST(TestBlockchain, testCheck);
#endif
};

#endif