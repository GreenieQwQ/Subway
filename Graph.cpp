#ifndef GRAPH_CPP
#define GRAPH_CPP
#include "Graph.h"

void Graph::eraseVertex(const string& name)
{
    if(vertex_list.find(name)!=vertex_list.end())
        vertex_list.erase(name);
    for(auto &x: vertex_list)
    {
        if(x.second.adjcentList.find(name)!=x.second.adjcentList.end())
            x.second.adjcentList.erase(name);
    }
    /*auto x = vertex_list.find(name);
    for(auto y: (*x).second.adjcentList)
    {
        list<edge>& temp = vertex_list[y.first].adjcentList;
        //vector<list<edge>::iterator> temp1;
        for(auto itr = temp.begin(); itr!= temp.end(); itr++)
        {
            if((*itr).name == name)
                temp.erase(itr);
        }
    }
    if(x != vertex_list.end())
        vertex_list.erase(x);
    number_of_vertex--;*/
}

void Graph::addAdjecnt(const string& nameA, const string& nameB,  weight w, const string& route,
                       const string& A2B, const string& B2A)
{
    auto x = vertex_list.find(nameA);
    if(x==vertex_list.end())
    {
        Vertex temp;
        temp.name = nameA;
        temp.adjcentList.insert(pair<string, edge>(nameB, edge(nameB, w, A2B)));
        temp.adjcentList[nameB].route.push_back(route);
        vertex_list.insert(pair<string, Vertex>(nameA, temp));
        number_of_vertex++;
    }
    else
    {
        auto y = (*x).second.adjcentList.find(nameB);
        if(y != (*x).second.adjcentList.end())
            (*x).second.adjcentList[nameB].route.push_back(route);
        else
        {
            edge temp;
            temp.name = nameB;
            temp.length = w;
            temp.route.push_back(route);
            temp.direction = A2B;
            (*x).second.adjcentList.insert(pair<string,edge>(nameB,temp));
        }
    }
    x = vertex_list.find(nameB);
    if(x==vertex_list.end())
    {
        Vertex temp;
        temp.name = nameB;
        //temp.road = roadB;
        temp.adjcentList.insert(pair<string, edge>(nameA, edge(nameA, w, B2A)));
        temp.adjcentList[nameA].route.push_back(route);
        vertex_list.insert(pair<string, Vertex>(nameB, temp));
        number_of_vertex++;
    }
    else
    {
        auto y = (*x).second.adjcentList.find(nameA);
        if(y != (*x).second.adjcentList.end())
            (*x).second.adjcentList[nameA].route.push_back(route);
        else
        {
            edge temp;
            temp.name = nameA;
            temp.length = w;
            temp.route.push_back(route);
            temp.direction = B2A;
            (*x).second.adjcentList.insert(pair<string,edge>(nameA,temp));
        }
    }
}

weight Graph::print_path(const string& source, const string& destination)
{
    bool mark = true;
    weight return_1 = test_srcdes(source, destination, mark);
    if(!mark)
        return return_1;
    dijkstra(source, destination);
    if(vertex_list[destination].distance == INFINITE)
    {
        cout<< "不存在从起点到达目的地的线路。"<<endl;
        mark = false;
        return INFINITE;
    }
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
        count--;
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
        x.second.path.fathername.clear();
        x.second.path.fathertome.clear();
        x.second.path.route.clear();
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
            min = x.second.distance;
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
        auto &z  = vertex_list[x.second.name];
        if(z.label == false)
        {
            if(y.distance + x.second.length < z.distance || z.distance == INFINITE)
            {
                z.distance = y.distance + x.second.length;
                /*for(auto road1: z.road)
                {
                    if(find(y.road.begin(), y.road.end(), road1) != y.road.end())
                        temp = road1;
                }*/
                z.path = Path(min_name, x.second.direction);
                z.path.route = x.second.route;
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
        cout<<"终点和起点都不存在。"<<endl;
        mark = false;
        return WRONG_VERTEX;
    }
    if(s1==vertex_list.end())
    {
        cout<<"起点不存在。"<<endl;
        mark = false;
        return WRONG_VERTEX;
    }
    if(s2==vertex_list.end())
    {
        cout<<"终点不存在。"<<endl;
        mark = false;
        return WRONG_VERTEX;
    }
    if(source == destination)
    {
        cout<< "您已在目的地。"<<endl;
        mark = false;
        return WEIGHT_ZEOR;
    }
}

weight inline Graph::print_pathin(const string& source, const string& destination)
{
    vector<Path> path;
    vector<string> route1;
    string temp = destination;
    string temp1;
    while(temp != source)
    {
        path.push_back(vertex_list[temp].path);
        temp = path.back().fathername;
    }
    if(path.size()>1){
    temp = initial_route(path[0],path[1]);
    if(temp.size())
    {
        route1.push_back(temp);
        route1.push_back(temp);
    }
    else
    {
        route1.push_back(path[0].route[0]);
        route1.push_back(path[1].route[0]);
    }
    for(int i=2; i<path.size()-2; i++)
        route1.push_back(path[i].route[0]);
    temp = initial_route(path[path.size()-2],path[path.size()-1]);
    /*for(auto x: path[path.size()-2].route)
    {
        cout<<x<<endl;
        cout<<path[path.size()-2].fathername<<endl;
    }
    for(auto x: path[path.size()-1].route)
    {
        cout<<x<<endl;
        cout<<path[path.size()-1].fathername<<endl;
    }*/
    if(temp.size())
    {
        route1.push_back(temp);
        route1.push_back(temp);
    }
    else
    {
        route1.push_back(path[path.size()-2].route[0]);
        route1.push_back(path.back().route[0]);
    }}
    else
    {
        for(int i=0; i<path.size(); i++)
        route1.push_back(path[i].route[0]);
    }
    


    bool out = true;
    cout<<"(起点,"<<source<<")"<<" -> ";
    temp = route1.back();
    temp1 = path.back().fathertome;
    int i = path.size()-1;
    while(i>0)
    {
        out = true;
        for(int j=0; j<i; j++)
        {
            if(route1[j] == temp)
            {
                i = j-1;
                out = false;
                if(j)
                {
                   cout<<"("<<temp<<","<<temp1<<","<<path[i].fathername<<")"<<" -> ";
                   temp = route1[i];
                   temp1 = path[i].fathertome;
                }
                break;
            }
        }
        if(out)
        {
            i--;
            cout<<"("<<temp<<","<<temp1<<","<<path[i].fathername<<")"<<" -> ";
            temp = route1[i];
            temp1 = path[i].fathertome;
        }
    }
    cout<<"("<<temp<<","<<temp1<<","<<destination<<")"<<endl;
    return vertex_list[destination].distance;
}

string inline Graph:: initial_route(Path& a, Path& b)
{
    string temp;
    for(auto x:a.route)
        for(auto y:b.route)
        {
            if(x==y)
            {
                temp = x;
                return x;
            }
        }
    return temp;
}
#endif
