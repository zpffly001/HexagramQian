#ifndef HEXAGRAMQIAN_HUFFMAN_H
#define HEXAGRAMQIAN_HUFFMAN_H

#include <array>
#include <queue>
#include "util/ByteBuffer.h"

class Huffman
{
public:
    /**
     * 压缩结果
    */
    struct Result
    {
        /**
         * 压缩字典
        */
        ByteBuffer dictionary;

        /**
         * 压缩结果数据块
         * 每一块对应于传入的一块
        */ 
        std::vector<ByteBuffer> data;
    };

    /**
     * Huffman压缩的对外接口
    */

    /**
     * 对若干数据块进行压缩
     * 压缩字典应该根据所有传入的数据快建立，然后逐块进行压缩
     * @param data 待压缩数据
     * @return 压缩结果对象
    */
    static Result compress(const std::vector<ByteBuffer> &data);
    static std::vector<ByteBuffer> compress(const ByteBuffer &dict, const std::vector<ByteBuffer> &data);

    /**
     * 对一个数据块进行解压
     * @param dict 压缩过程返回的字典
     * @param data 解压缩数据
     * @return 解压结果
    */
    static ByteBuffer decompress(const ByteBuffer &dict, const ByteBuffer &data);
    
public:
    class HuffmanTree
    {
    public:
        class Node
        {
        public:
            /* 叶子结点的Byte有效，非叶子节点的Byte无意义 */
            Byte value;
            /* 权重 */
            UInt32 weight;
            Node *left;
            Node *right;

            /* std::sort的排序优先级函数 */
            struct cmp
            {
                /* FIXME *&a */
                bool operator()(Node *&a, Node *&b) const;
            };

            /**
             * 构造函数指定权，左子树，右子树，Byte
            */
            Node(UInt32 len = 0, Node *le = nullptr, Node *ri = nullptr, Byte val = 0b11111111u);

            ~Node();

            //判断当前节点是否为叶子节点
            bool isLeaf();
        };

        /* Huffman树的根节点 */
        Node *root;

        HuffmanTree(std::array<UInt32, size_t(256)> count);

        void creatHuffman(std::array<UInt32, size_t(256)> count);
        

        
    };
    

    
    
};

Huffman::Huffman(/* args */)
{
}

Huffman::~Huffman()
{
}


#endif