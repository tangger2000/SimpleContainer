# SimpleContainer V1说明文档

## 前言

出于对Docker的好奇，我了解了一些Docker的底层实现技术的皮毛。刚好最近在学习c++，于是想着写一个简单的容器，原理是给予Linux namespace。至于更深入的Cgroups等，还未看到，暂时无法写出来，留给时间吧！

## 一、环境依赖

（一）运行环境

经测试ubuntu14、16、18都没有问题

（二）依赖

需要g++

需要系统的root权限

需要iptables2

## 二、程序功能说明

### （一）隔离：

实现了UTS、Mount、IPC、PID、NETWORK共五种隔离。

### （二）文件系统：

装载了一个busybox（文件系统）。添加了更多的文件支持，经过测试，基本上常用的命令（/bin和/usr/bin里面有些命令我没有添加so库）都能实现。你可以理解为我为这个容器装载了一个辣鸡镜像。

### （三）自定义：

可自定义容器的hostname以及装载的rootfs（busybox）绝对路径。通过命令参数实现，目前仅有三个可选项。

## 三、使用说明

一、下载仓库内所有文件。（建议git clone）

二、将rootfs.tar.gz解包，可解包至任意路径。但是必须保证，\*.hpp和\*.cpp处于同一文件路径下，否则编译失败！

三、检查本机环境，是否有gcc（g++)/clang/make等编译工具以及iptables2。PS：我只会g++,所以只能g++做演示，见谅！

> g++检查方式：
>
> 在命令行下，输入 g++ -v   。然后会有一堆版本信息。
>
> iptables检查方式：
>
> 别检查了，直接apt install iptables2    。

四、编译：

> g++ main.cpp -o simplecontainer -std=c++11

五、启动运行：

> sudo ./simplecontainer -n "你的容器Hostname" -d "你的rootfs路径"
>
> eg： sudo ./simplecontainer -n "test" -d "/rootfs"
>
> 如果需要寻求帮助：
>
> ./simplecontainer -h

六、然后进去瞧两眼。

七、退出

### 说明

因为这玩意儿写着好玩儿的，没有配置nat转发，如果您让容器内可以ping通百度之内的，请自己设置nat转发。默认外部veth0的ip是10.0.0.1，容器内eth0（veth1改名）ip为10.0.0.2。

## 致谢

**超链接不分先后！** 

[酷壳：DOCKER基础技术：LINUX NAMESPACE（上）](https://coolshell.cn/articles/17010.html)

[yadutaf：Introduction to Linux namespaces](https://blog.yadutaf.fr/2014/01/12/introduction-to-linux-namespaces-part-4-ns-fs/)

[子厚.：容器基础(二): 使用Namespace进行边界隔离](https://www.cnblogs.com/aios/p/10035923.html)

[[Linux虚拟网络设备之bridge(桥)](https://segmentfault.com/a/1190000009491002)](https://segmentfault.com/a/1190000009491002)

[[Linux下getopt()函数的简单使用](https://www.cnblogs.com/qingergege/p/5914218.html)](https://www.cnblogs.com/qingergege/p/5914218.html)

[linux 网络虚拟化： network namespace 简介](https://cizixs.com/2017/02/10/network-virtualization-network-namespace/)

[理解Docker容器网络之Linux Network Namespace](https://tonybai.com/2017/01/11/understanding-linux-network-namespace-for-docker-network/)

