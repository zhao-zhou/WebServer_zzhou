/*http请求类型如下*/
```
    1. Get 请求指定的页面信息，并返回实体主体
    2. Post  向指定资源提交数据进行处理请求（例如提交表单或者上传文件）。数据被包含在请求体中。POST请求可能会导致新的资源的建立和/或已有资源的修改。
    3. Head 类似于get请求，但是响应消息没有内容，只是获得报头
    4. Put 从客户端向浏览器传送的数据取代指定的文档内容
    5. Delete 请求服务器删除指定的页面
    6. Connect  HTTP/1.1协议中预留给能够将连接改为管道方式的代理服务器
    7. Options 允许客户端查看浏览器的性能
    8. Trace 回显服务器收到的请求，主要用于测试和诊断
```

/*http常见状态码*/
```
    • 200 OK  客户端请求成功
    • 301 Moved Permanently 重定向
    • 400 Bad Request       客户端请求有语法错误，不能被服务器所理解
    • 401 Unauthorized      请求未经授权，这个状态代码必须和WWW-Authenticate报头域一起使用 
    • 403 Forbidden            服务器收到请求，但是拒绝提供服务
    • 404 Not Found           请求资源不存在，eg：输入了错误的URL
    • 500 Internal Server Error     服务器发生不可预期的错误
    • 503 Server Unavailable        服务器当前不能处理客户端的请求，一段时间后可能恢复正常
```

/*va_list搭配vsnprintf实现不定参数长度的写入*/
```
 头文件：#include <stdarg.h>提供方法实现函数为可变参数表的函数
 int fun(int n, ...);
 定义特殊类型 va_list 
 初始化：va_start(va_list tmp, 最后一个普通参数（n）);
 访问： 类型名 va_arg(va_list tmp, return类型名)
 根据格式写入： vsnprintf(char* str, int str.size, format, va_list)
 结束： va_end(va_list tmp); 
```