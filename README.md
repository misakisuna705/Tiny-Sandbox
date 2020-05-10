# Ubuntu

<!-- vim-markdown-toc GFM -->

* [使用方式](#使用方式)
    - [編譯](#編譯)
    - [執行](#執行)
    - [清理](#清理)
* [專案結構](#專案結構)
* [函式劫持](#函式劫持)
* [執行結果](#執行結果)

<!-- vim-markdown-toc -->

---

## 使用方式

### 編譯

```zsh
make
```

### 執行

-   make

```zsh
make run ARG="xxx"
```

-   exe

```zsh
bin/main xxx
```

### 清理

```zsh
make clean
```

## 專案結構

```zsh
❯ tree
.
├── LICENSE
├── Makefile
├── README.md
├── bin
│   ├── main
│   └── sandbox.so
├── build
│   ├── main.o
│   └── sandbox.o
├── core
├── doc
│   └── img
├── include
├── lib
└── src
    ├── main.c
    └── sandbox
        └── sandbox.c

8 directories, 10 files
```

## 函式劫持

```c
// one arg and return int
chdir rmdir remove unlink

// one arg and return ptr
opendir

// two args and return int
mkdir create creat64 chmod

// two args and return ptr
fopen fopen64

// two args and return int with two paths checking
rename symlink link

// three args and return int/ssize_t
open open64 chown readlink __xstat __xstat64

// four args and return int
openat openat64

// system and exec checking
system execv execvp execve execl execlp execle
```

## 執行結果

-   regular

![01](doc/img/01.png)

-   other

![02](doc/img/02.png)
