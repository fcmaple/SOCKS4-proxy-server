#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <boost/asio.hpp>

#include "printHTML.h"
using namespace std;
using boost::asio::ip::tcp;

class socks_session: public std::enable_shared_from_this<socks_session>
{
public:
    socks_session(tcp::socket socket,string addr_,string port_,string text_,int position_) 
    :socket_(std::move(socket))
    {
      set_(addr_,port_,text_,position_);
      string addr_port = addr+":"+port;
      print_content_data(addr_port,position+5);
    }
    void start(){
      do_openfile();
      do_read(0,false);
    }
    
    ~socks_session() {
        //std::cout << "Destructor for AsyncHttpConnection\n";
    }
private:
    void set_(string addr_,string port_,string text_,int position_) {
      text = text_ ;
      addr = addr_;
      port = port_;
      position = position_;
    }
    void do_openfile(){
      
      string filename("test_case/"+text);
      //cout<<filename<<endl;
      string line;
      ifstream input_file(filename);
      if(!input_file.is_open()){
        cerr<<"file is open\n";
      }
      while(getline(input_file,line,'\n')){
        string new_line = "";
        for(auto s : line) {
          if(s!='\r' && s!='\n') new_line +=s;
        }
        new_line+="\n";
        lines.push_back(new_line);
      }
      input_file.close();
    }
    void do_read(int l , bool ing) {
      auto self(shared_from_this());
      socket_.async_read_some(boost::asio::buffer(data_, max_length),[this, self,l,ing](boost::system::error_code ec, std::size_t length) {
        if (!ec) {

          int sz = length;
          bool stop = false;
          bool who_cmd = false;
          string content = "";
          string test_who="";
          if(l>0){
            for(auto dd:lines[l-1]){
              if(dd!='\r' && dd!='\n') test_who+=dd;
            }
            if(test_who=="who") who_cmd=true;
          } 
          for(int i=0;i<sz;i++){
              if(stop){
                content += data_[i];
                print_content_data(content,position);
                break;
              }
              if(data_[i]=='\n') {
                content +="&NewLine;";
                //cout<<content;
                print_content_data(content,position);
                content = "";
                
              }else if(data_[i]=='%'){
                stop = true;
                content += data_[i];
                //print_content_data(string(1,data_[i]),position);
              }
              else{
                if(data_[i]=='\r') {
                  continue;
                }
                if(who_cmd){
                  if(data_[i]=='<' || data_[i]=='>'){
                    content += data_[i];
                    print_content_data(content,position);
                    content="";
                    continue;
                  }

                }
                
                content += data_[i];
                if(i == sz-1){
                  print_content_data(content,position);
                }
              }
          }
          if(content[0]!='%') do_read(l,true);
          else{
            string command = "";
            string w_cmd = "";
            for(auto s:lines[l]){
              if(s!='\r' && s!='\n') command+=s;
            }
            //int ssz = lines[l].size();
              //cout<<"<script>document.getElementById(\"s1\").innerHTML+= \"";
              
              //for(int i=0;i<ssz-2;i++) command += lines[l][i];
              w_cmd = command +"\n";
              command += "&NewLine;";
              print_title_data(command,position);
              strcpy(write_buf,w_cmd.c_str());
              do_write(w_cmd.size(),l);
          }
        }else{
          //cout<<ec.message()<<endl;
        }
      });
    }
    void do_write(std::size_t length,int l) {
      
    auto self(shared_from_this());
    boost::asio::async_write(
        socket_, boost::asio::buffer(write_buf, length),
        [this, self,l](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            do_read(l+1,false);
          }
          else{
            //cout<<ec.message()<<endl;
          }
        });
  }
    
    
  tcp::socket socket_;
  vector<string> lines;
  std::size_t length = 1000;
  enum { max_length = 10000 };
  char data_[max_length];
  char write_buf[max_length];
  int n_buf = 0;
  string cmdd;
  int position;
  string port="";
  string addr="";
  string text="";
  int now_line = 0;
};
class session: public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket,string addr_,string port_,string text_,int position_) 
    :socket_(std::move(socket))
    {
      set_(addr_,port_,text_,position_);
      string addr_port = addr+":"+port;
      print_content_data(addr_port,position+5);
    }
    void start(){
      do_openfile();
      do_read(0,false);
    }
    
    ~session() {
        //std::cout << "Destructor for AsyncHttpConnection\n";
    }
private:
    void set_(string addr_,string port_,string text_,int position_) {
      text = text_ ;
      addr = addr_;
      port = port_;
      position = position_;
    }
    void do_openfile(){
      
      string filename("test_case/"+text);
      //cout<<filename<<endl;
      string line;
      ifstream input_file(filename);
      if(!input_file.is_open()){
        cerr<<"file is open\n";
      }
      while(getline(input_file,line,'\n')){
        string new_line = "";
        for(auto s : line) {
          if(s!='\r' && s!='\n') new_line +=s;
        }
        new_line+="\n";
        //cout<<new_line;
        lines.push_back(new_line);
      }
      input_file.close();
    }
    void do_read(int l , bool ing) {
      auto self(shared_from_this());
      socket_.async_read_some(boost::asio::buffer(data_, max_length),[this, self,l,ing](boost::system::error_code ec, std::size_t length) {
        if (!ec) {

          int sz = length;
          bool stop = false;
          bool who_cmd = false;
          string content = "";
          string test_who="";
          if(l>0){
            for(auto dd:lines[l-1]){
              if(dd!='\r' && dd!='\n') test_who+=dd;
            }
            if(test_who=="who") who_cmd=true;
          } 
          for(int i=0;i<sz;i++){
              if(stop){
                content += data_[i];
                print_content_data(content,position);
                break;
              }
              if(data_[i]=='\n') {
                content +="&NewLine;";
                //cout<<content;
                print_content_data(content,position);
                content = "";
                
              }else if(data_[i]=='%'){
                stop = true;
                content += data_[i];
                //print_content_data(string(1,data_[i]),position);
              }
              else{
                if(data_[i]=='\r') {
                  continue;
                }
                if(who_cmd){
                  if(data_[i]=='<' || data_[i]=='>'){
                    content += data_[i];
                    print_content_data(content,position);
                    content="";
                    continue;
                  }

                }
                
                content += data_[i];
                if(i == sz-1){
                  print_content_data(content,position);
                }
              }
          }
          if(content[0]!='%') do_read(l,true);
          else{
            string command = "";
            string w_cmd = "";
            for(auto s:lines[l]){
              if(s!='\r' && s!='\n') command+=s;
            }
            //int ssz = lines[l].size();
              //cout<<"<script>document.getElementById(\"s1\").innerHTML+= \"";
              
              //for(int i=0;i<ssz-2;i++) command += lines[l][i];
              w_cmd = command +"\n";
              command += "&NewLine;";
              print_title_data(command,position);
              strcpy(write_buf,w_cmd.c_str());
              do_write(w_cmd.size(),l);
          }
        }else{
          //cout<<ec.message()<<endl;
        }
      });
    }
    void do_write(std::size_t length,int l) {
      
    auto self(shared_from_this());
    boost::asio::async_write(
        socket_, boost::asio::buffer(write_buf, length),
        [this, self,l](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            do_read(l+1,false);
          }
          else{
            //cout<<ec.message()<<endl;
          }
        });
  }
    
    
  tcp::socket socket_;
  vector<string> lines;
  std::size_t length = 1000;
  enum { max_length = 10000 };
  char data_[max_length];
  char write_buf[max_length];
  int n_buf = 0;
  string cmdd;
  int position;
  string port="";
  string addr="";
  string text="";
  int now_line = 0;
};

class command
{
public:
  command(boost::asio::io_context &io_context,string str)
  :socket_(io_context),resolver(io_context),socks_resolver(io_context),socks_socket_(io_context)
  { 
    str_cmd = str;
    do_split();
  }
private:
int make_split(int n,string &str,string ss){
      int sz = ss.size();
      bool flag = false;
      while(ss[n]!='&' && n<sz){
        if(flag){
          str += ss[n];
        }
        if(ss[n]=='=') flag = true;
        n++;
      }
      return n+1;
    }
  
  bool valid(string addr,string port,string text)  
    {
      if(addr == "") return false;
      if(port == "") return false;
      if(text == "") return false;
      return true;
    }
    bool valid_socks(string addr,string port)
    {
        if(addr == "") return false;
        if(port == "") return false;
        return true;
    }
  void do_connect(string addr,string port,string text,int position){
      resolver.async_resolve(addr,port,[this,addr,port,text,position](boost::system::error_code err,
        tcp::resolver::iterator endpoint_it){
          if(!err){
            tcp::endpoint endpoint = *endpoint_it;
            socket_.async_connect(endpoint,[this,addr,port,text,position](boost::system::error_code err){
                if(!err && socket_.native_handle()!=-1){
                  std::make_shared<session>(std::move(socket_),addr,port,text,position)->start();
                  }
                else if(socket_.native_handle()==-1)
                {
                  socket_.close();
                  do_connect(addr,port,text,position);
                }
            });
          }else{
            cout<< err.message()<<endl;
          }
        }
      );
    }
    void do_connect_socks(string addr,string port,string text,int position)
    {
       
       tcp::resolver::iterator s_endpoint_it  = socks_resolver.resolve(socks_addr,socks_port);
       tcp::resolver::iterator end;
        boost::system::error_code err= boost::asio::error::host_not_found;
        tcp::endpoint endpoint;
        while(err && s_endpoint_it!=end){
            endpoint = *s_endpoint_it;
            socks_socket_.close();
            socks_socket_.connect(endpoint,err);
            s_endpoint_it++;
        }
        tcp::resolver::iterator endpoint_it  = resolver.resolve(addr,port);
        endpoint = *endpoint_it;
        status_ = 0x01;
        unsigned short sk_port =endpoint.port();
        port_high_byte_ = (sk_port>>8) & 0xff;
        port_low_byte_ = sk_port & 0xff;
        address_ = endpoint.address().to_v4().to_bytes();
        make_request();
        boost::asio::write(socks_socket_,boost::asio::buffer(request, sizeof(request)));
        socks_socket_.read_some(boost::asio::buffer(reply, sizeof(reply)));
        std::make_shared<socks_session>(std::move(socks_socket_),addr,port,text,position)->start();
    }
    void  make_request()
    {
        request[0] = version_;
        request[1] = status_;
        request[2] = port_high_byte_;
        request[3] = port_low_byte_;
        request[4] = address_[0];
        request[5] = address_[1];
        request[6] = address_[2];
        request[7] = address_[3];
        request[8] = 0;
        request[9] = 0;
    }
  void do_split(){
    int sz = str_cmd.size();
    for(int i=0;i<sz;i++){
      if(str_cmd[i]=='?') {
        int next = make_cmd(i+1,cmd1);
        next = make_cmd(next,cmd2);
        next = make_cmd(next,cmd3);
        next = make_cmd(next,cmd4);
        next = make_cmd(next,cmd5);
        make_socks_cmd(next,socks_cmd);
        break;
    }
    }
    do_split_socks(socks_cmd);
    do_split_addr(cmd1,1);
    do_split_addr(cmd2,2);
    do_split_addr(cmd3,3);
    do_split_addr(cmd4,4);
    do_split_addr(cmd5,5);
    
    
  }
  int make_socks_cmd(int n,string &cmd){
      int sz = str_cmd.size();
      while(n<sz){
          cmd += str_cmd[n];
          n++;
      }
      return 0;
  }
  int make_cmd(int n,string &cmd)
  {
    int count = 0;
    int sz = str_cmd.size();
    while(n<sz&&count<3){
      if(str_cmd[n]=='&'){
        count++;
        if(count==3) return n+1;
      }
      cmd += str_cmd[n];
      n++;
    }
    return 0;
  }
    void do_split_addr(string ss,int position){
      int flag = false;
      int sz = ss.size();
      string addr = "";
      string port = "";
      string text = "";
      for(int q=0 ; q<sz;q++){
        if(flag) continue;
        if(ss[q]=='='){
          int next = make_split(q,addr,ss);
          next = make_split(next,port,ss);
          next=make_split(next,text,ss);
          flag = true;
        }
      }
      if(valid(addr,port,text))
      {
        if(socks_mode) do_connect_socks(addr,port,text,position);
        else do_connect(addr,port,text,position);
      } 
    }
    void do_split_socks(string ss){
      int flag = false;
      int sz = ss.size();
      string addr = "";
      string port = "";
      for(int q=0 ; q<sz;q++){
        if(flag) continue;
        if(ss[q]=='='){
          int next = make_split(q,addr,ss);
          make_split(next,port,ss);
          flag = true;
        }
      }
      if(valid_socks(addr,port))
      {
          socks_addr = addr;
          socks_port = port;
            socks_mode = true;
      }
    }
  tcp::socket socket_;
  tcp::resolver resolver;
  tcp::resolver socks_resolver;
  tcp::socket socks_socket_;
  string str_cmd;
  string cmd1;
  string cmd2;
  string cmd3;
  string cmd4;
  string cmd5;
  string socks_cmd;
  string socks_addr;
  string socks_port;
  bool socks_mode = false;
  boost::asio::ip::address_v4::bytes_type address_;
  unsigned char request[10];
  unsigned char reply[8];
  const unsigned char version_ = 0x04;
  unsigned char null_byte_ =0;
  unsigned char user_id_ = 32;
  unsigned char status_;
  unsigned char port_high_byte_;
  unsigned char port_low_byte_;
  
};
int main(int, char* const[], char* const envp[]) {
  /* [Required] HTTP Header */
  cout << "Content-type: text/html" << endl << endl;
  boost::asio::io_context io_context;
  string str = getenv("REQUEST_URI");
  cout<<"<title>NP Project 3Console</title>"<<endl;
  print_html();
  command ccmd(io_context,str);


  io_context.run();
  return 0;
}