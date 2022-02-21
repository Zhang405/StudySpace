CXX ?= g++

CXXFLAGS += -g



server: main.cpp  ./timer/lst_timer.cpp ./http/http_conn.cpp ./log/log.cpp ./CGImysql/sql_connection_pool.cpp  ./webserver/webserver.cpp config.cpp
	$(CXX) -o server  $^ $(CXXFLAGS)  $$(mysql_config --cflags --libs)

clean:
	rm  -r server
