# Linux Note

## 1. Linux 基础命令

### 1.1 ln 软硬链接

**硬链接**

硬链接说白了是一个指针，指向文件索引节点，系统并不为它重新分配inode。可以用: ln 命令来建立硬链接。

尽管硬链接节省空间，也是Linux系统整合文件系统的传统方式，但是存在一下不足之处：

- （1）不可以在不同文件系统的文件间建立链接

- （2）只有超级用户才可以为目录创建硬链接。

**软链接（符号链接）**

软链接克服了硬链接的不足，没有任何文件系统的限制，任何用户可以创建指向目录的符号链接。因而现在更为广泛使用，它具有更大的灵活性，甚至可以跨越不同机器、不同网络对文件进行链接。
建立软链接，只要在 ln 后面加上选项  –s

```shell
$ ln -s abc cde 	# 建立 abc 的软连接
$ ln abc cde 		# 建立 abc 的硬连接，
```

**删除链接**

```shell
$ rm -rf symbolic_name
$ unlink symbolic_name
```

### 1.2 find grep xargs

```shell
# 列出当前目录及子目录下所有文件和文件夹
$ find .

# 在/home目录下查找以.txt结尾的文件名 但忽略大小写
$ find /home -iname "*.txt"

# 当前目录及子目录下查找所有以 .txt 和 .pdf 结尾的文件
$ find . \( -name "*.txt" -o -name "*.pdf" \)
$ find . -name "*.txt" -o -name "*.pdf"

# 匹配文件路径或者文件
$ find /usr/ -path "*local*"

# 基于正则表达式匹配文件路径
$ find . -regex ".*\(\.txt\|\.pdf\)$"

# 同上，但忽略大小写
$ find . -iregex ".*\(\.txt\|\.pdf\)$"

# 找出/home下 不是 以 .txt 结尾的文件
$ find /home ! -name "*.txt"

# 根据文件类型进行搜索
# 类型参数列表：
#  f 普通文件
#  l 符号连接
#  d 目录
#  c 字符设备
#  b 块设备
#  s 套接字
#  p Fifo
$ find . -type 类型参数

# 基于目录深度搜索 向下最大深度限制为3
$ find . -maxdepth 3 -type f

# 搜索出深度距离当前目录至少2个子目录的所有文件
$ find . -mindepth 2 -type f

# 根据文件时间戳进行搜索
# UNIX/Linux文件系统每个文件都有三种时间戳：
# - 访问时间（-atime/天，-amin/分钟）：用户最近一次访问时间。
# - 修改时间（-mtime/天，-mmin/分钟）：文件最后一次修改时间。
# - 变化时间（-ctime/天，-cmin/分钟）：文件数据元（例如权限等）最后一次修改时间。
$ find . -type f 时间戳

# 搜索最近七天内被访问过的所有文件
$ find . -type f -atime -7

# 搜索恰好在七天前被访问过的所有文件
$ find . -type f -atime 7

# 搜索超过七天内被访问过的所有文件
$ find . -type f -atime +7

# 搜索访问时间超过10分钟的所有文件
$ find . -type f -amin +10

# 找出比 file.log 修改时间更长的所有文件
$ find . -type f -newer file.log

# 根据文件大小进行匹配
# 文件大小单元：
#  b —— 块（512字节）
#  c —— 字节
#  w —— 字（2字节）
#  k —— 千字节
#  M —— 兆字节
#  G —— 吉字节
$ find . -type f -size 文件大小单元

# 搜索大于10KB的文件
$ find . -type f -size +10k

# 搜索小于10KB的文件
$ find . -type f -size -10k

# 搜索等于10KB的文件
$ find . -type f -size 10k

# 删除匹配文件 删除当前目录下所有.txt文件
$ find . -type f -name "*.txt" -delete

# 根据文件权限/所有权进行匹配 当前目录下搜索出权限为777的文件
$ find . -type f -perm 777

# 找出当前目录下权限不是644的php文件
$ find . -type f -name "*.php" ! -perm 644

# 找出当前目录用户tom拥有的所有文件
$ find . -type f -user tom

# 找出当前目录用户组sunk拥有的所有文件
$ find . -type f -group sunk

# 借助-exec选项与其他命令结合使用 
# 找出当前目录下所有root的文件，并把所有权更改为用户tom
# {} 用于与 -exec 选项结合使用来匹配所有文件，然后会被替换为相应的文件名。
$ find .-type f -user root -exec chown tom {} \;

# 找出自己家目录下所有的.txt文件并删除
# -ok 和 -exec 行为一样，不过它会给出提示，是否执行相应的操作。
$ find $HOME/. -name "*.txt" -ok rm {} \;

# 查找当前目录下所有.txt文件并把他们拼接起来写入到all.txt文件中
$ find . -type f -name "*.txt" -exec cat {} \;> all.txt

# 将30天前的.log文件移动到old目录中
$ find . -type f -mtime +30 -name "*.log" -exec cp {} old \;

# 找出当前目录下所有.txt文件并以 “File:文件名” 的形式打印出来
$ find . -type f -name "*.txt" -exec printf "File: %s\n" {} \;

# 因为单行命令中 -exec 参数中无法使用多个命令，以下方法可以实现在 -exec 之后接受多条命令
$ -exec ./text.sh {} \;

# 搜索但跳出指定的目录
# 查找当前目录或者子目录下所有.txt文件，但是跳过子目录sk
$ find . -path "./sk" -prune -o -name "*.txt" -print

# find其他技巧收集
# 要列出所有长度为零的文件
$ find . -empty

# -exec 接收 find 传过来的所有内容，容易造成溢出
# xargs find 的好伴侣，xargs 将 find 命令查找的结果分成若干模块输出给后面的指令
```

```shell
# grep 内容过滤

# 在文件中搜索一个单词，命令会返回一个包含“match_pattern”的文本行：
$ grep match_pattern file_name
$ grep "match_pattern" file_name

# 输出除之外的所有行 -v 选项：
$ grep -v "match_pattern" file_name

# 标记匹配颜色 --color=auto 选项：
$ grep "match_pattern" file_name --color=auto

# 使用正则表达式 -E 选项：
$ grep -E "[1-9]+"
$ egrep "[1-9]+"

# 只输出文件中匹配到的部分 -o 选项：
$ echo this is a test line. | grep -o -E "[a-z]+\."
line.
$ echo this is a test line. | egrep -o "[a-z]+\."
line.

# 统计文件或者文本中包含匹配字符串的行数 -c 选项：
$ grep -c "text" file_name

# 输出包含匹配字符串的行数 -n 选项：
$ grep "text" -n file_name
$ cat file_name | grep "text" -n

# 搜索多个文件并查找匹配文本在哪些文件中：
$ grep -l "text" file1 file2 file3...

# 在多级目录中对文本进行递归搜索：
$ grep "text" . -r -n

# 忽略匹配样式中的字符大小写：
$ echo "hello world" | grep -i "HELLO"

# 选项 -e 制动多个匹配样式：
$ echo this is a text line | grep -e "is" -e "line" -o
is
line
# 也可以使用-f选项来匹配多个样式，在样式文件中逐行写出需要匹配的字符。
cat patfile
aaa
bbb
$ echo aaa bbb ccc ddd eee | grep -f patfile -o

# 在grep搜索结果中包括或者排除指定文件：
# 只在目录中所有的.php和.html文件中递归搜索字符"main()"
$ grep "main()" . -r --include *.{php,html}
# 在搜索结果中排除所有README文件
$ grep "main()" . -r --exclude "README"
# 在搜索结果中排除filelist文件列表里的文件
$ grep "main()" . -r --exclude-from filelist

# 使用0值字节后缀的grep与xargs：
# 测试文件：
$ echo "aaa" > file1
$ echo "bbb" > file2
$ echo "aaa" > file3
$ grep "aaa" file* -lZ | xargs -0 rm
# 执行后会删除 file1 和 file3，grep 输出用 -Z 选项来指定以 0 值字节作为终结符文件名（\0），xargs -0 读取输入并用 0 值字节终结符分隔文件名，然后删除匹配文件，-Z 通常和 -l 结合使用。

# grep静默输出：
# 不会输出任何信息，如果命令运行成功返回0，失败则返回非0值。一般用于条件测试。
$ grep -q "test" filename

# 打印出匹配文本之前或者之后的行：
# 显示匹配某个结果 之后的3行，使用 -A 选项：
$ seq 10 | grep "5" -A 3
# 显示匹配某个结果 之前的3行，使用 -B 选项：
$ seq 10 | grep "5" -B 3
# 显示匹配某个结果的 前三行和后三行，使用 -C 选项：
$ seq 10 | grep "5" -C 3
```

```shell
# xargs

# 参数：
-a file 从文件中读入作为sdtin
-e flag ，注意有的时候可能会是-E，flag必须是一个以空格分隔的标志，当xargs分析到含有flag这个标志的时候就停止。
-p 当每次执行一个argument的时候询问一次用户。
-n num 后面加次数，表示命令在执行的时候一次用的argument的个数，默认是用所有的。
-t 表示先打印命令，然后再执行。
-i 或者是-I，这得看linux支持了，将xargs的每项名称，一般是一行一行赋值给 {}，可以用 {} 代替。
-r no-run-if-empty 当xargs的输入为空的时候则停止xargs，不用再去执行了。
-s num 命令行的最大字符数，指的是 xargs 后面那个命令的最大命令行字符数。
-L num 从标准输入一次读取 num 行送给 command 命令。
-l 同 -L。
-d delim 分隔符，默认的xargs分隔符是回车，argument的分隔符是空格，这里修改的是xargs的分隔符。
-x exit的意思，主要是配合-s使用。。
-P 修改最大的进程数，默认是1，为0时候为as many as it can ，这个例子我没有想到，应该平时都用不到的吧。
```

### 1.3 VIM

设置 ~/.bashrc

添加 set -o vi 	-- 可以直接使用 vim 的各种快捷键

VIM 快捷键：

<img src="image/vim.gif">

### 1.4 GCC

gcc 工作流程

```shell
# 预处理 头文件展开 宏替换
$ gcc -E hello.c
hello.i

# 生成汇编代码
$ gcc -S hello.i
hello.s

# 将汇编编译成二进制文件 
$ gcc -c hell0.s
hello.o

# 链接
$ gcc hello.o
a.out
```

gcc 参数

```shell
# 指定编译输出的名字
$ gcc main.c -o main

# 通过 -Wall 参数启用所有警告
$ gcc -Wall main.c -o main

# 使用 -E 参数只产生预处理输出
$ gcc -E main.c > main.i

# 使用 -S 参数只产生汇编代码
$ gcc -S main.c > main.s

# 使用 -C 参数只产生编译的代码
$ gcc -C main.c
# 上面的代码产生main.o, 包含机器级别的代码或者编译的代码。

# 使用-save-temps参数产生所有的中间步骤的文件
$ gcc -save-temps main.c
$ ls
a.out  main.c  main.i  main.o  main.s

# 使用 -l 参数链接共享库
$ gcc  -Wall main.c -o main -lCPPfile

# 使用 -fPIC 产生位置无关的代码
# 当产生共享库的时候，应该创建位置无关的代码，这会让共享库使用任意的地址而不是固定的地址，要实现这个功能，需要使用-fPIC参数。
# 下面的例子产生libCfile.so动态库。
$ gcc -c -Wall -Werror -fPIC Cfile.c
$ gcc -shared -o libCfile.so Cfile.o
# 产生共享库的时候使用了-fPIC 参数。
# 注意 -shared 产生共享库。

# 使用 -V 打印所有的执行命令
$ gcc -Wall -v main.c -o main
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/i686-linux-gnu/4.6/lto-wrapper
...

# 使用 -D 参数可以使用编译时的宏
$ gcc -Wall -D MY_MACRO main.c -o main

# 使用 -Werror 将警告升级为错误
$ gcc -Wall -Werror main.c -o main

# 使用 @ 参数从文件中读取参数
# gcc参数可以从文件中读取，通过@后跟文件名的方式提供， 多个参数可以使用空格区隔。
$ cat opt_file 
-Wall -omain
$ gcc main.c @opt_file

# 使用参数 -I 指定头文件的文件夹
$ gcc -I/home/codeman/include input-file.c
# -I 取消前一个参数功能，一般用在 -Idir 之后。

# 使用参数-std指定支持的c++/c的标准
$ gcc -std=c++11 hello-world.cpp

# 使用 -static 生成静态链接的文件 静态编译文件(把动态库的函数和其它依赖都编译进最终文件)
$ gcc main.c -static -o main -lpthread
# 相反的使用 -shared 使用动态库链接。

# 使用 -g 用于 gdb 调试
$ gcc main.c -static -o main -g

# -lstdc++ 指定 gcc 以 c++ 方式编译
$ gcc main.cpp -lstdc++ -o main

# -O 优化选项， 1-3 越高优先级越高
$ gcc main.cpp -lstdc++ -o main -O1

# 使用 -M 生成文件关联的信息
$ gcc -M main.c
main.o: main.c /usr/include/stdc-predef.h /usr/include/stdio.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
 /usr/include/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
```

### 1.5 库文件制作

**静态库制作和使用**

```shell
# 步骤
# 1. 编译为 .o 文件
# 2. 将 .o 文件打包：ar rcs libmycalc.a file1.0 file2.o ...
# 3. 将头文件与库一起发布

# 查看库信息
$ nm libmycalc.a

# 使用
# 编译时 需要加静态库名（记得路径），-I 包含头文件
$ gcc main.c -o app -I include/ -L lib/ -lmycalc

# 优点：
# 1. 执行快
# 2. 发布应用时不需要发布库
# 缺点：
# 1. 执行程序体积比较大
# 2. 库变更时需要重新编译应用
```

**动态库制作和使用**

```shell
# 步骤
# 1. 编译与位置无关的代码，生成 .o 关键参数 -fPIC
# 2. 将 .o 文件打包， 关键参数 -shared
# 3. 将库与头文件一起发布
$ gcc -shared -o libcalc.so *.o

# 使用
# -L 指定动态库路径 -I 指定库名
$ gcc main.c -o app -I include/ -L lib/ -lcalc

# ldd 查看库依赖
$ ldd libcalc.so

# 优点：
# 1. 执行程序体积小
# 2. 库变更时，一般不需要重新发布动态库
# 缺点：
# 1. 执行时需要加载动态库，相对而言，比静态库慢
# 2. 发布应用时需要同时发布动态库

# 解决不能加载动态库的问题
# 1. 拷贝到 /lib 下。 一般不允许
# 2. 将库路径增加到环境变量 LD_LIBRARY_PATH 中，不是特别推荐
# 3. 配置 /etc/ld.so.conf 文件，增加 当前项目库路径，执行 sudo ldconfig -v
```

<img src="image/数据段.png">

### 1.6 Makefile

makefile 的三要素：

- 目标
- 依赖
- 规则命令

写法：

- 目标：依赖
- tab键 规则命令

```makefile
app: main.c add.c sub.c div.c mul.c
	gcc -o app -I./include/ main.c add.c sub.c div.c mul.c
```

如果更改其中一个文件，所有的源码都重新编译

可以考虑编译过程分解，先生成 .o 文件，然后使用 .o 文件编程结果

规则是递归的，依赖文件如果比目标文件新，则重新生成目标文件

```shell
ObjFiles=main.o add.o sub.o div.o mul.o
app: $(ObjFiles)
	gcc -o app -I./include/ $(ObjFiles)

main.o: main.c
	gcc -c main.c -I./include/
add.o: add.c
	gcc -c add.c -I./include/	
sub.o: sub.c
	gcc -c sub.c -I./include/	
div.o: div.c
	gcc -c div.c -I./include/
mul.o: mul.c
	gcc -c mul.c -I./include/
```

makefile 的隐含规则：默认处理第一个目标

```shell
# get all .c files
SrcFiles=$(wildcard *.c)

# all .c files --> .o file
ObjFiles=$(patsubst %.c,%.o,$(SrcFiles))

test:
	echo $(SrcFiles)
	echo $(ObjFiles)
```

makefile 变量：

- $@	代表目标
- $^    代表全部依赖
- $<    第一个依赖
- $?    第一个变化的依赖

```shell
# get all .c files
SrcFiles=$(wildcard *.c)

# all .c files --> .o file
ObjFiles=$(patsubst %.c,%.o,$(SrcFiles))

all:app

# 目标文件用法 $(Var)
app: $(ObjFiles)
	gcc -o $@ -I./include/ $(ObjFiles)
	
# 模式匹配规则， $@ $< 这样的变量，只能在规则中出现
%.o:%.c
	gcc -c $< -I./include/ -o $@

# @ 在规则前代表不输出该条规则的命令
# - 规则前的“-”，代表该条规则报错，仍然继续执行
# .PHONY 定义伪目标，防止有歧义
.PHONY:clean all
clean:
	-@rm -f *.o
	-@rm -f app
```

make -f makefile1	指定makefile文件进行编译

### 1.7 gdb 调试












































