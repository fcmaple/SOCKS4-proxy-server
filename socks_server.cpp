#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include<stdio.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <vector>
#include <sys/socket.h>

#include <cstring>
using boost::asio::io_context;
using boost::asio::ip::tcp;
using namespace std;
boost::asio::io_context io_context;
class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),data_acceptor_(io_context),resolver(io_context),socket_(io_context),socket_client(io_context)
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
           
            signal(SIGCHLD,SIG_IGN);
            socket_client=move(socket);
            io_context.notify_fork(io_context::fork_prepare);
            
            if(fork()==0){
              //socket_client=move(socket);
                io_context.notify_fork(io_context::fork_child);
                acceptor_.close();
                
                strcpy(SERVER_ADDR,socket_client.local_endpoint().address().to_string().c_str());
                strcpy(REMOTE_ADDR,socket_client.remote_endpoint().address().to_string().c_str());
                sprintf(SERVER_PORT,"%u",socket_client.local_endpoint().port());
                sprintf(REMOTE_PORT,"%u",socket_client.remote_endpoint().port());
                size_t len = socket_client.read_some(boost::asio::buffer(data_, max_length));
                /*
                for(int i=0;i<(int)len;i++){
                  printf("%u ",data_[i]);
                }*/

                VN = data_[0];
                CD = data_[1];
                //printf(" CD = %u\n",CD);
                DSPPORT[0] =  data_[2];
                DSPPORT[1] =  data_[3];
                DSP = DSPPORT[0]*256+DSPPORT[1];
                DSPIP[0] = data_[4];
                DSPIP[1] = data_[5];
                DSPIP[2] =data_[6];
                DSPIP[3] =data_[7]; 
                if(data_[4]==0 && data_[5]==0 && data_[6]==0 && data_[7]!=0) AA=true;
                DSP_IP = to_string(DSPIP[0])+"."+to_string(DSPIP[1])+"."+to_string(DSPIP[2])+"."+to_string(DSPIP[3]);
                if(AA) do_resolve((int)len);
                DSPORT = to_string(DSP);
                NU = data_[(int)len-1];
                
                if(do_firewall()){
                  if(CD==1) do_connect(DSP_IP,DSPORT);
                  else if(CD ==2) do_bind();
                }else{
                  do_reply(91);
                }
               
                //socket_client.close();
            }
            else{
               io_context.notify_fork(io_context::fork_parent);
                socket_client.close();
                do_accept(); 
            }
             
          }
        });
        
       
        
  }
  void do_resolve(int len){
    bool domain_start = false;
    for(int i=8;i<len-1;i++){
      if(domain_start){
        domain_.push_back(data_[i]);
      }
      if(data_[i]==0) {
        domain_start = true;
      }
    }
    
    DSP_IP = domain_;


  }
  bool do_firewall(){
    char per[20];
    char wall_cmd[10];
    char wall_ip[20];
    string permit = "permit";
    string filename("socks.conf");
    ifstream input_file(filename);
    string line="";
    if(!input_file.is_open()){
        cerr<<"file is open\n";
      }
      while(getline(input_file,line,'\n')){
        string new_line = "";
        for(auto s : line) {
          if(s!='\r' && s!='\n') new_line +=s;
        }
        sscanf(new_line.c_str(),"%s %s %s",per,wall_cmd,wall_ip);
        // cout<<per<<" "<<wall_cmd<<" "<<wall_ip<<endl;
        bool same = true;
        if(CD ==0x01){
          if(wall_cmd[0] == 'c'){
            
            for(int i=0;i<(int)sizeof(wall_ip);i++){
              if(wall_ip[i]=='*') 
              {
                input_file.close();
                return true;
              }
              if(wall_ip[i] !=DSP_IP[i]) {
                same=false;
                break;
              }
            }
            if(same){
              input_file.close();
              return true;
            }
          }
        }else{
          if(wall_cmd[0] == 'b'){
            for(int i=0;i<(int)sizeof(wall_ip);i++){
              if(wall_ip[i] !=DSP_IP[i] && wall_ip[i]!='*') break;
            }
            input_file.close();
            return true;
          }
        }

      }
      input_file.close();
      return false;
  }
  void do_bind(){
    unsigned short bind_port(0);
    tcp::endpoint endpoint(tcp::v4(),bind_port);
    data_acceptor_.open(endpoint.protocol());
    //data_acceptor_.set_option(tcp::acceptor::reuse_address(true));
    data_acceptor_.bind(endpoint);
    data_acceptor_.listen();
    new_port = data_acceptor_.local_endpoint().port();
    address_ = data_acceptor_.local_endpoint().address().to_v4().to_bytes();


    do_reply(90);

  }
  void do_connect(string addr,string port){
      boost::system::error_code err;
      tcp::resolver::iterator endpoint_it  = resolver.resolve(addr,port,err);
      
      tcp::resolver::iterator end;
      boost::system::error_code er;
      tcp::endpoint endpoint = *endpoint_it;
      DSP_IP = endpoint.address().to_v4().to_string();
     if(err){
        do_reply(91);
        return ;
      } 
      if(!er){
          socket_.async_connect(endpoint,[this,addr,port](boost::system::error_code err){
              
                if(!err && socket_.native_handle()!=-1){
                  
                  do_reply(90);
                  }
                else if(socket_.native_handle()==-1)
                {
                  socket_.close();
                  do_connect(addr,port);
                }
                else{
                  
                }
            });
      }else{
        unsigned char re = 91;
       
        do_reply(re);
        
      }

    }
  void do_reply(unsigned char cmd ){
    unsigned char res[8]={};
    
    res[0] = 0;
    res[1] = cmd;
    if(CD==1 && cmd ==90){
      res[2] = data_[2];
      res[3] = data_[3];
      res[4] = data_[4];
      res[5] = data_[5];
      res[6] = data_[6];
      res[7] = data_[7];
    }else if (CD==2 && cmd==90){
      res[2] = (new_port>>8) & 0xff;
      res[3] = new_port & 0xff;
      res[4] = address_[0];
      res[5] = address_[1];
      res[6] = address_[2];
      res[7] = address_[3];
    }else{
      for(int i=2;i<8;i++) res[i]=0;
    }
    /*
    for(int i=0;i<8;i++){
      printf("%u ",res[i]);
    }
    printf("%u ",new_port);
    cout<<endl;*/
    cout<<"<S_IP>: "<<REMOTE_ADDR<<endl;
    cout<<"<S_PORT>: "<<REMOTE_PORT<<endl;
    cout<<"<D_IP>: "<<DSP_IP<<endl;
    cout<<"<D_PORT>: "<<DSPORT<<endl;
    if(CD==1) cout<<"<Command>: CONNECT\n";
    else if(CD==2) cout<<"<Command>: BIND\n";
    if(cmd==90) cout<<"<Reply>: Accept\n";
    else if(cmd ==91) cout<<"<Reply>: Reject\n";
    boost::asio::write(socket_client, boost::asio::buffer(res, sizeof(res)));
    if(CD==2 && cmd==90){
      boost::system::error_code ec;
      data_acceptor_.accept(socket_,ec);
      if (!ec)
          {
            boost::asio::write(socket_client, boost::asio::buffer(res, sizeof(res)));
            do_read();
            do_read_client();
          }
          else{
            
          }
    }else if(CD ==1 && cmd ==90){
      do_read();
      do_read_client();
    }
    

  }
  void do_read_client(){
    
    socket_client.async_read_some(boost::asio::buffer(read_buf_client, max_length),
    [this](boost::system::error_code ec, std::size_t length)
    {
      if(!ec){
        /*
        int len = length;
        for(int i=0;i<len;i++){
          printf("%c",read_buf_client[i]);
        }*/
        do_write_out(length);        
      }else{
       
        exit(0);
        
      }
      
    });
  }
  
  void do_read(){

    socket_.async_read_some(boost::asio::buffer(read_buf_out, max_length),
    [this](boost::system::error_code ec, std::size_t length)
    {
      if(!ec){
        /*
        int len = length;
        for(int i=0;i<len;i++){
          printf("%c",read_buf_out[i]);
        }*/
        do_write(length);
      }else{
        
        exit(0);
      }
      
    });

  }
  void do_write_out(int length){
    boost::asio::async_write(socket_,boost::asio::buffer(read_buf_client, length),
    [this](boost::system::error_code ec, std::size_t length)
    {
      if(!ec){
      
        do_read_client();
      }else{
        
        exit(0);
      }
      
    });
    
  }
  void do_write(int length){
    boost::asio::async_write(socket_client, boost::asio::buffer(read_buf_out, length),
    [this](boost::system::error_code ec, std::size_t length)
    {
      if(!ec){
        do_read();
      }else{
        
        exit(0);
      }
      
    });
   
  }
  int count =0;
    enum { max_length = 50000 };
    boost::asio::ip::address_v4::bytes_type address_;
  unsigned char data_[max_length];
  
  unsigned char read_buf_out[max_length] = {};
  unsigned char read_buf_client[max_length]={};
  unsigned char domain_char[max_length]={};
  string domain_ = "";
  char SERVER_ADDR[max_length];
  char SERVER_PORT[max_length];
  char REMOTE_ADDR[max_length];
  char REMOTE_PORT[max_length];
  unsigned short new_port;
  int DSPPORT[2];
  int DSPIP[4];
  unsigned char userid[5];
  string DSP_IP;
  string DSPORT;
  int NU;
  int DSP;
  int VN;
  int CD;
  pid_t pid;
  bool AA = false;
  tcp::acceptor acceptor_;
  tcp::acceptor data_acceptor_;
  tcp::resolver resolver;
  tcp::socket socket_;
  tcp::socket socket_client;

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

    

    server s(io_context, std::atoi(argv[1]));

    io_context.run();
  }
  
  catch (std::exception& e)
  {
    //std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}