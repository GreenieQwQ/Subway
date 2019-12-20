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
    const size_t INF = -1; //代表距离无穷
    struct station
    {
        string name;
        size_t distance; //到下一站的距离
        station(const string& n, size_t d): name(n), distance(d) {}
    };
    map<string, vector<station>> route; //存储线路们    
    map<string, vector<string>> whichRoute; //通过名字即可知道站点属于哪种线路 
    set<string> axis; //交点的集合
    Graph model; //存储原图——仅包含交点

public:
    Subway(){ initialize(); }
    void printPath(const string& source, const string& destination) const;

#ifndef DEBUG
private:
#else
public:
#endif
    void initialize(); //初始化
    void readData(istream& in = cin); //读取线路数据
    void generateGraph(); //生成原图 存入model中
    void generateNode(const string& name, Graph& g) const; //根据名字向图g加入新点 
    size_t findStation(const vector<station>& v, const string& name) const; //根据名字找到station在v中对应的index 找不到返回-1
};

void Subway::initialize()
{
    readData();
    generateGraph();
}

void Subway::readData(istream& in)
{
    string r, n;
    size_t dist;
    while(in >> r >> n >> dist)
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
        size_t length = 0; //记录路经长
        string firstAxis; vector<string> firstRoute;
        bool first = true; //第二个交点才连接
        for(const auto& s: r.second) //对线路的每一车站 记录交点和交点之间形成的边
        {
            if(axis.count(s.name)) //若为交点
            {
                if(first) //第一次则记录下交点
                {
                    first = false;
                    firstAxis = s.name; 
                    firstRoute = whichRoute[s.name];
                }
                else //第二/n次则向图中增加邻边
                    model.addAdjecnt(firstAxis, firstRoute, s.name, whichRoute[s.name], length);

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

void Subway::generateNode(const string& name, Graph& g) const
{
    if(!axis.count(name)) //若不为交点 才需加入图中
    {
        const auto it = whichRoute.find(name);
        if(it == whichRoute.end())
            throw invalid_argument(name); //不存在这个站点
        vector<string> r = (*it).second; //获取对应路线---其实r肯定只有一个元素

        const auto itr = route.find(r[0]);
        if(itr == route.end())
            throw runtime_error("Missing route.");
        const vector<station>& theRoute = (*itr).second;

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
                g.addAdjecnt(name, r, stationName, (*whichRoute.find(stationName)).second, length);
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
                g.addAdjecnt(name, r, stationName, (*whichRoute.find(stationName)).second, length);
                break; //只需要记录后一个交点即可
            }
            length += distance; //在可能的记录后才更新length
        }
    }
}

void Subway::printPath(const string& source, const string& destination) const
{
    Graph newGraph = model; // 拷贝模板图
    try
    {
        generateNode(source, newGraph);
        generateNode(destination, newGraph);
    }
    catch(invalid_argument& e)
    {
        cerr << "No such station '" << e.what() << "' exists." << endl;
        return;
    }
    newGraph.dijkstra(source, destination);
    newGraph.print_path();
}

#endif