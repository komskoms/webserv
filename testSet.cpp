// confirm set container for nginx config parsing 
#include<set>
#include<string>
#include<vector>
#include<utility>
#include<iostream>
#include<iomanip>

using namespace std;
using pss = pair<string, string>; // c++98 warning

class test {
    public:
        test();
        test(string a, string b, pss c) : _ip(a), _port(b), v(c) {}
        ~test() {}
        string getip() const {return this->_ip;}
        string getport() const {return this->_port;}
        pss getpss() const {return this->v;}
    private:
        string _ip;
        string _port;
        pss v;
};

struct checkAddr
{
    bool operator()(const test& lhs, const test& rhs)
    {
        return ((lhs.getip() < rhs.getip()) || (lhs.getport() < rhs.getport()));
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    set<test, checkAddr> t;

    vector<pss> ex;
    ex.push_back(make_pair("hello1", "world1"));
    ex.push_back(make_pair("hello1", "world2"));
    ex.push_back(make_pair("hello1", "world1"));
    ex.push_back(make_pair("hello1", "world1"));
    ex.push_back(make_pair("42", "mosong~~"));
    
    test a("127.0.0.1", "80", ex[0]);
    test b("127.0.0.2", "80", ex[1]); // ip만 다름
    test c("127.0.0.1", "81", ex[2]); // port만 다름
    test d("127.0.0.1", "80", ex[3]); // a와 완전 동일(내용도)
    test e("127.0.0.1", "80", ex[4]); // ip, port만 같음 -
    
    t.insert(a);
    t.insert(b);
    t.insert(c);
    t.insert(d);
    t.insert(e); // 적용 안됨(nginx config에서 동일 ip, port (+server_name)가 존재하면 먼저 설정된 서버로 유지)
    
    for (set<test>::iterator itr = t.begin(); itr != t.end(); ++itr)
    {
        cout << setw(15) << "ip : " <<  itr->getip()
            << setw(15) << "port : " << itr->getport()
            << setw(15) << "field-key : " << itr->getpss().first
            << setw(15) << "field-value : " << itr->getpss().second << endl;
    }
    cout << endl;
    return 0;
}