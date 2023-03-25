#ifndef HEXAGRAMQIAN_STRING_UTIL_H
#define HEXAGRAMQIAN_STRING_UTIL_H

#include <vector>
#include <string>
#include "ByteBuffer.h"


class StringUtil
{
public:
    /**
     * 将字符串按自然段分割
     * 若按'\n'分割出现空字符串，则不包含在结果中
    */
    static std::vector<ByteBuffer> splitParagraph(const std::string &str);

    /**
     * 字符串分割
    */
    static std::vector<std::string> split(const std::string &str, const std::string &delims);

    /**
     * 判断字符串是否结尾于
    */
    static bool endsWith(const std::string &str, const std::string &needle);
};


#endif