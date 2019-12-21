#define DEBUG
#include "Subway.h"
#include <iostream>
#include <fstream>
using namespace std;

#define UNIX
#ifdef UNIX
#include <unistd.h>
#else
#include <windows.h>
#endif

void printFrontPadding()
{
    cout << "\n*********************\n";
}

void printEndPadding()
{
    cout << "*********************\n";
}

void printEnd()
{
    printFrontPadding();
    cout << "感谢您使用本查询系统，再见！\n";
    printEndPadding();
}

void printMissingStation()
{
    printFrontPadding();
    cout << "此站点不存在哦，请输入正确的站点。\n";
}


void printPathHead(const string& src, const string& dst)
{
    printFrontPadding();
    cout << "站点 '" << src << "' 到站点 '" << dst << "' 的路线为：\n"; 
}

string getSource()
{
    printFrontPadding();
    cout << "欢迎来到广州地铁线路查询系统\n"
         << "请输入出发的站点（输入quit退出本系统）：\n";
    cout << "==== ";
    string tmp; cin >> tmp;
    return tmp;
}

string getDst()
{
    printFrontPadding();
    cout << "请输入目的地站点（输入quit退出本系统，输入return返回出发站点的输入）：\n"
         << "==== ";
    string tmp; cin >> tmp;
    return tmp;
}

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