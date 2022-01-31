#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>

using namespace std;

class Node;
class Edge;
class Graph;

//点集
class Node{
public:
    Node(int val){
        value=val;
        in=0;
        out=0;
    }
    int value;
    int in; //入度,有多少个边指向此结点
    int out;	//出度，有多少条边由此结点发散
    vector<Node*> nexts;  //此结点是from的情况下，to结点
    vector<Edge*> edges;	//从此结点出发，发散出边的集合

};
//边集
class Edge
{
public:
    int weight; //权值
    Node* from; //边的起点
    Node* to;   //边的终点
    Edge();
    Edge(int weight,Node* from,Node* to){
        this->weight = weight;
        this->from = from;
        this->to = to;
    }
};
//图
class Graph{
public:
    map<int,Node*> nodes;// 点序号和结点的映射集合
    set<Edge*> edges;	//边的集合

};

//创建图
Graph* CreateGraph(int matrix[][3],int n){	//n表示边数 即matrix.size()
    Graph* graph = new Graph;
    for(int i=0;i!=n;i++){
        /*
         * 转换格式:如
         *  {from,to,weight}
         *  {1,3,5},
            {3,5,22},
            {5,7,2},
            {7,9,6},
            {2,4,1},
            {5,4,0},
            {4,6,0},
         */
        int weight = matrix[i][2];
        int from = matrix[i][0];
        int to = matrix[i][1];

        //起点和终点若不存在就创建
        if(graph->nodes.find(from) == graph->nodes.end()){
            graph->nodes[from] = new Node(from);
        }

        if(graph->nodes.find(to) == graph->nodes.end()){
            graph->nodes[to] = new Node(to);
        }

        Node* fromnode = graph->nodes[from];
        Node* tonode = graph->nodes[to];

        //通过起点终点得到一条边
        Edge* newedge=new Edge(weight,fromnode,tonode);
        //将终点放入起点的点集中
        fromnode->nexts.push_back(tonode);
        //起点的出度加1
        fromnode->out++;

        //该点产生向后有一条边
        fromnode->edges.push_back(newedge);

        //终点的入度加1
        tonode->in++;

        //将边加入边集
        graph->edges.insert(newedge);
    }
    return graph;
}

//宽度(广度)优先遍历
void bfs(Node* node){
    /*
     * 将新结点加入队列,并且放入集合set中标记,意味着该点已经出现过,避免重复的情况
     * 再依次从队列中弹出元素,再将 弹出元素 后面的元素(当前元素.next[i])(如果是新的) 加入队列和集合set中,否则跳过
     */
    if(node==nullptr) return;
    queue<Node*> q;
    set<Node*> set;

    q.push(node);
    set.insert(node);
    while (!q.empty()){
        Node* cur = q.front();	//保存deq队列首元
        q.pop();//出队列打印
        cout << cur->value << " ";
        for (int i = 0 ; i < cur->nexts.size() ; i++){
            Node* next = cur->nexts[i];
            if (set.find(next) == set.end()){
                q.push(next);
                set.insert(next);
            }
        }
    }
    cout<<endl;
}

//图的深度优先遍历
void dfs(Node* node){
    if(node==nullptr) return;

    stack<Node*> s;
    set<Node*> set;
    s.push(node);
    set.insert(node);
    cout<<node->value<<" ";
    while(!s.empty()){
        Node* cur = s.top();
        s.pop();
        for(vector<Node*>::iterator next = cur->nexts.begin() ; next != cur->nexts.end() ; next++){
            //如果next没有在集合set中被标记，就再次将cur压入栈中，将当前的next压入栈中，并且把next在set中标记上
            //下一轮从栈中弹出的就是next，并继续往下，直到next.next为0或元素都已经出现过，就开始回到上一层，即从栈中弹出最上方元素
            if(set.find(*next) == set.end()){
                s.push(cur);
                s.push(*next);
                set.insert(*next);
                cout << (*next)->value << " ";
                break;
            }
        }
    }
    cout<<endl;
}

//拓扑排序
list<Node*> sortedTopology(Graph graph) {
    //点作为key,入度作为value
    map<Node*, int> m;
    //入度为0时点进入该队列(入度为0说明该点就是起点)
    queue<Node*> zeroQueue;

    //将所有点和入度大小的关系存在m中
    for (map<int, Node*>::iterator it = graph.nodes.begin() ; it != graph.nodes.end() ; it++){
        m[it->second] = it->second->in;
        if (it->second->in == 0) zeroQueue.push(it->second);
    }

    list<Node*> result;
    while (!zeroQueue.empty()){
        //弹出入度为0的结点放到结果中，并且 删除与该点后面的点的联系，即后面的点的入度减1
        Node* cur = zeroQueue.front();
        zeroQueue.pop();
        result.push_back(cur);
        for (int i = 0 ; i < cur->nexts.size() ; i++){
            Node* next = cur->nexts[i];
            m[next] = --(next->in);
            //如果入度减1后变成0，说明又是新的起点，放入队列
            if (next->in == 0){
                zeroQueue.push(next);
            }
        }
    }
    return result;
}

//类似于并查集的集合，用于kruscalMST函数
class mySet{
public:
    map<Node*, list<Node*>*> setmap;
    mySet(list<Node*> nodes){
        list<Node*>* Set;
        for (list<Node*>::iterator it = nodes.begin() ; it != nodes.end() ; it++){
            Node* cur = (*it);
            Set = new list<Node*>();
            //初始的父结点是自己
            Set->push_back(cur);
            setmap[cur] = Set;
        }
    }
    bool isSameSet(Node* from, Node* to){
        list<Node*>* fromSet = setmap[from];
        list<Node*>* toSet = setmap[to];
        bool t = (fromSet == toSet);
        return t;
    }
    void Union(Node* from, Node* to){
        list<Node*>* fromSet = setmap[from];
        list<Node*>* toSet = setmap[to];
        for (list<Node*>::iterator it = toSet->begin() ; it != toSet->end() ; it++){
            fromSet->push_back(*it);
            setmap[*it] = fromSet;
        }
    }
};

struct CMP{
    bool operator()(Edge* e1, Edge* e2){
        return e1->weight > e2->weight;
    }
};

//最小生成树 kruscal
set<Edge*> kruscalMST(Graph graph){
    //1：将graph中所有点放入链表l中
    list<Node*> l;
    for (map<int, Node*>::iterator it = graph.nodes.begin() ; it != graph.nodes.end() ; it++){
        l.push_back(it->second);
    }
    //2：myset类似于并查集，但不是真正的并查集
    mySet myset(l);
    //3：初始化优先队列，（CMP为自定义比较大小的方式，按照从小到大），将边放入该队列pq中
    priority_queue<Edge*, vector<Edge*>, CMP> pq;
    for (set<Edge*>::iterator it = graph.edges.begin() ; it != graph.edges.end() ; it++){
        pq.push(*it);
    }
    //4：返回的结果集
    set<Edge*>* result = new set<Edge*>;
    while (!pq.empty()){
        //5：弹出weight最小的边，并且比较边的 起点处from 和 终点处to所指向的链表是否相同，相同则进入下一轮
        Edge* edge = pq.top();
        pq.pop();
        if (!myset.isSameSet(edge->from, edge->to)){
            result->insert(edge);
            myset.Union(edge->from, edge->to);
        }
    }
    return *result;
}

//最小生成树 prim
set<Edge*> primMST(Graph graph){
    priority_queue<Edge*, vector<Edge*>, CMP> pq;
    //标记某一点是否出现过
    set<Node*> s;
    set<Edge*>* result = new set<Edge*>();
    //该处for循环是为了处理森林,即不相关的两张图
    for (map<int, Node*>::iterator it = graph.nodes.begin() ; it != graph.nodes.end() ; it++){
        //当前点是起始点
        if (!s.count((*it).second)){
            s.insert((*it).second);
            //由1个点,解锁所有相连的边，放入优先队列
            for (vector<Edge*>::iterator cur = (*it).second->edges.begin() ; cur != (*it).second->edges.end() ; cur++){
                pq.push(*cur);
            }
            while (!pq.empty()){
                //弹出在优先队列中 已经解锁的 权值最小的边
                Edge* edge = pq.top();
                pq.pop();
                //弹出边的终点，可能是一个新的点
                Node* toNode = edge->to;
                //如果标记中从未出现该点，就是新的点，插入标记并且将该边加入结果集中
                if (!s.count(toNode)){
                    s.insert(toNode);
                    result->insert(edge);
                    //会有重复的边重复进入队列，但即使进入后toNode已经在s中被标记，所以重复的边会被跳过
                    for (vector<Edge*>::iterator nextEdge = toNode->edges.begin() ; nextEdge != toNode->edges.end() ; nextEdge++){
                        pq.push(*nextEdge);
                    }
                }
            }
        }
    }
    return *result;
}

//获取最短距离并且没有被锁死的结点， 用于dijkstra函数
Node* getMinDistanceAndUnselectedNode(map<Node*, int> &distanceMap, set<Node*> &selectedNodes){
    //初始化最小的结点为空，最小的距离为无穷大
    Node* minNode = nullptr;
    int minDistance = 0x7FFFFFFF;
    //遍历传入的distanceMap，分别拿到结点和节点的最小值
    for (map<Node*, int>::iterator it = distanceMap.begin() ; it != distanceMap.end() ; it++){
        Node* node = (*it).first;
        int distance = (*it).second;
        //如果上一步拿到的结点不在被标记的集合中，并且 拿到的距离(distance) 小于 最小距离(minDistance)
        //最小的结点就是该结点，最小的 距离(minDistance) 更新变成更小的(distance)
        if (!selectedNodes.count(node) && distance < minDistance){
            minNode = node;
            minDistance = distance;
        }
    }
    return minNode;
}

//获取某一点到图中其他点的最短距离，注意权值不能为负数
map<Node*, int> dijkstra(Node* pNode){
    /*
     * 从pNode出发到所有点的最小距离
     * key：从head出发到达key
     * value：从head出发到达key的最短距离
     * 如果在表中没有T的记录，含义是从head出发到T的距离为∞
     */
    map<Node*, int>* distanceMap = new map<Node*, int>();
    (*distanceMap)[pNode] = 0;
    //已经求过距离的节点存在该集合中，并且以后不使用
    set<Node*> selectedNodes;
    Node* minNode = getMinDistanceAndUnselectedNode(*distanceMap, selectedNodes);
    while (minNode){
        //distance是当前结点 距离 起始结点pNode 的最短距离
        int distance = (*distanceMap)[minNode];
        for (vector<Edge*>::iterator it = minNode->edges.begin() ; it != minNode->edges.end() ; it++){
            Node* toNode = (*it)->to;
            //如果该点不在被标记的集合中,该点(toNode)的距离就是 minNode点的value值 加上 该点(toNode)的权值
            if (!distanceMap->count(toNode)) (*distanceMap)[toNode] = distance + (*it)->weight;

            //如果该点已经被标记了，就用原map中 toNode的value值 和 minNode点的distance加上toNode点的权值 做比较
            (*distanceMap)[toNode] = min((*distanceMap)[toNode], distance + (*it)->weight);
        }
        //因为该点的所有情况已经遍历完，将该点放入集合selectedNodes中 不再使用
        selectedNodes.insert(minNode);
        //获取下一个minNode
        minNode = getMinDistanceAndUnselectedNode(*distanceMap, selectedNodes);
    }
    return *distanceMap;
}


void test01(){
    //测试创建图,BFS,DFS以及拓扑排序
    //有向图
    int arr[][3] = {
            {1,3,5},
            {3,5,22},
            {5,7,2},
            {7,9,6},
            {2,4,1},
            {5,4,0},
            {4,6,0},
    };
    Graph* graph = CreateGraph(arr, 7);
    Node *temp = graph->nodes.begin()->second;
    cout << "BFS: ";
    bfs(temp);
    cout << endl;
    cout << "DFS: ";
    dfs(temp);
    cout << endl;

    cout << "sortedTopology: ";
    list<Node*> result = sortedTopology(*graph);
    list<Node*>::iterator it;
    for (it = result.begin() ; it != result.end() ; it++){
        cout << (*it)->value << " ";
    }
    cout << endl;
}

void test02(){
    //测试Cruscal,Prim,dijkstra算法
    //无向图
    int arr[][3] = {
            {1,2,7},
            {2,1,7},
            {1,3,2},
            {3,1,2},
            {1,4,100},
            {4,1,100},
            {2,4,1000},
            {4,2,1000},
            {3,4,4},
            {4,3,4},
            {2,5,100000},
            {5,2,100000}
    };
    Graph* graph = CreateGraph(arr, 12);
    set<Edge*> kruscal = kruscalMST(*graph);
    cout << "kruscal: ";
    for (set<Edge*>::iterator it = kruscal.begin() ; it != kruscal.end() ; it++){
        cout << (*it)->weight << " ";
    }
    cout << endl;

    set<Edge*> prim = primMST(*graph);
    cout << "prim: ";
    for (set<Edge*>::iterator it = kruscal.begin() ; it != kruscal.end() ; it++){
        cout << (*it)->weight << " ";
    }
    cout << endl;

    map<Node*, int> minDistance = dijkstra(graph->nodes.begin()->second);
    cout << "dijkstra: ";
    for (map<Node*, int>::iterator it = minDistance.begin() ; it != minDistance.end() ; it++){
        cout << graph->nodes.begin()->second->value << " - " << (*it).first->value << " = " << (*it).second << endl;
    }
}

int main() {

    //test01();
    test02();
    return 0;
}
