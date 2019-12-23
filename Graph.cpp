#ifndef GRAPH_CPP
#define GRAPH_CPP
#include "Graph.h"

void Graph::eraseVertex(const string& name)
{
    auto x = vertex_list.find(name);
    if(x != vertex_list.end())  
        vertex_list.erase(x);   
}

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
    bool mark = true;
    weight return_1 = test_srcdes(source, destination, mark);
    if(!mark)
        return return_1;
    dijkstra(source, destination);
    return print_pathin(source, destination);
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
        x.second.path.second.clear();
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
                z.distance = y.distance + x.length;
                /*for(auto road1: z.road)
                {
                    if(find(y.road.begin(), y.road.end(), road1) != y.road.end())
                        temp = road1;
                }*/
                z.path = pair<string, string>(min_name, x.route);
            }
            else if(y.distance + x.length == z.distance)
            {
                if(x.route == y.path.second)
                    z.path.second = x.route;
            }
            count++;
        }
    }
}

weight inline Graph::test_srcdes(const string& source, const string& destination, bool & mark)
{
    auto s1 = vertex_list.find(source);
    auto s2 = vertex_list.find(destination);
    if(s1 == s2 && s1==vertex_list.end())
    {
        cout<<"Both the source and destination do not exit."<<endl;
        mark = false;
        return WRONG_VERTEX;
    }
    if(s1==vertex_list.end())
    {
        cout<<"The source does not exit."<<endl;
        mark = false;
        return WRONG_VERTEX;
    }
    if(s2==vertex_list.end())
    {
        cout<<"The destination does not exit."<<endl;
        mark = false;
        return WRONG_VERTEX;
    }
    if(source == destination)
    {
        cout<< "Where are you going?"<<endl;
        mark = false;
        return WEIGHT_ZEOR;
    }
    if(vertex_list[destination].distance == INFINITE)
    {
        cout<< "No way"<<endl;
        mark = false;
        return INFINITE;
    }
}

weight inline Graph::print_pathin(const string& source, const string& destination)
{
    vector<pair<string, string>> path;
    string temp = destination;
    while(temp != source)
    {
        path.push_back(vertex_list[temp].path);
        temp = path.back().first;
    }
    /*for(auto x:path)
        cout<<x.second<<endl;*/
    bool out = true;
    cout<<"(src,"<<source<<")"<<" -> ";
    temp = path.back().second;
    int i = path.size()-1;
    while(i>0)
    {
        out = true;
        for(int j=0; j<i; j++)
        {
            if(path[j].second == temp)
            {
                i = j-1;
                out = false;
                if(j)
                {
                    cout<<"("<<temp<<","<<path[i].first<<")"<<" -> ";
                    temp = path[i].second;
                }
                break;
            }
        }
        if(out)
        {
            cout<<"("<<temp<<","<<path[i].first<<")"<<" -> ";
            i--;
            temp = path[i].second;
        }
    }
    cout<<"("<<temp<<","<<destination<<")"<<endl;
    return vertex_list[destination].distance;
}
#endif
