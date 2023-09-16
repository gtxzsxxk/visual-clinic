# Visual Clinic Documentation

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

## 如何调试

首先前往`Toolchain`设置中将debugger从`Bundled`改为`MinGW`，然后前往
`Qt\Tools\mingw1120_64\etc`，修改`gdbinit`为

```shell
python
import sys
sys.path.insert(0, sys.path[0] + '/../share/gcc-11.2.0/python')
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers (None)
end
```

即可正常调试。

## 可执行文件路径

可执行文件在`output`目录下，已使用windeployqt进行发布。

## 工程结构

本工程项目名为`Visual Clinic`，借鉴了`Visual Studio`的命名。其中，可视化对应着作业中多个可视化的要求部分。
本项目的工程结构参考了传统linux的c/c++项目的工程结构，如下所示：

```plaintext
├─include
├─lib
│  └─Eigen
├─resources
└─src
```

- `include`目录用于存放工程公有的除了库以外的头文件
- `lib`目录用于存放工程使用的库的头文件与c文件
- `resources`目录用于存放工程使用的资源文件
- `src`用于存放工程的c文件
- `.`根目录用于存放qt设计师的ui文件与其它杂项

