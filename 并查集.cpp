#include <iostream>
#include <map>
#include <list>
#include <stack>

using namespace std;

template <typename T>
class Node{
public:
    T value;
    Node(T value){
        this->value = value;
    }
};

//并查集
template <typename T>
class unionSet{
public:
    //样本 对应 该样本所包含的的集合
    map<T, Node<T>* >* nodes;
    //某一节点的父亲是谁
    map<Node<T>*, Node<T>* >* parents;
    //记录样本集合的个数，因为合并时是将 较小集合的元素 合并到 较大集合 中
    map<Node<T>*, int>* mapSize;

    unionSet(list<T> values){
        nodes = new map<T, Node<T>* >();
        parents = new map<Node<T>*, Node<T>* >();
        mapSize = new map<Node<T>*, int>();

        for (typename list<T>::iterator it = values.begin() ; it != values.end() ; it++){
            Node<T>* node = new Node<T>(*it);
            //初始化时每个集合都只有自己，并且自己是自己的父亲，容量为1
            nodes->template insert(make_pair(*it, node));
            parents->template insert(make_pair(node, node));
            mapSize->template insert(make_pair(node, 1));
        }
    }

    //找到传入节点的最顶层的位置 并 保留在栈中 ， 依次弹出栈中元素将其父亲节点设置成最底层的元素，下一次查找时就只需要一次就可以直接拿到
    Node<T>* findFather(Node<T>* cur){
        stack<Node<T>*> s;
        while (cur != parents->find(cur)->second){
            s.push(cur);
            cur = parents->find(cur)->second;
        }

        while (!s.empty()){
            parents->find(s.top())->second = cur;
            s.pop();
        }
        return cur;
    }

    //如果两个节点的父亲相同则是相同的集合
    bool isSameSet(T a, T b){
        return findFather(nodes->find(a)->second) == findFather(nodes->find(b)->second);
    }

    //合并
    void Union(T a, T b){
        Node<T>* aHead = findFather(nodes->find(a)->second);
        Node<T>* bHead = findFather(nodes->find(b)->second);
        //如果两个不是同一个集合
        if (aHead != bHead){
            //获取集合的大小，因为较小的要插到较大的集合中
            int aSize = mapSize->find(aHead)->second;
            int bSize = mapSize->find(bHead)->second;

            Node<T>* bigNode = aSize >= bSize ? aHead : bHead;
            Node<T>* smallNode = bigNode == aHead ? bHead : aHead;
            //小集合的父亲结点 变成 较大的集合
            parents->find(smallNode)->second = bigNode;
            //合并后的集合大小增加
            mapSize->find(bigNode)->second = aSize + bSize;
            mapSize->erase(smallNode);
        }

    }

};

void test01(){
    list<char> values;
    values.push_back('a');
    values.push_back('b');
    values.push_back('c');
    values.push_back('d');
    values.push_back('e');

    unionSet<char> us(values);
    cout << (us.isSameSet('a','c') == 1 ? "YES" : "NO") << endl;
    us.Union('a','c');
    cout << (us.isSameSet('a','c') == 1 ? "YES" : "NO") << endl;
    us.Union('a','e');
    cout << (us.isSameSet('a','b') == 1 ? "YES" : "NO") << endl;

}

int main() {
    test01();
    return 0;
}
