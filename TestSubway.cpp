#define DEBUG
#include "Subway.h"
#include <iostream>
#include <fstream>
using namespace std;

//单元测试函数
#define UNIX
#ifdef UNIX
#include <unistd.h>
#else
#include <windows.h>
#endif

enum STATE { CLEAR, DATA_ERROR };

int main()
{
    //初始化
    ifstream in("data.txt", ios::in); //读取数据
    if(!in)
    {
        cerr << "初始化失败，找不到地铁数据文件data.txt\n";
        return DATA_ERROR;
    }
    Subway s(in);
    in.close(); //关闭文件

    string src, dst;
    src = "V1";
    dst = "V6";

    cout << "Axis count:\n";
    for(int i = 1; i <= 7; ++i)
    {
        string v = "V"; v += '0' + i;
        cout << v << ": "  << s.axis.count(v) << endl;
    }

    cout << "\nFind pos:\n";
    for(int i = 1; i <= 7; ++i)
    {
        string v = "V"; v += '0' + i;
        string r = s.whichRoute[v].at(0);
        cout << "Finding " << v << " in line '"
            << r << "' at " << s.findStation(s.route[r], v) << endl;
    }

    cout << "\nPrint path:\n";
    for(int i = 1; i <= 7; ++i)
    {
        string v = "V"; v += '0' + i;
        auto r = s.whichRoute[v];
        cout << "Route of " << v << "'s: ";
        for(auto& str : r)
            cout << str << " ";
        cout << endl;
    }

}