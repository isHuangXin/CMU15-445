## cmu-db的安装

Database system project based on CMU 15-445/645 (FALL 2020) 

参考：https://github.com/cmu-db/bustub/blob/master/README.md

课程链接：https://15445.courses.cs.cmu.edu/fall2020/


---

### 克隆存储库

github上新建一个仓库，名为`database-cmu15445-fall2020`。

```bash
$ cd /Users/huangxin/code/cpp
$ git clone --bare https://github.com/cmu-db/bustub.git bustub-public
$ cd bustub-public

$ git push git@github.com:student/bustub-private.git master #同步到自己的远程仓库

$ cd ..
$ rm -rf bustub-public

$ git https://github.com/isHuangXin/CMU15-445.git # 从自己的远程仓库拉取到本地

$ git reset --hard 444765a
HEAD 现在位于 444765a Add PR template. (#156)

# git remote add public git@github.com:cmu-db/bustub.git
# git pull public master

# Bugfix for M1
# Bugfix: -mcpu=apple-m1 && sprintf

$ git add .
$ git commit -m 'Bugfix: -mcpu=apple-m1 && sprintf'
$ git push -f origin master # 将更改好的初始化仓库推送到自己的远程仓库

# Just have a start
# 可以开始编码完成4个Project了

# Project 1: C++ Primer
# Project 2: Buffer Pool Manager
# Project 3: B+Tree Index
# Project 4: Query Execution
# Project 4: Concurrency Control
```

### build

经测试，支持macOS M1，Apple Silicon编译。

```bash
# First install the packages that BusTub requires:
$ sudo ./build_support/packages.sh

# To build the system from the commandline, execute the following commands:
$ mkdir build
$ cd build
$ cmake ..
$ make

# To speed up the build process, you can use multiple threads by passing the -j flag to make. For example, the following command will build the system using four threads:
$ make -j 4


# TESTING
# You can compile and run each test individually from the command-line:
$ mkdir build
$ cd build
$ make starter_test
$ ./test/starter_test
```
