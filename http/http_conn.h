#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <map>

#include "../lock/locker.h"
#include "../CGImysql/sql_connection_pool.h"
#include "../epoll_tool.h"

//定义http响应的一些状态信息
static const char *ok_200_title = "OK";
static const char *error_400_title = "Bad Request";
static const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
static const char *error_403_title = "Forbidden";
static const char *error_403_form = "You do not have permission to get file form this server.\n";
static const char *error_404_title = "Not Found";
static const char *error_404_form = "The requested file was not found on this server.\n";
static const char *error_500_title = "Internal Error";
static const char *error_500_form = "There was an unusual problem serving the request file.\n";

class http_conn
{
public:
    static const int FILENAME_LEN = 200;
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;

    //报文请求方式，目前只使用GET，POST
    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };

    //主状态机状态
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,//请求行
        CHECK_STATE_HEADER,         //请求头
        CHECK_STATE_CONTENT         //请求内容
    };

    //解析的结果反馈
    enum HTTP_CODE
    {
        NO_REQUEST,         //请求不完整
        GET_REQUEST,        //请求完整
        BAD_REQUEST,        //请求有误
        NO_RESOURCE,        //无请求
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };

    //从状态机状态
    enum LINE_STATUS
    {
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN   //读取不完整
    };

public:
    http_conn() {}
    ~http_conn() {}

public:
    void init(int sockfd, const sockaddr_in &addr, char *root, int TRIGMode,
                string user, string passwd, string sqlname);
    void close_conn(bool real_close = true);
    void process();
    bool read_once();
    bool write();
    void initmysql_result(connection_pool *connPool);
    int timer_flag;
    int improv;//模式选择：reactor，proactor

private:
    void init();
    char *get_line() { return m_read_buf + m_start_line; };//移动字符串

    //对请求报文解析，最后调用do_request生成响应报文
    HTTP_CODE process_read();
    HTTP_CODE parse_request_line(char *text);
    HTTP_CODE parse_headers(char *text);
    HTTP_CODE parse_content(char *text);
    HTTP_CODE do_request();
    bool process_write(HTTP_CODE ret);
    LINE_STATUS parse_line();

    void unmap();

    //生成响应报文，由do_request调用
    bool add_response(const char *format, ...);
    bool add_content(const char *content);
    bool add_status_line(int status, const char *title);
    bool add_headers(int content_length);
    bool add_content_type();
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_blank_line();

public:
    static int m_epollfd;
    static int m_user_count;
    MYSQL *mysql;
    int m_state;  //读为0, 写为1

private:
    CHECK_STATE m_check_state;
    METHOD m_method;

    int m_sockfd;
    sockaddr_in m_address;

    //记录read的解析字符
    char m_read_buf[READ_BUFFER_SIZE];
    int m_read_idx;
    int m_checked_idx;
    int m_start_line;

    //记录响应数据
    char m_write_buf[WRITE_BUFFER_SIZE];
    int m_write_idx;
    
    //响应的6部分
    char m_real_file[FILENAME_LEN];
    char *m_url;
    char *m_version;
    char *m_host;
    int m_content_length;
    bool m_linger;  //长短连接


    char *m_file_address;
    struct stat m_file_stat;
    struct iovec m_iv[2];
    int m_iv_count;
    int cgi;        //是否启用的POST
    char *m_string; //存储请求头数据
    int bytes_to_send;
    int bytes_have_send;
    char *doc_root;

    map<string, string> m_users;
    int m_TRIGMode;//触发模式，LT or ET

    char sql_user[100];
    char sql_passwd[100];
    char sql_name[100];

    char userName[50];

private:
    bool uploadTxt();
    bool downloadTxt();
};

#endif
