#ifndef GRAPH_H
#define GRAPH_h

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#define INFINITE -1
#define WEIGHT_ZEOR 0
#define WRONG_VERTEX -2
//#define Transfer_time 10000
//#define real_time 5

using namespace std;

typedef size_t weight;

struct edge
{
    string name;
    vector<string> route;
    string direction;
    weight length;
    edge(const string& name1, const weight& length1, const string& direction1):name(name1),
    length(length1),direction(direction1){}
    edge(){}
};

class Path
{
    public:
    string fathername;//该点对应的最短路径的父亲（假设起点为始祖）
    vector<string> route;//两点之间的线路数目
    string fathertome;//从父亲点到此站点的方向
    Path(const string& fathername1, const string& fathertome1):
    fathername(fathername1),fathertome(fathertome1){}//初始化函数
    Path(){}
};

class Vertex
{
    public:
    string name;//站点名
    map<string, edge> adjcentList;//邻接表
    bool label;//是否已找到最短路径标记
    weight distance;//最短路径长度
    Path path;//路径
};

class Graph
{
public:
    Graph() {number_of_vertex = 0;}
    void addAdjecnt(const string& nameA, const string& nameB,  weight w, const string& route, 
                   const string& A2B, const string& B2A); 
    /**
     * 增加邻边 若没有点a/b则创建点a/b
     * A2B为A到B的方向
     */
    void eraseVertex(const string& name); //删除名字为name的站点
    weight print_path(const string& source, const string& destination);
     /**
      * 打印实际最短路径，格式：(起点,src) -> (线路,中间站点) -> (线路, dst) (坐哪条线路, 到哪个站)  返回路径的长度
      */
private:
    int number_of_vertex;
    map<string, Vertex> vertex_list;
private:
    void dijkstra(const string& source, const string& destination);//计算最短路径
    void inline initial();//计算最短路径初始化
    void inline findsmallest(string& min_name);//找最小的权值的边
    void inline updata_info(const string& min_name, int& count);//更新信息
    weight inline test_srcdes(const string& source, const string& destination, bool & mark);//增加健壮性
    weight inline print_pathin(const string& source, const string& destination);//打印路径
    string inline initial_route(Path& a, Path& b);//找到相邻的边的公共线，没有则返回size为0的字符串
};

#endif
