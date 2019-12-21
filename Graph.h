#ifndef GRAPH_H
#define GRAPH_h

#include <iostream>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>

#define INFINITE -1

using namespace std;

typedef int weight;
typedef pair<string, weight> edge;

class Vertex
{
    public:
    string name;
    vector<string> road; //所属线路
    list<edge> adjcentList;
    bool label;
    weight distance;
    pair<string, string> path;
};

class Graph
{
public:
    Graph() {}
    void addAdjecnt(const string& nameA, const vector<string>& roadA, const string& nameB, const vector<string>& roadB, weight w); //增加邻边 若没有点a/b则创建点a/b
    weight print_path(const string& source, const string& destination); //打印路径 (road,name) -> (road,name) -> ....
    //void dijkstra(const string& source, const string& destination);    
private:
    int number_of_vertex;
    map<string, Vertex> vertex_list;
private:
    void dijkstra(const string& source, const string& destination);  
    void inline initial();
    void inline findsmallest(string& min_name);
    void inline updata_info(const string& min_name, int& count);
};

/*void Graph::addAdjecnt(const string& nameA, const vector<string>& roadA, const string& nameB, const vector<string>& roadB, weight w)
{
    auto x = vertex_list.find(nameA);
    if(x==vertex_list.end())
    {
        Vertex temp;
        temp.name = nameA;
        temp.road = roadA;
        temp.adjcentList.push_back(edge(nameB, w));
        vertex_list.insert(pair<string, Vertex>(nameA, temp));
        number_of_vertex++;
    }
    else
    {
        (*x).second.adjcentList.push_back(edge(nameB, w));
    }
    x = vertex_list.find(nameB);
    if(x==vertex_list.end())
    {
        Vertex temp;
        temp.name = nameB;
        temp.road = roadB;
        temp.adjcentList.push_back(edge(nameA, w));
        vertex_list.insert(pair<string, Vertex>(nameB, temp));
        number_of_vertex++;
    }
    else
    {
        (*x).second.adjcentList.push_back(edge(nameA, w));
    }
}

weight Graph::print_path(const string& source, const string& destination)
{
    dijkstra(source, destination);
    stack<pair<string, string>> path;
    string temp = destination;
    string temp1;
    while(temp != source)
    {
        path.push(vertex_list[temp].path);
        temp = path.top().first;
    }
    while(path.size()!=0)
    {
        cout<<"("<<path.top().second<<","<<path.top().first<<")"<<" -> ";
        path.pop();
        if(path.size()==1)
            temp1 = path.top().second;
    }
    cout<<"("<<temp1<<","<<destination<<")"<<endl;
    return vertex_list[destination].distance;
}

void Graph::dijkstra(const string& source, const string& destination)
{
    int count = 1;//记录点的数目;
    initial();
    vertex_list[source].distance = 0;
    while(count != 0)
    {
        string min_name;
        findsmallest(min_name);
        if(min_name.size()==0)
        {
            count--;
            continue;
        }
        vertex_list[min_name].label = true;
        updata_info(min_name, count);
    }
}


初始化

void inline Graph::initial()
{
    for(auto &x : vertex_list)
    {
        x.second.label =  false;
        x.second.distance = INFINITE;
        x.second.path.first.clear();
    }
}


找最小的边

void inline Graph::findsmallest(string & temp)
{
    weight min = INFINITE;
    for(auto x: vertex_list)
    {
        if(x.second.distance != INFINITE && (min > x.second.distance || min==INFINITE)&& x.second.label==false)
        {
            temp = x.first;
        }
    }
}


更新信息

void inline Graph::updata_info(const string& min_name, int& count)
{
    //vertex_list[min_name];
    auto &y = vertex_list[min_name];
    for(auto x: y.adjcentList)
    {
        auto &z  = vertex_list[x.first];
        if(z.label == false)
        {
            if(y.distance + x.second < z.distance || z.distance == INFINITE)
            {
                string temp;
                z.distance = y.distance + x.second;
                for(auto road1: z.road)
                {
                    if(find(y.road.begin(), y.road.end(), road1) != y.road.end())
                        temp = road1;
                }
                z.path = pair<string, string>(min_name, temp);
            }
            count++;
        }
    }
}*/


#endif