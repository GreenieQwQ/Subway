#ifndef GRAPH_CPP
#define GRAPH_CPP
#include "Graph.h"

void Graph::addAdjecnt(const string& nameA, const string& nameB,  weight w, const string& route)
{
    auto x = vertex_list.find(nameA);
    if(x==vertex_list.end())
    {
        Vertex temp;
        temp.name = nameA;
        //temp.road = roadA;
        temp.adjcentList.push_back(edge(nameB, route, w));
        vertex_list.insert(pair<string, Vertex>(nameA, temp));
        number_of_vertex++;
    }
    else
    {
        (*x).second.adjcentList.push_back(edge(nameB, route, w));
    }
    x = vertex_list.find(nameB);
    if(x==vertex_list.end())
    {
        Vertex temp;
        temp.name = nameB;
        //temp.road = roadB;
        temp.adjcentList.push_back(edge(nameA, route, w));
        vertex_list.insert(pair<string, Vertex>(nameB, temp));
        number_of_vertex++;
    }
    else
    {
        (*x).second.adjcentList.push_back(edge(nameA, route, w));
    }
}

weight Graph::print_path(const string& source, const string& destination)
{
    dijkstra(source, destination);
    stack<pair<string, string>> path;
    string temp = destination;
    while(temp != source)
    {
        path.push(vertex_list[temp].path);
        temp = path.top().first;
    }
    while(path.size()!=0)
    {
        cout<<"("<<path.top().first<<","<<path.top().second<<")"<<" -> ";
        path.pop();
    }
    cout<<"(des,"<<destination<<")"<<endl;
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

/*
初始化
*/
void inline Graph::initial()
{
    for(auto &x : vertex_list)
    {
        x.second.label =  false;
        x.second.distance = INFINITE;
        x.second.path.first.clear();
    }
}

/*
找最小的边
*/
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

/*
更新信息
*/
void inline Graph::updata_info(const string& min_name, int& count)
{
    //vertex_list[min_name];
    auto &y = vertex_list[min_name];
    for(auto x: y.adjcentList)
    {
        auto &z  = vertex_list[x.name];
        if(z.label == false)
        {
            if(y.distance + x.length < z.distance || z.distance == INFINITE)
            {
                string temp;
                z.distance = y.distance + x.length;
                /*for(auto road1: z.road)
                {
                    if(find(y.road.begin(), y.road.end(), road1) != y.road.end())
                        temp = road1;
                }*/
                temp = x.route;
                z.path = pair<string, string>(min_name, temp);
            }
            count++;
        }
    }
}
#endif
