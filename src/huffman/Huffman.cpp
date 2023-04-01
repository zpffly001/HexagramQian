#include "Huffman.h"

/**
 * 对若干数据块进行压缩
 * 没有字典，需统计频数
*/
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

    /* 进行压缩 */
    compress_private(codemap, data, compress_text, longbuffer);
    /* 频数转换为ByteBuffer作为频数字典 */
    dictionary = toDictionaryByte(count);
    return Huffman::Result({dictionary, compress_text});
}

/**
 * 对若干数据块进行压缩
 * 有字典，无需统计频数
*/
std::vector<ByteBuffer> Huffman::compress(const ByteBuffer & dict, const std::vector<ByteBuffer>& data)
{
    std::vector<ByteBuffer> compress_text;
    /* 压缩字典（即时生成，非持久化保存的字典）*/
	std::array<BitBuffer, size_t(256)> codemap;
    /* 哨兵buffer */
	BitBuffer longbuffer;
    /* 把字典转换为频数 */
    std::array<UInt32, size_t(256)> count = fromDictionaryByte(dict);
    /* 根据频数建立Huffman树 */
    HuffmanTree huffman_tree(count);
    /* 根据Huffman树建立压缩字典 */
	toCodeMap(huffman_tree, codemap, longbuffer);
    /* 进行压缩 */
	compress_private(codemap, data, compress_text, longbuffer);
	return compress_text;
}

/**
 * 对一个数据块进行解压
*/
ByteBuffer Huffman::decompress(const ByteBuffer &dict, const ByteBuffer &data)
{
    ByteBuffer decompress_text;
    /* 把字典转换为频数 */
    std::array<UInt32, size_t(256)> count = fromDictionaryByte(dict);
    /* 根据频数建立Huffman树 */
    HuffmanTree huffman_tree(count);
    /* 根据huffman树进行解压 */
    decompress_private(huffman_tree, data, decompress_text);
    return decompress_text;
}

Huffman::HuffmanTree::Node::Node(UInt32 len, Node *le, Node *ri, Byte val) : weight(len), left(le), right(ri),
                                                                             value(val) {
    ;
}

bool Huffman::HuffmanTree::Node::cmp::operator()(Node *&a, Node *&b) const
{
    return a->weight > b->weight;
}

Huffman::HuffmanTree::Node::~Node()
{
    delete this->left;
    delete this->right;
}

bool Huffman::HuffmanTree::Node::isLeaf()
{
    if (this == nullptr) {
        return false;
    }
    if (this->left == nullptr && this->right == nullptr) {
        return true;
    }
    return false;
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

/**
 * 频数count转换为ByteBuffer作为频数字典
 * 把count的每一项转换为32位（4 byte）存放在ByteBuffer中，调用ByteBuffer类中的write函数即可
*/
ByteBuffer Huffman::toDictionaryByte(const std::array<UInt32, size_t(256)> &count)
{
    ByteBuffer result;
    for (size_t i = 0; i < 256; i++)
    {
        result.write(count.at(i));
    }
    return result;
}

/**
 * 把字典转换为频数
*/
std::array<UInt32, size_t(256)> Huffman::fromDictionaryByte(const ByteBuffer &dictionary)
{
    std::array<UInt32, size_t(256)> count;
    for (size_t i = 0; i < 256; i++)
    {
        /* 把这四个Byte转换为UInt32并赋值到count中 */
        count.at(i) = fromByteBuffer(dictionary, 4 * i);
    }
    return count;
}

/**
 * 四个Byte转换为UInt32并赋值到array中
*/
UInt32 Huffman::fromByteBuffer(const ByteBuffer &UInt32_buffer, size_t start)
{
    unsigned int ret = 0;
    for (int i = 0; i < 4; i++)
    {
        ret |= static_cast<unsigned int>(UInt32_buffer[3 - i + start] << 8 * i);
    }
    return ret;
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

void Huffman::toCodeMap(HuffmanTree huffman_tree, std::array<BitBuffer, size_t(256)> &codemap) {
    BitBuffer nowmap;
    creatCodeMap(huffman_tree.root, codemap, nowmap);
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

/**
 * 
*/
void Huffman::compress_private(const std::array<BitBuffer, size_t(256)> &codemap,
                                const std::vector<ByteBuffer> &ori_text,
                                std::vector<ByteBuffer> &compress_text,
                                BitBuffer &longbuffer)
{
    /* 遍历每一个ByteBuffer的每一个Byte */
    for (std::vector<ByteBuffer>::const_iterator iter = ori_text.begin(); iter != ori_text.end(); iter++)
    {
        ByteBuffer now_bytbuffer;
        BitBuffer temp_bit;
        for (size_t i = 0; i < iter->size(); i++)
        {
            size_t temp_count;
            BitBuffer temp_bit;
            /* 将Byte与0~255的下标进行对应 */
            temp_count = size_t((unsigned char) (*iter)[i]);
            /* 获取该byte对应的huffman编码 */
            temp_bit = codemap.at(temp_count);
            /* 将该huffman编码push进bitbuffer中 */
            now_bytbuffer.pushBitBuffer(temp_bit);
        }
        now_bytbuffer = now_bytbuffer.toByteBuffer(longbuffer);
        now_bytbuffer.clear();
        compress_text.push_back(now_bytbuffer);
    }
}

ByteBuffer Huffman::BitBuffer::toByteBuffer(Huffman::BitBuffer &longbuffer) {
    ByteBuffer buffer;
    std::array<unsigned char, 8> byte;
    int index = 0;
    unsigned char byte_num = 0;
    for (std::vector<Bit>::const_iterator iter = this->bits.begin(); iter != this->bits.end(); iter++) {
        if (index == 8) {
            byte_num = 128 * byte.at(7) + 64 * byte.at(6) + 32 * byte.at(5) + 16 * byte.at(4) + 8 * byte.at(3) +
                       4 * byte.at(2) + 2 * byte.at(1) + 1 * byte.at(0);
            buffer.push_back(byte_num);//每8个插入一次
            index = 0;
        }
        //原：byte[index++] = *iter;
        if (iter->b == 0) {
            byte.at(7 - index) = 0;
        } else {
            byte.at(7 - index) = 1;
        }
        index++;
    }
    if (index != 8) {
        //原：for (int i = index; i < 8; i++) {
        for (int i = 0; i < 8 - index; i++) {
            byte.at(7 - index - i) = longbuffer.bits.at(i);//不满8个，剩余部分置为longbuffer的一部分
        }
        byte_num = 128 * byte.at(7) + 64 * byte.at(6) + 32 * byte.at(5) + 16 * byte.at(4) + 8 * byte.at(3) +
                   4 * byte.at(2) + 2 * byte.at(1) + 1 * byte.at(0);
        buffer.push_back(byte_num);//每8个插入一次
    }
	else
	{
		byte_num = 128 * byte.at(7) + 64 * byte.at(6) + 32 * byte.at(5) + 16 * byte.at(4) + 8 * byte.at(3) +
			4 * byte.at(2) + 2 * byte.at(1) + 1 * byte.at(0);
		buffer.push_back(byte_num);//每8个插入一次
	}
    return buffer;
}

/**
 * 根据huffman树进行解压
*/
void Huffman::decompress_private(HuffmanTree huffman_tree, const ByteBuffer &ori_text, ByteBuffer &decompress_text)
{
    /* 把ByteBuffer转换为BitBuffer，便于遍历 */
    BitBuffer bitbuffer = BitBuffer(ori_text);
    /* 用于搜索huffman树的指针 */
    HuffmanTree::Node *p = huffman_tree.root;
    Bit left_bit(0);
    Bit right_bit(1);
    for (std::vector<Bit>::iterator iter = bitbuffer.bits.begin(); iter != bitbuffer.bits.end(); iter++)
    {
        if (*iter == left_bit)
        {
            p = p->left;
        }else 
        {
            p = p->right;
        }
        if (p->isLeaf())
        {
            decompress_text.push_back(p->value);
            p = huffman_tree.root;
        }
    }
}

void Huffman::BitBuffer::pushBitBuffer(BitBuffer buffer)
{
    for (std::vector<Bit>::const_iterator iter = buffer.bits.begin(); iter != buffer.bits.end(); iter++)
    {
        this->addBit(*iter);
    }
    
}

Huffman::BitBuffer::BitBuffer() {
}

Huffman::BitBuffer::BitBuffer(ByteBuffer buffer)
{
    for (size_t i = 0; i < buffer.size(); i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            addBit(buffer[i][7 - j]);
        }
    }
}

void Huffman::BitBuffer::addBit(Bit addbit)
{
    this->bits.push_back(addBit);
}

void Huffman::BitBuffer::delBit() {
    this->bits.pop_back();
}

void Huffman::BitBuffer::clear()
{
    std::vector<Bit>().swap(this->bits);
}