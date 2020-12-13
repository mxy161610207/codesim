# codesim

ISER2020 PA1 - c++源代码相似性度量

课程主页：http://jyywiki.cn/ISER/2020/

实验要求： http://jyywiki.cn/ISER/2020/assignments/PA1

参考论文

- Yanyan Jiang, and Chang Xu. Needle: Detecting Code Plagiarism on Student Submissions. In SIGCSE China, 2018.

```bash
# 代码文件结构
├── include
│   ├── MaximumWeightFlow.h
│   └── MyPaser.h
├── makefile
└── src
    ├── MaximumWeightFlow.cpp
    ├── MyPaser.cpp
    └── main.cpp
```



## Dependencies

本项目运行在Linux 或 Windows Subsystem for Linux(WSL)上，在使用前请确保已安装如下package：

- g++
- objdump
- make

可以通过`-v`参数测试是否已经安装。



## Build & Run

#### Linux

Build:

在linux环境下，请使用make命令构建项目，输出文件为项目根目录下的`codesim`。

```bash
mxy@linux:~$ cd your/project/codesim/dir
mxy@linux:/codesim$ make clean
mxy@linux:/codesim$ make
```

Run：

在本地，以`./`运行`codesim`，接受两个c++源文件，并输出他们之间的差异值，精确到小数。

```bash
mxy@linux:/codesim$ ./codesim --help
usage: codesim [-v|--verbose] [-h|--help] code1 code2
mxy@linux:/codesim$ ./codesim foo.cc bar.cpp
99.3
```

如果不想添加`./`前缀，可以将生成的`codesim`文件复制到`/usr/bin`目录下。

```bash
mxy@linux:/codesim$ sudo mv codesim /usr/bin
mxy@linux:/codesim$ codesim foo.cc bar.cpp
99.3
mxy@linux:/codesim$ sudo rm /usr/bin/codesim 	# delete codesim 
```

