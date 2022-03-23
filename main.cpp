#include "webserver.h"

using namespace std;

void analy_arg(int argc, char*argv[]);

struct defaultPara{
    //端口号,默认1997
    int PORT = 1997;

    //触发组合模式,默认listenfd LT + connfd LT
    int TRIGMode = 0;

    //listenfd触发模式，默认LT
    int LISTENTrigmode = 0;

    //connfd触发模式，默认LT
    int CONNTrigmode = 0;

    //数据库连接池数量,默认8
    int sql_num = 8;

    //线程池内的线程数量,默认8
    int thread_num = 8;

    //并发模型,默认是proactor
    int actor_model = 0;
}myServerPara;

int main(int argc, char *argv[])
{
    string user = "root";
    string passwd = "192845";
    string databasename = "webList";

    //对main参数解析
    analy_arg(argc, argv);

    WebServer server;
    
    //初始化
    server.init(myServerPara.PORT, user, passwd, databasename, myServerPara.TRIGMode, 
                myServerPara.sql_num,  myServerPara.thread_num, myServerPara.actor_model);

    //数据库
    server.sql_pool();

    //线程池
    server.thread_pool();

    //触发模式
    server.trig_mode();

    //监听
    server.eventListen();

    //运行
    server.eventLoop();

    return 0;
}

void analy_arg(int argc, char*argv[]){
    int opt;
    const char *str = "p:m:s:t:a:";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 'p':
        {
            myServerPara.PORT = atoi(optarg);
            break;
        }
        case 'm':
        {
            myServerPara.TRIGMode = atoi(optarg);
            break;
        }
        case 's':
        {
            myServerPara.sql_num = atoi(optarg);
            break;
        }
        case 't':
        {
            myServerPara.thread_num = atoi(optarg);
            break;
        }
        case 'a':
        {
            myServerPara.actor_model = atoi(optarg);
            break;
        }
        default:
            break;
        }
    }
}