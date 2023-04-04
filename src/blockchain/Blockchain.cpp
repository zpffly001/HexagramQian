#include <cassert>
#include <algorithm>
#include "Blockchain.h"

std::vector<std::reference_wrapper<ChainBlock>> Blockchain::GLOBAL_CHAIN;

UInt Blockchain::create(const std::string &data)
{
    ChainBlock *newBlock = nullptr;
    UInt32 prevHash;
    UInt blockId = size();

    /* 计算前一块哈希 */
    if (size() > 0)
    {
        prevHash = get(size() - 1).getBlockHash();
    }else{
        /* 第一块则为0 */
        prevHash = 0u;
    }
    /* 创建块 */
    newBlock = new ChainBlock(data, prevHash, blockId);
    /* 加入链 */
    /* std::ref(*newBlock)将一个对象转换成一个引用包装器（reference wrapper），它的主要作用是将一个对象转换成一个引用类型，并提供了访问该对象的引用的方法。需要注意的是，std::reference_wrapper并不是一个裸引用，它本身是一个对象，可以被复制和赋值。 */
    GLOBAL_CHAIN.push_back(std::ref(*newBlock));
}

const ChainBlock &Blockchain::get(UInt blockId)
{
    return GLOBAL_CHAIN[blockId];
}

size_t Blockchain::size()
{
    return GLOBAL_CHAIN.size();
}

std::vector<std::reference_wrapper<const ChainBlock>> Blockchain::getGlobalChain()
{
    std::vector<std::reference_wrapper<const ChainBlock>> ret;
    ret.reserve(size());
    for(auto &ref : GLOBAL_CHAIN)
    {
        ret.emplace_back(ref.get());
    }
    return ret;
}


#ifdef UNIT_TEST

void Blockchain::clear() {
    for (auto &ref: GLOBAL_CHAIN) {
        delete &ref.get();
    }
    GLOBAL_CHAIN.clear();
}

#endif