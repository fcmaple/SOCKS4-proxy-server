#include <iostream>
#include <string>
#include "printHTML.h"
using namespace std;

void print_content_data(string ch, int position){
  cout<<"<script>document.getElementById(\"s"<<position<<"\").innerHTML+=\""<<ch<<"\"</script>\n"<<flush;
}
void print_title_data(string ch, int position){
  cout<<"<script>document.getElementById(\"s"<<position<<"\").innerHTML+=\"<b>"<<ch<<"</b>\"</script>\n"<<flush;
}
void print_content(int position)
{
  cout<<"<script>document.getElementById(\"s"<<position<<"\").innerHTML+= \""<<flush;
}
void print_html()
{
    cout<<"<!DOCTYPE html>\n";
    cout<<"<html lang=\"en\">\n";
  cout<<"<head>\n";
    cout<<"<meta charset=\"UTF-8\" />\n";
    cout<<"<title>NP Project 3 Sample Console</title>\n";
    cout<<"<link\n";
      cout<<"rel=\"stylesheet\"\n";
      cout<<"href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css\"\n";
      cout<<"integrity=\"sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\"\n";
      cout<<"crossorigin=\"anonymous\"\n";
    cout<<"/>\n";
    cout<<"<link\n";
      cout<<"href=\"https://fonts.googleapis.com/css?family=Source+Code+Pro\"\n";
      cout<<"rel=\"stylesheet\"\n";
    cout<<"/>\n";
    cout<<"<link\n";
      cout<<"rel=\"icon\"\n";
      cout<<"type=\"image/png\"\n";
      cout<<"href=\"https://cdn0.iconfinder.com/data/icons/small-n-flat/24/678068-terminal-512.png\"\n";
    cout<<"/>\n";
    cout<<"<style>\n";
      cout<<"* {\n";
       cout<<"font-family: 'Source Code Pro', monospace;\n";
        cout<<"font-size: 1rem !important;\n";
      cout<<"}\n";
      cout<<"body {\n";
        cout<<"background-color: #212529;\n";
      cout<<"}\n";
      cout<<"pre {";
        cout<<"color: #cccccc;\n";
      cout<<"}\n";
      cout<<"b {\n";
        cout<<"color: #01b468;\n";
      cout<<"}\n";
    cout<<"</style>\n";
  cout<<"</head>\n";
  cout<<"<body>\n";
    cout<<"<table border = 3 class=\"table table-dark table-bordered\">\n";
      cout<<"<thead>\n";
        cout<<"<tr>\n";
          cout<<"<th width = 20%><pre id = \"s6\" class=\"mb-0\"></pre></th>\n";
          cout<<"<th width = 20%><pre id = \"s7\" class=\"mb-0\"></pre></th>\n";
          cout<<"<th width = 20%><pre id = \"s8\" class=\"mb-0\"></pre></th>\n";
          cout<<"<th width = 20%><pre id = \"s9\" class=\"mb-0\"></pre></th>\n";
          cout<<"<th width = 20%><pre id = \"s10\" class=\"mb-0\"></pre></th>\n";
        cout<<"</tr>\n";
      cout<<"</thead>\n";
      cout<<"<tbody>\n";
        cout<<"<tr>\n";
          cout<<"<td width = 20%><pre id=\"s1\" class=\"mb-0\"></pre></td>\n";
          cout<<"<td width = 20%><pre id=\"s2\" class=\"mb-0\"></pre></td>\n";
          cout<<"<td width = 20%><pre id=\"s3\" class=\"mb-0\"></pre></td>\n";
          cout<<"<td width = 20%><pre id=\"s4\" class=\"mb-0\"></pre></td>\n";
          cout<<"<td width = 20%><pre id=\"s5\" class=\"mb-0\"></pre></td>\n";
        cout<<"</tr>\n";
      cout<<"</tbody\n";
    cout<<"</table>\n";
  cout<<"</body>\n";
  cout<<"</html>\n";
}