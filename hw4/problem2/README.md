# 第四次作业第二题

## 如何编译运行

本项目使用`QT Creator`构建，代码在`Clion`中编写。要在`Clion`中编译运行项目，
首先应当在`CMakeLists.txt`中加入

```cmake
set(CMAKE_PREFIX_PATH "D:\\Qt\\6.5.2\\mingw_64")
```

来设置`qt cmake`的前缀路径。

接下来进入`Clion`的设置，新建一个`MinGW`工具链，
路径指向`D:\Qt\Tools\mingw1120_64`。

又由于运行项目时，我们的exe需要寻找qt的一些dll。所以我们又要修改环境变量。
直接修改系统的环境变量会搞乱我现有的一套`MinGW`配置，于是我们点开运行的`configure`，
直接在`ENVIRONMENTS`填入`PATH=D:\Qt\6.5.2\mingw_64\bin`。

此时即可完成编译调试与运行。

## 程序运行结果

![screenshot.png](screenshot.png)

我使用多种QT自带的布局管理器管理了程序的布局，程序可以随意缩放，界面的控件也会跟随着变换。

我使用了信号与槽的机制来同步信息，
使用程序时，拖动任意一个Dial都会引起另一个Dial的同步转动和LCD数值的更新。

## 可执行文件路径

可执行文件在`output`目录下，已使用windeployqt进行发布。
