[toc]

> 阅读本文大概需要 4 分钟

# Qt Creator 源码学习笔记 01

`Qt Creator` 是一款开源的轻量级 `IDE`，整个架构代码全部使用 `C++/Qt` 开发而成，非常适合用来学习`C++`和`Qt` 知识，这也是我们更加深入学习`Qt`最好的方式，学习了解官方人员是如何开发一款软件的

## 下载

一般我们下载`Qt`进行安装的时候会默认安装好`Qt Creator`，此时安装的是可执行程序，而我们今天要讲的是下载源码并且编译，那么首先要到官方仓库中下载某个版本的源码

下载方式有两种，下载zip源码包和`gigt clone 获取`

[直接下载，打开通用下载页面下载](https://download.qt.io/official_releases/qtcreator/])

由于目前更新速度比较快，导致版本很多，不建议直接下载最新版本，这里以4.6版本为例

![](https://gitee.com/devstone/imageBed/raw/master/images/202110312327818.png)

通过这种方式下载下来直接解压就能看到整个框架源码

![](https://gitee.com/devstone/imageBed/raw/master/images/202110312331077.png)

另一种通过`git`方式下载源码更推荐，下载下来后你相当于拥有了所有版本的源码了，想要学习那个版本直接切换就行了，非常方便，下载地址有两个

- 官方: https://code.qt.io/cgit/qt-creator/qt-creator.git
- GitHub 镜像: https://github.com/qt-creator/qt-creator.git

命令分别为

```
 git clone git://code.qt.io/qt-creator/qt-creator.git
 
 git clone https://github.com/qt-creator/qt-creator.git
```

![](https://gitee.com/devstone/imageBed/raw/master/images/202110312350289.png)

可能有时候遇到网速慢等原因，下载会持续一段时间，如果等了好久还是下载慢，那么建议直接选择`Download Zip`，一般非常快

下载完成后，进入该源码目录执行切换版本命令即可



## 编译

源码下载完成后就可以开始动手直接尝试编译了，先不用详细了解每个目录是干啥的，跑起来再说

开始编译前，确保你电脑已经安装好了`Qt`，并且最好是 `Qt 5.6.2`及以上的版本

为啥会是这个呢，打开配置文件就可以看到

```
#version check qt
!minQtVersion(5, 6, 2) {
    message("Cannot build $$IDE_DISPLAY_NAME with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.6.2.")
}
```

直接打开工程文件`qtcreator.pro`即可，等待其加载完成，打开plugin.pro配置文件，只保留`coreplugin`插件工程，其余的都注释掉，暂时用不到，我们开始只关心核心插件

加载完成后直接构建编译即可，如果不出意外等待大概 10 多分钟时间就可以编译完成，你可以看到如下界面


![](https://gitee.com/devstone/imageBed/raw/master/images/202111011131568.png)


[^1]: https://www.devbean.net/2016/07/qt-creator-source-study-02/






