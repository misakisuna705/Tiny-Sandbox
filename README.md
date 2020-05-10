# Ubuntu

<!-- vim-markdown-toc GFM -->

* [使用方式](#使用方式)
    - [編譯](#編譯)
    - [執行](#執行)
    - [清理](#清理)
* [專案結構](#專案結構)
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

## 執行結果

-   regular

![01](doc/img/01.png)

-   other

![02](doc/img/02.png)
