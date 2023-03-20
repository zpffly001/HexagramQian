# 增加git子模块
git submodule add -f  https://github.com/RWTH-HPC/CMake-codecov.git externals/CMake-codecov
git submodule add -f https://github.com/google/googletest.git externals/googletest
git submodule add -f https://github.com/Corvusoft/restbed.git externals/restbed
git submodule add -f https://github.com/google/glog.git externals/glog

# 删除git子模块
rm -rf externals/CMake-codecov
git rm --cached externals/CMake-codecov

# 新仓库拉取子模块
git submodule update --init --recursive

# glog一开始没有logging.h，只有logging.h.in，那么你需要编译，编译后会生成对应的logging.h头文件
# https://www.bbsmax.com/A/MAzAm9n8z9/
mkdir build && cd build
cmake.. && make

# https://blog.csdn.net/weixin_39445116/article/details/116767026
mkdir build && cd build
cmake.. && make