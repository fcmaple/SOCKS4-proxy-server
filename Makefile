CXX=g++
CXXFLAGS=-std=c++17 -Wall -pedantic -pthread -lboost_system -g
CXX_INCLUDE_DIRS:=/usr/local/include
CXX_INCLUDE_PARAMS=$(addprefix -I , $(CXX_INCLUDE_DIRS))
CXX_LIB_DIRS=/usr/local/lib
CXX_LIB_PARAMS=$(addprefix -L , $(CXX_LIB_DIRS))

all: socks_server console http_server

http_server: http_server.cpp
	$(CXX) -o $@ $^ $(CXX_LIB_PARAMS) $(CXX_INCLUDE_PARAMS) $(CXXFLAGS)
socks_server: socks_server.cpp 
	$(CXX)  -o $@ $^ $(CXX_INCLUDE_PARAMS) $(CXX_INCLUDE_PARAMS)  $(CXXFLAGS)
console: console.cgi.cpp print_shell.cpp
	$(CXX) -o hw4.cgi $^ $(CXX_INCLUDE_PARAMS) $(CXX_LIB_PARAMS) $(CXXFLAGS)

clean:
	rm -f socks_server hw4.cgi http_server
