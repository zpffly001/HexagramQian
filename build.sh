# 增加git子模块
git submodule add -f  https://github.com/RWTH-HPC/CMake-codecov.git externals/CMake-codecov
git submodule add -f https://github.com/google/googletest.git externals/googletest
git submodule add -f https://github.com/Corvusoft/restbed.git externals/restbed
git submodule add -f https://github.com/google/glog.git externals/glog

# 删除子模块
rm -rf externals/CMake-codecov
git rm --cached externals/CMake-codecov

# 拉取子模块
git submodule update --init --recursive

# glog一开始没有logging.h，只有logging.h.in，那么你需要编译，编译后会生成对应的logging.h头文件
# https://www.bbsmax.com/A/MAzAm9n8z9/
mkdir build && cd build
cmake.. && make

# https://blog.csdn.net/weixin_39445116/article/details/116767026
mkdir build && cd build
cmake.. && make




# static void toCodeMap(HuffmanTree huffman_tree, std::array<BitBuffer, size_t(256)> &);
# 引用传递的参数，在函数定义时可以省略参数名称，只保留引用符号&。
# 这是因为引用是一个别名，它指向传递给函数的实参，因此函数内部可以直接使用它，而不需要复制参数的值。
# 这也提高了程序的效率，因为引用传递避免了参数值的复制。
