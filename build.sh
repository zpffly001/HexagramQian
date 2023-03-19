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