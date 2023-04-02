#include "StringUtil.h"


/**
 * 将字符串按自然段分割
 * 若按 '\n' 分割出现空字符串，则不包含在结果中
 */
std::vector<ByteBuffer> StringUtil::splitParagraph(const std::string &str)
{
    std::vector<ByteBuffer> output;
    size_t first = 0;
    while (first < str.size())
    {
        /* FIXME 找到下一个\n */
        const auto second = str.find_first_of('\n', first);
        if (first != second)
        {
            /* FIXME 把\n之前的数据截取放到output中 */
            auto cur = str.substr(first, second - first);
            /* emplace_back类似于push_back */
            output.emplace_back(void *) cur.data(), cur.size());
        }
        /* npos是一个常数，用来表示不存在的位置,string::npos代表字符串到头了结束了 */
        if (second == std::string::npos)
        {
            break;
        }
        /* 往下循环遍历 */
        first = second + 1;
    }
    return output;
}

/**
 * 字符串分割
 * Code from: http://quick-bench.com/mhyUI8Swxu3As-RafVUSVfEZd64
 */
std::vector<std::string> StringUtil::split(const std::string &str, const std::string &delims = " ")
{
    std::vector<std::string> output;
    size_t first = 0;
    while (first < str.size())
    {
        /* FIXME 获取下一个指定字符的位置 */
        const auto second = str.find_first_of(delims, first);
        if (first != second)
        {
            /* FIXME 把从first起到second之间的字符串放入output*/
            output.emplace_back(str.substr(first, second - first));
        }
        if (second == std::string::npos)
        {
            break;
        }
        first = second + 1;
    }
    return output;
}

/**
 * 判断字符串是否结尾于
 */
bool endsWith(const std::string &str, const std::string &needle)
{
    return str.rfind(needle) == (str.length() - needle.length());
}