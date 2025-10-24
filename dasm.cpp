#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <optional>
#include <cctype>
#include <algorithm>

using namespace std;

static long long lbl_n = 0;
static int lnum = 0;
static vector<string> out_thing, file_lines;
struct Bx { enum T { I, E, W } t; int d; string le, lend, lh; };
static vector<Bx> stk;

static string trimx(const string&s){size_t a=0,b=s.size();while(a<b&&isspace((unsigned char)s[a]))++a;while(b>a&&isspace((unsigned char)s[b-1]))--b;return s.substr(a,b-a);}
static string lowerme(string s){for(char&c:s)c=(char)tolower((unsigned char)c);return s;}
static string make_lbl(const string&p){return "."+p+"_"+to_string(lbl_n++);}
static bool isnum2(const string&s){if(s.empty())return 0;if(s.size()>2&&s[0]=='0'&&(s[1]=='x'||s[1]=='X')){for(size_t i=2;i<s.size();++i)if(!isxdigit((unsigned char)s[i]))return 0;return 1;}for(char c:s)if(!isdigit((unsigned char)c))return 0;return 1;}
static long long numparse(const string&s){return (s.size()>2&&s[0]=='0'&&(s[1]=='x'||s[1]=='X'))?stoll(s,nullptr,16):stoll(s,nullptr,10);}
static bool isimm2(const string&s){if(isnum2(s))return 1;if(s.size()>=2&&s.front()=='\''&&s.back()=='\'')return 1;return 0;}

struct cond3 { string a,b,c; };
static optional<cond3> parse_if(const string&s){static const vector<string>ops={"==","!=",">=","<=","UGT","UGE","ULT","ULE",">","<"};for(auto&o:ops){size_t p=s.find(o);if(p!=string::npos){string l=trimx(s.substr(0,p)),r=trimx(s.substr(p+o.size()));if(l.empty()||r.empty())return nullopt;return cond3{l,o,r};}}return nullopt;}
static bool constcmp(const string&a,const string&o,const string&b,bool&v){if(!isnum2(a)||!isnum2(b))return 0;long long x=numparse(a),y=numparse(b);
if(o=="=="){v=x==y;return 1;}if(o=="!="){v=x!=y;return 1;}if(o==">"){v=x>y;return 1;}if(o=="<"){v=x<y;return 1;}if(o==">="){v=x>=y;return 1;}if(o=="<="){v=x<=y;return 1;}
if(o=="UGT"){v=(unsigned long long)x>(unsigned long long)y;return 1;}if(o=="UGE"){v=(unsigned long long)x>=(unsigned long long)y;return 1;}
if(o=="ULT"){v=(unsigned long long)x<(unsigned long long)y;return 1;}if(o=="ULE"){v=(unsigned long long)x<=(unsigned long long)y;return 1;}return 0;}
static string jmpfalse(const string&o,bool u){if(!u){if(o=="==")return"jne";if(o=="!=")return"je";if(o==">")return"jle";if(o=="<")return"jge";if(o==">=")return"jl";if(o=="<=")return"jg";}
else{if(o=="UGT")return"jbe";if(o=="UGE")return"jb";if(o=="ULT")return"jae";if(o=="ULE")return"ja";}return"jne";}

static void emitx(const string&s){out_thing.push_back(s);}
static void close_blk(const Bx&b){if(b.t==Bx::I){emitx(b.le+":");emitx(b.lend+":");}else if(b.t==Bx::E){emitx(b.lend+":");}else{emitx(string("jmp ")+b.lh);emitx(b.lend+":");}}
static void close_til(int d){while(!stk.empty()&&stk.back().d>=d){auto b=stk.back();stk.pop_back();close_blk(b);}}
static bool readfilex(const string&p,vector<string>&v){ifstream f(p);if(!f)return 0;string s;while(getline(f,s))v.push_back(s);return 1;}

int main(int argc,char**argv){
    ios::sync_with_stdio(0);cin.tie(0);
    if(argc<2){cerr<<"no input\n";return 1;}
    string fname=argv[1];
    if(!readfilex(fname,file_lines)){cerr<<"cant open\n";return 2;}
    for(lnum=0;lnum<(int)file_lines.size();++lnum){
        string raw=file_lines[lnum], line=raw;
        int i=0,dep=0;while(i<(int)line.size()){if(line[i]=='&'){++dep;++i;while(i<(int)line.size()&&line[i]==' ')++i;}else if(line[i]==' ')++i;else break;}
        string body=trimx(line.substr(i));
        close_til(dep+1);
        if(body.empty()||body[0]==';'){emitx(raw);continue;}
        size_t sp=body.find_first_of(" \t");
        string head=lowerme(sp==string::npos?body:body.substr(0,sp));
        string rest=trimx(body.substr(head.size()));

        if(head=="if"||head=="elif"||head=="else"||head=="while"||head=="end"){
            int dd=dep;
            if(head=="if"){
                auto c=parse_if(rest);if(!c){cerr<<"bad if\n";return 3;}
                string L1=make_lbl("if_else"),L2=make_lbl("if_end");bool v=0;
                if(constcmp(c->a,c->b,c->c,v)){if(!v)emitx("jmp "+L1);stk.push_back(Bx{Bx::I,dd,L1,L2,""});continue;}
                if(isimm2(c->a)){cerr<<"imm lhs\n";return 4;}
                emitx("cmp "+lowerme(c->a)+", "+c->c);
                emitx(jmpfalse(c->b,(c->b=="UGT"||c->b=="UGE"||c->b=="ULT"||c->b=="ULE"))+" "+L1);
                stk.push_back(Bx{Bx::I,dd,L1,L2,""});continue;
            }
            if(head=="elif"){
                if(stk.empty()||stk.back().t!=Bx::I||stk.back().d!=dd){cerr<<"elif bad\n";return 5;}
                auto prev=stk.back();stk.pop_back();
                emitx("jmp "+prev.lend);emitx(prev.le+":");
                auto c=parse_if(rest);if(!c){cerr<<"bad elif\n";return 6;}
                string L1=make_lbl("if_else");bool v=0;
                if(constcmp(c->a,c->b,c->c,v)){if(!v)emitx("jmp "+L1);stk.push_back(Bx{Bx::I,dd,L1,prev.lend,""});continue;}
                if(isimm2(c->a)){cerr<<"imm lhs\n";return 7;}
                emitx("cmp "+lowerme(c->a)+", "+c->c);
                emitx(jmpfalse(c->b,(c->b=="UGT"||c->b=="UGE"||c->b=="ULT"||c->b=="ULE"))+" "+L1);
                stk.push_back(Bx{Bx::I,dd,L1,prev.lend,""});continue;
            }
            if(head=="else"){
                if(stk.empty()||stk.back().t!=Bx::I||stk.back().d!=dd){cerr<<"else bad\n";return 8;}
                auto prev=stk.back();stk.pop_back();
                emitx("jmp "+prev.lend);emitx(prev.le+":");
                stk.push_back(Bx{Bx::E,dd,"",prev.lend,""});continue;
            }
            if(head=="while"){
                auto c=parse_if(rest);if(!c){cerr<<"bad while\n";return 9;}
                string Lh=make_lbl("while_head"),Le=make_lbl("while_end");bool v=0;emitx(Lh+":");
                if(constcmp(c->a,c->b,c->c,v)){if(v)stk.push_back(Bx{Bx::W,dd,"",Le,Lh});else emitx(Le+":");continue;}
                if(isimm2(c->a)){cerr<<"imm lhs\n";return 10;}
                emitx("cmp "+lowerme(c->a)+", "+c->c);
                emitx(jmpfalse(c->b,(c->b=="UGT"||c->b=="UGE"||c->b=="ULT"||c->b=="ULE"))+" "+Le);
                stk.push_back(Bx{Bx::W,dd,"",Le,Lh});continue;
            }
            if(head=="end"){
                if(stk.empty()||stk.back().d!=dd){cerr<<"end bad\n";return 11;}
                auto b=stk.back();stk.pop_back();close_blk(b);continue;
            }
        }
        emitx(body);
    }
    close_til(0);
    for(auto&s:out_thing)cout<<s<<"\n";
    if(argc>=2){string of=fname;size_t d=of.find_last_of('.');if(d==string::npos)of+=".asm";else of=of.substr(0,d)+".asm";ofstream f(of);for(auto&s:out_thing)f<<s<<"\n";}
}
