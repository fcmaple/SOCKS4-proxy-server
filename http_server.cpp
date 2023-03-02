#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

using boost::asio::io_context;
using boost::asio::ip::tcp;
using namespace std;
boost::asio::io_context io_context;


class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    do_read();
  }

private:
  void do_read()
  {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          cout<<data_<<endl;
          sscanf(data_,"%s %s %s %s %s",REQUEST_METHOD,REQUEST_URI,SERVER_PROTOCOL,HOST_NAME,HTTP_HOST);
          if (!ec)
          {
            do_write(length);
          }
        });
  }

  void do_write(std::size_t length)
  {
    auto self(shared_from_this());
    string res="";
    for(auto i:SERVER_PROTOCOL)
    {
      if(i=='\0') break;
      res+=i;
    }
    res+=" 200 OK\n";
    strcpy(res_data,res.c_str());
    cout<<"res_data = "<<res_data;
    boost::asio::async_write(socket_, boost::asio::buffer(res_data, strlen(res_data)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            strcpy(SERVER_ADDR,socket_.local_endpoint().address().to_string().c_str());
            strcpy(REMOTE_ADDR,socket_.remote_endpoint().address().to_string().c_str());
            sprintf(SERVER_PORT,"%u",socket_.local_endpoint().port());
            sprintf(REMOTE_PORT,"%u",socket_.remote_endpoint().port());
            bool q = false;
            int j = 0;
            for(int i=0;i<max_length;i++){
              if(REQUEST_URI[i]=='\0') break;
              if(REQUEST_URI[i]=='?'){
                q = true;
                continue;
              }
              if(q){
                QUERY_STRING[j] = REQUEST_URI[i];
                j++;
              }
            }
            string exec= ".";
            for(int i=0;i<max_length;i++){
              if(REQUEST_URI[i]=='\0' || REQUEST_URI[i]=='?') break;
              exec += REQUEST_URI[i];
            }
            cout<<"exec = "<<exec<<endl;
            
            io_context.notify_fork(io_context::fork_prepare);
            if(fork()==0){
              io_context.notify_fork(io_context::fork_child);
              setenv("REQUEST_METHOD", REQUEST_METHOD, 1);
              setenv("REQUEST_URI", REQUEST_URI, 1);
              setenv("QUERY_STRING", QUERY_STRING, 1);
              setenv("SERVER_PROTOCOL", SERVER_PROTOCOL, 1);
              setenv("HTTP_HOST", HTTP_HOST, 1);
              setenv("SERVER_ADDR", SERVER_ADDR, 1);
              setenv("SERVER_PORT", SERVER_PORT, 1);
              setenv("REMOTE_ADDR", REMOTE_ADDR, 1);
              setenv("REMOTE_PORT", REMOTE_PORT, 1);
              cout<<getenv("REQUEST_METHOD")<<endl;
              cout<<REQUEST_URI<<endl;
              cout<<HTTP_HOST<<endl;
              int sock = socket_.native_handle();
              dup2(sock,STDIN_FILENO);
              dup2(sock,STDOUT_FILENO);
              dup2(sock,STDERR_FILENO);
              socket_.close();
              if(execlp(exec.c_str(),exec.c_str(),NULL)<0){
                cout << "Content-type:text/html<pre>FAIL</pre>";
              }
            }else{
              io_context.notify_fork(io_context::fork_parent);
              socket_.close();
            }
          }
        });
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  char res_data[max_length];
  char REQUEST_URI[max_length];
  char REQUEST_METHOD[max_length];
  char QUERY_STRING[max_length];
  char SERVER_PROTOCOL[max_length];
  char HTTP_HOST[max_length];
  char SERVER_ADDR[max_length];
  char SERVER_PORT[max_length];
  char REMOTE_ADDR[max_length];
  char REMOTE_PORT[max_length];
  char HOST_NAME[max_length];
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket))->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    server s(io_context, std::atoi(argv[1]));

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}