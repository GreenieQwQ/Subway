#ifndef GRAPH_H
#define GRAPH_h

#include <iostream>
#include <map>
#include <list>
#include <string>
using namespace std;

typedef int weight;
typedef pair<string, weight> edge;

class Vertex
{
    string name;
    string road; //所属线路
    list<edge> adjcentList;
};

class Graph
{
public:
    Graph() {}
    void addAdjecnt(string a, string b, weight w); //增加邻边 若没有点a/b则创建点a/b
    void print_path(); //打印路径 (road,name) -> (road,name) -> ....
    void dijkstra(string source, string destination);    
private:
    int number_of_vertex;
    map<string, Vertex> vertex_list;
};


#endif