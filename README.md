
*服务器框架及实现*
==================
> **模型**
  >> epoll模型实现高并发
  >> Reactor事件处理实现主线程监控，工作线程处理I/O
  >> Proactor事件处理实现主线程处理I/O，工作线程处理逻辑
  >> 实现主线程和工作线程在LT及ET下组合运行

> **线程池**
  >> 避免线程重复创建的开销，默认初始建立8个线程对请求进行处理
  >> 采用信号量控制线程池

> **数据库队列**
  >> 同线程池，建立8个供连接的数据库

*服务器运行流程*
================
> **安装MySQL**
  >> 可参考：https://blog.csdn.net/weixx3/article/details/80782479
    
> **建立符合Webserver的数据库**
  ```
  create database base;

  use base;
  create table user(
    username char(50) NULL,
    passwd char(50) NULL,
    txtnum int 0
  )ENGINE=InnoDB;
 
  insert into user(username, passwd, txtnum) values('name', 'passwd', 'txtnum');
  ```

> **修改数据库初始化信息**
    ```main.cpp```
    string user = "user";
    string passwd = "passwd";
    string databasename = "base";

> **代码运行**
  sh ./build.sh
  ./server

> **客户端访问：** localhost:ip

> **本项目已搭建在云服务区：** http://162.14.81.75:1997/ </br>
 ![mainInterface](https://github.com/zhao-zhou/WebServer_zzhou/blob/master/root/interface.png)

*默认设置*
===========
  > -p，自定义端口号，默认1997

  > -m，listenfd和connfd的模式组合，默认使用LT + LT
    ```参考webserver的trig_mode函数```
	
  > -s，数据库连接数量，默认：8

  > -t，线程池大小默认为8

  > -a，选择反应堆模型，默认0
	  >>> 0，Proactor模型
	  >>> 1，Reactor模型

*测试*
========
> **使用测试工具：** Webbench

> **测试代码**
  >> 进入TestTool文件夹执行webbench
  >> webbench -c 客户端数目 -t 测试时间 http://162.14.81.75:1997/

> **测试结果**
  >> 100 clients, running 10 sec.
  >> Speed=42384 pages/min, 79116 bytes/sec.
  >> Requests: 7064 susceed, 0 failed.
    
