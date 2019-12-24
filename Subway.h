#ifndef SUBWAY_H
#define SUBWAY_H

#include "Graph.h"
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

class Subway
{
#ifndef DEBUG
private:
#else
public:
#endif
    // const size_t INF = -1; //代表距离无穷
    struct station
    {
        string name;
        size_t distance; //到下一站的距离
        station(const string& n, size_t d): name(n), distance(d) {}
    };
    map<string, vector<station>> route; //存储线路数据   
    map<string, vector<string>> whichRoute; //通过名字即可知道站点属于哪种线路 
    set<string> axis; //交点的集合
    mutable Graph model; //存储模型图——仅包含交点 在查询函数需要对模型图进行修改

public:
    Subway(){}
    Subway(istream& in){ initialize(in); }
    void initialize(istream& in = cin); //初始化
    void printPath(const string& source, const string& destination) const;
    bool contains(const string& name) const; //查询是否含有此条站点

#ifndef DEBUG
private:
#else
public:
#endif
    void readData(istream& in = cin); //读取线路数据
    void generateGraph(); //生成原图 存入model中
    bool generateNode(const string& name, Graph& g) const; //根据名字向图g加入新点 
    size_t findStation(const vector<station>& v, const string& name) const; //根据名字找到station在v中对应的index 找不到返回-1
};

void Subway::initialize(istream& in)
{
    readData(in);
    generateGraph();
}

void Subway::readData(istream& in)
{
    string r, n;
    size_t dist;
    while(in >> r && r != "Quit" && in >> n >> dist) //标准输入流输入Quit结束
    {
        route[r].push_back(station(n,dist));
        vector<string>& theRoute = whichRoute[n];
        theRoute.push_back(r);
        if(theRoute.size() == 2) //存在于两条线以上
            axis.insert(n); //加入交点集
    }
}

void Subway::generateGraph()
{
    for(const auto& r :route) //对每一条线路
    {
        const string start = r.second.at(0).name + "方向"; //起点站方向
        const string end = r.second.at(r.second.size()-1).name + "方向"; //终点站方向
        
        size_t length = 0; //记录路经长
        string Axis; //记录交点
        bool first = true; //第二个交点才连接
        for(const auto& s: r.second) //对线路的每一车站 记录交点和交点之间形成的边
        {
            if(axis.count(s.name)) //若为交点
            {
                if(first) //第一次则记录下交点
                {
                    first = false;
                    Axis = s.name; 
                }
                else //第二/n次则向图中增加邻边 更新firstAxis
                {
                    model.addAdjecnt(Axis, s.name, length, r.first, end, start); //增加邻边——属于这条线，指向终点站
                    Axis = s.name;
                }    

                length = s.distance; //从交点开始记录长度
            }
            else
                length += s.distance;
        }
    }
}

size_t Subway::findStation(const vector<station>& v, const string& name) const
{
    for(size_t i = 0; i < v.size(); ++i)
    {
        if(name == v[i].name)
            return i;
    }
    return -1;
}

bool Subway::generateNode(const string& name, Graph& g) const
{
    if(!axis.count(name)) //若不为交点 才需加入图中
    {
        const auto it = whichRoute.find(name);
        if(it == whichRoute.end())
            throw invalid_argument(name); //不存在这个站点
        vector<string> r = (*it).second; //获取对应路线---其实r肯定只有一个元素

        const auto itr = route.find(r[0]); //r[0]为线路名字
        if(itr == route.end())
            throw runtime_error("Missing route.");
        const vector<station>& theRoute = (*itr).second;

        const string start = theRoute[0].name + "方向"; //起点站方向
        const string end = theRoute[theRoute.size()-1].name + "方向"; //终点站方向

        size_t pos = findStation(theRoute, name);
        if(pos == -1)
            throw runtime_error("Missing station.");
        
        //向前找到第一个交点
        size_t length = 0; //记录距离
        for(int i = pos - 1; i >= 0; --i)
        {
            const string& stationName = theRoute[i].name;
            const size_t distance = theRoute[i].distance;
            length += distance; //在可能的记录前更新length
            if(axis.count(stationName)) //为交点
            {
                g.addAdjecnt(name, stationName, length, r[0], start, end); //指向起点
                break; //只需要记录前一个交点即可
            }
        }
        
        //向后找第一个交点 
        length = theRoute[pos].distance;
        for(int i = pos + 1; i < theRoute.size(); ++i)
        {
            const string& stationName = theRoute[i].name;
            const size_t distance = theRoute[i].distance;
            if(axis.count(stationName)) //为交点
            {
                g.addAdjecnt(name, stationName, length, r[0], end, start); //指向终点站
                break; //只需要记录后一个交点即可
            }
            length += distance; //在可能的记录后才更新length
        }
        return true; //向模板图增加点
    }

    return false; //不向模板图增加点
}

void Subway::printPath(const string& source, const string& destination) const
{
    bool isAddsrc = false;
    bool isAddDst = false;

    try
    {
        isAddsrc = generateNode(source, model);
        isAddDst = generateNode(destination, model);
    }
    catch(invalid_argument& e)
    {
        cerr << "No such station '" << e.what() << "' exists." << endl;
        return;
    }
    model.print_path(source, destination);

    if(isAddsrc)
        model.eraseVertex(source); //把添加的点从模板图删除
    
    if(isAddDst)
        model.eraseVertex(destination); 
}

bool Subway::contains(const string& name) const
{
    auto it = whichRoute.find(name);
    return !(it == whichRoute.end());
}

#endif