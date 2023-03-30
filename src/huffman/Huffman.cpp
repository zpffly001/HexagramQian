#include "Huffman.h"

Huffman::Result Huffman::compress(const std::vector<ByteBuffer> &data)
{
    /* 频数字典ByteBuffer */
    ByteBuffer dictionary;
    /* 压缩后的vector ByteBuffer */
    std::vector<ByteBuffer> compress_text;
    /* 压缩字典（即时生成，非持久化保存的字典） */
    std::array<BitBuffer, size_t(256)> codemap;
    /* 哨兵buffer */
    BitBuffer longbuffer;

    /* 统计频数，即每个Byte出现的次数 */
    std::array<UInt32, size_t(256)> count = doFrequency(data);
    /* 根据频率建立Huffman树 */
    HuffmanTree huffman_tree(count);
    /* 根据Huffman树建立压缩字典 */
    toCodeMap(huffman_tree, codemap, longbuffer);

    compress_private(codemap, data, compress_text, longbuffer);
}

Huffman::HuffmanTree::Node::Node(UInt32 len, Node *le, Node *ri, Byte val) : weight(len), left(le), right(ri),
                                                                             value(val) {
    ;
}

/**
 * HuffmanTree构造函数
*/
Huffman::HuffmanTree::HuffmanTree(std::array<UInt32, size_t(256)> count)
{
    this->creatHuffman(count);
}

/**
 * 根据统计数据，构建一棵Huffman树
*/
void Huffman::HuffmanTree::creatHuffman(std::array<UInt32, size_t(256)> count)
{
    /* 把256个元素放入优先队列中(包括频次为0的元素， 目的是作为哨兵项，处理尾部不满8bit的情况) */
    std::priority_queue<Node *, std::vector<Node *>, Node::cmp> huffman_queue;
    for (size_t i = 0; i < 256; i++)
    {
        huffman_queue.push(new Node(count.at(i), nullptr, nullptr, i));
    }

    /* 每次取最小的两个元素合并为最新的元素，放入队列中，直到优先队列中仅有1个元素 */
    while (huffman_queue.size() > 1)
    {
        Node *p1, *p2;
        /* 获取队首元素 */
        p1 = huffman_queue.top();
        /* 令队首元素出队 */
        huffman_queue.pop();
        p2 = huffman_queue.top();
        huffman_queue.pop();
        huffman_queue.push(new Node(p1->weight + p2->weight, p1, p2));
    }
    
    /* 仅剩的元素作为HuffmanTree的root */
    this->root = huffman_queue.top();
    huffman_queue.pop();
}

/**
 * 根据原始数据统计频数
*/
std::array<UInt32, size_t(256)> Huffman::doFrequency(const std::vector<ByteBuffer> &data)
{
    /* Huffman频数 */
    std::array<UInt32, size_t(256)> count;
    /* 初始化清空 */
    count.fill(0);

    /* 遍历每一个Bytebuffer的每一个Byte，统计频数 */
    for (std::vector<ByteBuffer>::const_iterator iter = data.begin(); iter != data.end(); iter++)
    {
        /* 内层循环遍历每一个ByteBuffer */
        for (size_t i = 0; i < iter.size(); i++)
        {
            size_t temp_count;
            /* 将Byte与0~255的下标进行对应 */
            temp_count = size_t((unsigned char) (*iter)[i]);
            /* 例如当前Byte值为50，就会放到下标为50的索引下，且索引对应的值+1 */
            count.at(temp_count) = count.at(temp_count) + 1;
        }
    }
    return count;
}

void Huffman::toCodeMap(HuffmanTree huffman_tree, std::array<BitBuffer, size_t(256)> &codemap, BitBuffer &longbuffer)
{
    BitBuffer nowmap;
    creatCodeMap(huffman_tree.root, codemap, nowmap);
    for (size_t i = 0; i < 256; i++)
    {
        if (codemap.at(i).bits.size() >= 8)
        {
            /* 寻找大于等于8Bit的huffman编码，作为哨兵项 */
            longbuffer = codemap.at(i);
            break;
        }
    }
}

/**
 * 递归函数，通过遍历huffman树的所有节点形成压缩字典
*/
void Huffman::creatCodeMap(HuffmanTree::Node *node, std::array<BitBuffer, size_t(256)> &arr, BitBuffer &nowmap)
{
    size_t index;
    if (node == nullptr)
    {
        return;
    }
    if (node->isLeaf())
    {
        index = size_t((unsigned char) (node->value));
        /* 找到叶节点，则将该叶节点对应的Byte的编码表写入 */
        arr.at(index) = nowmap;
        return;
    }
    nowmap.addBit(0);
    creatCodeMap(node->left, arr, nowmap);
    /* 回溯 */
    nowmap.delBit();
    
    nowmap.addBit(1);
    creatCodeMap(node->right, arr, nowmap);
    /* 回溯 */
    nowmap.delBit();
}