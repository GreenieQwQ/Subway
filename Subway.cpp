#include "Subway.h"
#include <iostream>
#include <fstream>
using namespace std;

#define UNIX

void printPadding()
{
    cout << "*********************\n";
}

void printEnd()
{
    printPadding();
    cout << "感谢您使用本查询系统，再见！\n";
    printPadding();
}

void printMissingStation()
{
    printPadding();
    cout << "此站点不存在哦，请输入正确的站点。\n";
}


void printPathHead(const string& src, const string& dst)
{
    printPadding();
    cout << "站点 '" << src << "' 到站点' " << "' 的路线为：\n"; 
}

string getSource()
{
    printPadding();
    cout << "欢迎来到广州地铁线路查询系统\n"
         << "请输入出发的站点（输入Quit退出本系统）：\n";
    cout << "==== ";
    string tmp; cin >> tmp;
    return tmp;
}

string getDst()
{
    printPadding();
    cout << "请输入目的地站点（输入Quit退出本系统，输入Return返回出发站点的输入）：\n"
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
    while( ( ( src = getSource() ) != "Quit") && !src.empty() ) //empty代表读入失败
    {
        if( !s.contains(src) )//不含有此站点
        {
            printMissingStation();
            //s.matchSimilar(src) 为顾客匹配名字可能相符的站点
            continue;
        }

        while( ( ( dst = getSource() ) != "Quit") && dst != "Return" && !dst.empty() )
        {
            if(!s.contains(dst))
            {
                printMissingStation();
                //s.findsimilar(dst)
                continue;
            }
        }

        if(dst == "Quit")
            break;
        else if(dst == "Return")
            continue;
        
        printPathHead(src, dst);
        s.printPath(src, dst);
        printPadding();
        system("pause");
    #ifdef UNIX
        system("clear");
    #else
        system("cls");
    #endif
    }

    printEnd();
}