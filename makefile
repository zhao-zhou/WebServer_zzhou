server: epoll_tool.h main.cpp  ./lock/locker.h ./http/http_conn.cpp ./http/http_conn.h ./CGImysql/sql_connection_pool.cpp ./CGImysql/sql_connection_pool.h  webserver.cpp webserver.h  ./threadpool/threadpool.cpp ./threadpool/threadpool.h
	g++ -o server main.cpp ./http/http_conn.cpp ./CGImysql/sql_connection_pool.cpp webserver.cpp ./threadpool/threadpool.cpp -lpthread -lmysqlclient

clean:
	rm  -r server
