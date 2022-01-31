#include <iostream>
using namespace std;
#include <stack>
#include <queue>
#include <map>
#include <algorithm>

template <class T>
class Node{
public:
    T data;
    Node* left;
    Node* right;;

    Node(){
        left = nullptr;
        right = nullptr;
        data = 0;
    }
};

template <class T>
class BinaryTree{
public:
    Node<T> *node;

    BinaryTree(){
        node = new Node<T>;
    }

    bool isEmpty(){
        if (node == nullptr) return true;
        else return false;
    }

    //递归-先序遍历
    void Recursive_PreOrder(Node<T>* pNode){
        if (pNode == nullptr) return;
        cout << pNode->data << " ";
        Recursive_PreOrder(pNode->left);
        Recursive_PreOrder(pNode->right);
    }

    //非递归-先序遍历
    void NonRecursive_PreOrder(Node<T>* pNode){
        /*
         * 1：将第一个结点压入栈中
         * 2：弹出该结点并打印,如果该结点的左右结点不为空,按照 右 左 的顺序压入栈中 (因为是先序遍历,所以会先弹出左)
         * 3：重复第二步,直到栈为空。
         */
        stack<Node<T>*> s;
        s.push(pNode);
        while (!s.empty()){
            Node<T>* cur = s.top();
            s.pop();
            if (cur->right != nullptr) s.push(cur->right);
            if (cur->left != nullptr) s.push(cur->left);
            cout << cur->data << " ";
        }
        cout << endl;
    }

    //递归-中序遍历
    void Recursive_InOrder(Node<T>* pNode){
        if (pNode == nullptr) return;
        Recursive_InOrder(pNode->left);
        cout << pNode->data << " ";
        Recursive_InOrder(pNode->right);
    }

    //非递归-中序遍历
    void NonRecursive_InOrder(Node<T>* pNode){
        /*
         * 1：将结点压入栈中,如果结点的左孩子存在 那就继续压入栈中,直到左孩子为空
         * 2：第一步之后,打印栈顶元素,访问栈顶元素的右孩子,弹出栈顶元素并又重新开始第一步
         * 3：经过上面两步后如果栈为空则遍历结束
         */
        stack<Node<T>*> s;
        Node<T>* temp = pNode;
        while (!s.empty() || temp != nullptr){
            if (temp != nullptr){
                s.push(temp);
                temp = temp->left;
            }
            else {
                cout << s.top()->data << " ";
                temp = s.top()->right;
                s.pop();
            }
        }
        cout << endl;
    }

    //递归-后序遍历
    void Recursive_PostOrder(Node<T>* pNode){
        if (pNode == nullptr) return;
        Recursive_PostOrder(pNode->left);
        Recursive_PostOrder(pNode->right);
        cout << pNode->data << " ";
    }

    //非递归-后序遍历
    void NonRecursive_PostOrder(Node<T>* pNode){
        /*
         *    与先序遍历类似
         * 1：将第一个结点压入 栈1 中
         * 2：从 栈1 弹出该结点并打印,如果该结点的左右结点不为空,按照 左 右 的顺序压入 栈1 中 (与先序遍历相反)
         * 3：将 栈1 中弹出的结点放到 栈2 中
         * 4：重复第二,三步,直到 栈1 为空,遍历结束, 栈2 依次弹出的顺序就是后序遍历的顺序。
         */
        stack<Node<T>*>s1;
        stack<Node<T>*>s2;
        s1.push(pNode);
        while (!s1.empty()){
            Node<T>* cur = s1.top();
            s2.push(s1.top());
            s1.pop();
            if (cur->left != nullptr) s1.push(cur->left);
            if (cur->right != nullptr) s1.push(cur->right);
        }
        while (!s2.empty()){
            cout << s2.top()->data << " ";
            s2.pop();
        }
    }



    //先序创建二叉树
    Node<T>* createPreBiTree(){
        /*
         *      如:   124##5##36##7##
         */
        T a;
        Node<T>* root;
        a = cin.get();
        if (a == '#'){
            root = nullptr;
        }
        else{
            root = new Node<T>;
            root->data = a;
            root->left = createPreBiTree();
            root->right = createPreBiTree();
        }
        return root;
    }

    //复制
    void copy(Node<T>* pNode , Node<T>* &root){
        if (pNode == nullptr) return;
        root = new Node<T>;
        root->data = pNode->data;
        copy(pNode->left,root->left);
        copy(pNode->right,root->right);
    }

    //计算深度
    int depth(Node<T>* pNode){
        if (pNode == nullptr) return 0;

        int m = depth(pNode->left);
        int n = depth(pNode->right);
        return m > n ? m + 1 : n + 1;
    }

    //计算结点的个数
    int nodeCount(Node<T>* pNode){
        if (pNode == nullptr) return 0;
        return nodeCount(pNode->left) + nodeCount(pNode->right) + 1;
    }

    //计算叶子的个数
    int leafCount(Node<T>* pNode){
        if (pNode == nullptr) return 0;
        if (pNode->left == nullptr && pNode->right == nullptr) return 1;
        return leafCount(pNode->left) + leafCount(pNode->right);
    }

    int maxWidth_usemap(Node<T>* head){
        if (head == nullptr) return 0;
        queue<Node<T>*> q;
        q.push(head);

        //结点和结点所在层的映射 从第一层依次往下
        map<Node<T>*, int> m;
        m[head] = 1;
        //初始化最开始在第一层
        int curLevel = 1;
        //当前层所存在的元素个数
        int curLevelNodes = 0;
        //最大值
        int max = 0;
        while (!q.empty()){
            //取出队列中最前面的结点并拿到其所在层数
            Node<T>* temp = q.front();
            q.pop();
            int curNodeLevel = m[temp];
            //有左/右孩子就将左孩子以及左孩子所在层数放入map中,加1是因为既然是当前元素的左/右孩子那就必然在他下一层
            if (temp->left != nullptr){
                m[temp->left] = curNodeLevel + 1;
                q.push(temp->left);
            }
            if (temp->right != nullptr){
                m[temp->right] = curNodeLevel + 1;
                q.push(temp->right);
            }

            //如果 当前结点所在的层数 等于当前层数 ， 当前层的元素个数加1
            if (curNodeLevel == curLevel) curLevelNodes++;

            //当前结点所在的层数 不等于 当前到达的层数，说明进入下一层了 ， 并且已经有了一个元素
            else {
                max = std::max(max,curLevelNodes);
                curLevel++;
                curLevelNodes = 1;
            }
        }
        //最后一层结束后不会再有下一层，所以不会进入else中执行max的操作
        max = std::max(max,curLevelNodes);
        return max;
    }


    void BFS(Node<T>* pNode){
        if (pNode == nullptr) return;
        queue<Node<T>*> q;
        q.push(pNode);
        while (!q.empty()){
            Node<T>* cur = q.front();
            cout << cur->data << " ";
            q.pop();
            if (cur->left != nullptr) q.push(cur->left);
            if (cur->right != nullptr) q.push(cur->right);
        }
        cout << endl;

    }


};

void test(){
    BinaryTree<char> tree;
    tree.node = tree.createPreBiTree();

    cout << "Recursive_PreOrder: ";
    tree.Recursive_PreOrder(tree.node);
    cout << endl;
    cout << "NonRecursive_PreOrder: ";
    tree.NonRecursive_PreOrder(tree.node);
    cout << endl;



    cout << "Recursive_InOrder: ";
    tree.Recursive_InOrder(tree.node);
    cout << endl;
    cout << "NonRecursive_InOrder: ";
    tree.NonRecursive_InOrder(tree.node);
    cout << endl;

    cout << "Recursive_PostOrder: ";
    tree.Recursive_PostOrder(tree.node);
    cout << endl;
    cout << "NonRecursive_PostOrder: ";
    tree.NonRecursive_PostOrder(tree.node);
    cout << endl;

    cout << "max width = ";
    cout << tree.maxWidth_usemap(tree.node) << endl;

    tree.BFS(tree.node);
}

int main(){
    test();
    return 0;
}