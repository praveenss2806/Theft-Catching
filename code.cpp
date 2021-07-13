#include<bits/stdc++.h>
using namespace std;

//for creating adjacency list
typedef vector<vector<int>> ADJ;
//for creating min_heap
typedef priority_queue<pair<int, int>,vector<pair<int,int>>,greater<pair<int,int>>> Queue;

int INF=INT_MAX;

class ASTAR{
        //No: of nodes;
        int nodes;
        //Adjacency list and to store edge cost;
        ADJ adj_list;
        ADJ edge_cost;
        //store distance
        vector<int> distance;
        //Mark visited nodes
        vector<bool> visited;
        //Nodes in open set
        vector<int> workset;
        //Storing huerestic values
        vector<int> p;
        //Coordinates
        vector<pair<int,int>> cord;
public:
        //Constructor
        ASTAR(int n,ADJ adj,ADJ cost,vector<pair<int,int>> xy){
                nodes=n;
                adj_list=adj;
                edge_cost=cost;
                cord=xy;
                distance=vector<int>(n,INF);
                visited=vector<bool>(n);
                p=vector<int>(n,-1);
                workset.reserve(n);
        }

        //Clear the changes made by the previous query.
        void Clear(){
                int i;
                for(i=0;i<workset.size();i++){
                        int v=workset[i];
                        distance[v]=INF;
                        visited[v]=false;
                        p[v]=-1;
                }
                workset.clear();
        }

        //To calculate the h value i.e cost+huristics
        void visit(Queue &q,int s,int dist,int heuristic){
                if(distance[s]>dist){
                        distance[s]=dist;
                        q.push({distance[s]+heuristic,s});
                        workset.push_back(s);
                }
        }

        //calculate hueristics
        int heuristics(int u,int d){
                if(p[u]==-1){
                        pair<int,int> p_u=cord[u];
                        pair<int,int> p_d=cord[d];

                        p[u]=sqrt(((p_u.first-p_d.first)*(p_u.first-p_d.first))+((p_u.second-p_d.second)*(p_u.second-p_d.second)));
                }
                return p[u];
        }

        //extract the root node from min heap
        int extractmin(Queue &q){
                pair<int,int> p=q.top();
                q.pop();
                return p.second;
        }

        //visit all the adjacent nodes
        void process(Queue &q,int u,int d,ADJ &adj,ADJ &cost){
                int i;
                for(i=0;i<adj[u].size();i++){
                        int v=adj[u][i];
                        if(visited[v]==false){
                                int w=cost[u][i];
                                visit(q,v,distance[u]+w,heuristics(u,d));
                        }
                }
        }

        //To find the shortest distance
        int query(int s,int d){
                Clear();
                Queue q;
                visit(q,s,0,heuristics(s,d));

                while(!q.empty()){
                        int v=extractmin(q);
                        if(v==d){
                                if(distance[d]!=INF){
                                        return distance[d];
                                }
                                else{
                                        return -1;
                                }
                        }
                        if(visited[v]!=true){
                                process(q,v,d,adj_list,edge_cost);
                                visited[v]=true;
                        }
                }
                return -1;
        }
};

int main(){
        cout<<"\t\t\t\t*************THEFT CATCHING*************\n";

        ifstream file("data.txt");

        //Extra variables for loops and getting data etc
        int i,x,y,u,v,c;
        string ll;
        vector<pair<string,int>> loc;

        //To get the no: of nodes and edges respectively
        int n,m;
        file>>n;
        file>>m;

        //To store the coordinates of the nodes
        vector<pair<int,int>> xy(n);

        //To get and store the coordinates of the node
        for(i=0;i<n;i++){
                file>>x>>y;

                file>>ll;
                loc.push_back(make_pair(ll,i));

                xy[i]=make_pair(x,y);
        }

        //To store data & cost of the graph in adjacency list;
        ADJ adj(n);
        ADJ cost(n);

        //To get the graph's data
        for(i=0;i<m;i++){
                file>>u>>v>>c;
                //Storing the directed graph
                adj[u-1].push_back(v-1);
                //Storing Edge cost
                cost[u-1].push_back(c);
        }

        ASTAR astar(n,adj,cost,xy);

        //Extra Variables
        int t,ok=0,mn=INT_MAX,crime,val;
        vector<pair<string,int>> police_dist;
        string str,cr,ans;

        file>>t;
        cout<<"Enter the location of the crime:";
        cin>>cr;

        //To get node no: of the the location
        for(i=0;i<loc.size();i++){
                if(loc[i].first==cr){
                        crime=loc[i].second;
                        ok=1;
                        break;
                }
        }
        if(!ok){
                cout<<"Sorry Location Not found!!!\n";
                return 0;
        }
        //cout<<crime<<"*\n";

        //To find the shortest dist between every police station from location
        for(i=0;i<t;i++){
                file>>u;
                file>>str;
                val=astar.query(u-1,crime);
                police_dist.push_back(make_pair(str,val));
        }

        cout<<"\nDistance for each police station:-\n";
        //To print all the distances and to find the min distance police station
        for(i=0;i<police_dist.size();i++){
                cout<<police_dist[i].first<<" is "<<police_dist[i].second<<" KM\n";

                if(mn>police_dist[i].second && police_dist[i].second>0){
                        mn=police_dist[i].second;
                        ans=police_dist[i].first;
                }
        }
        if(mn==INT_MAX){
                cout<<"\nSorry there is no police station near you!!!\n";
                return 0;
        }

        cout<<"\nThe nearest Police station "<<ans<<" has been informed!!\n";
        cout<<"\n\t\t\t\t*********Your Help will arrive Soon*****\n";

        return 0;
}
