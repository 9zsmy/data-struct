#include <iostream>
using namespace std;
#include <stack>
#include <set>
#include <map>

class TrieNode;
class Trie;

//前缀树的结点
class TrieNode{
public:
    //pass代表最开始到当前位置为前缀的个数
    //如该结点是b，上一个结点是a，pass = 1，代表以ab为前缀的单词个数是1
    //如果end为1，说明有一个单词是ab
    int pass;
    int end;
    TrieNode* next[26];
    TrieNode(){
        pass = 0;
        end = 0;
        for (int i = 0 ; i < 26 ; i++){
            next[i] = nullptr;
        }

    }
};

class Trie{
public:
    TrieNode* root;
    Trie(){
        root = new TrieNode();
    }

    void insert(string word){
        if (word.empty()) return;
        TrieNode* node = root;
        node->pass++;
        int index = 0;
        for (int i = 0 ; i < word.length() ; i++){
            index = word[i] - 'a';
            //由字符对应成走哪条路
            if (node->next[index] == nullptr) node->next[index] = new TrieNode();
            node = node->next[index];
            node->pass++;
        }
        node->end++;
    }

    //查找某一单词是否出现过
    int search(string word){
        if (word.empty()) return 0;
        TrieNode* node = root;
        for (int i = 0 ; i < word.length() ; i++){
            int index = word[i] - 'a';
            //如果当前结点的next[index]已经是空指针，但元素还没有寻找结束，说明word不存在，如存在ab，查找abc，返回0
            if (node->next[index] == nullptr) return 0;
            //不是空指针就继续往下
            node = node->next[index];
        }
        return node->end;
    }

    //查找以word为前缀的单词
    int prefixNumber(string word){
        if (word.empty()) return 0;
        TrieNode* node = root;
        for (int i = 0 ; i < word.length() ; i++){
            int index = word[i] - 'a';
            if (node->next[index] == nullptr) return 0;
            node = node->next[index];
        }
        return node->pass;
    }

    //删除一个单词
    void deleteWord(string word){
        //如果存在这个单词再执行
        if (search(word)){
            TrieNode* node = root;
            //因为是删除，所以该条单词路径上的pass要减1
            node->pass--;

            //栈中放一个哈希表，key为结点，value为在这个结点中的位置(下标)，在栈中存放要释放内存的结点
            stack<map<TrieNode*, int>> deleteNode;
            //集合set为了避免相同的结点出现在栈中，重复释放同一块内存
            set<TrieNode*> deleteSet;

            int index = 0;
            for (int i = 0 ; i < word.length() ; i++){
                index = word[i] - 'a';
                TrieNode* cur = node->next[index];
                //当前结点是node，下一个结点为cur，cur的pass变成0的话说明下一个结点需要被释放
                //node[index]即下一个结点的位置，将node和index的信息 先放入哈希表中 再将 哈希表 放入栈中
                if (--cur->pass == 0 && deleteSet.count(cur) == 0){
                    map<TrieNode*, int> m;
                    m[node] = index;
                    deleteNode.push(m);
                    deleteSet.insert(cur);
                }
                node = cur;
            }
            //个数减1
            node->end--;
            //处理栈中数据，即需要释放内存的地方
            while (!deleteNode.empty()) {
                map<TrieNode *, int> temp = deleteNode.top();
                for (map<TrieNode *, int>::iterator it = temp.begin(); it != temp.end(); it++) {
                    //it->first->next[it->second]相当于node[index],需要被清理
                    delete[] it->first->next[it->second];
                    //把这个位置设置为空指针
                    it->first->next[it->second] = nullptr;
                }
                deleteNode.pop();
            }
        }
    }
};

void test01(){
    Trie tree;
    tree.insert("abandon");
    tree.insert("he");
    tree.insert("hello");
    tree.insert("world");
    tree.insert("world");
    cout << "search: " << tree.search("he") << endl;
    cout << "prefixNumber: " << tree.prefixNumber("he") << endl;

    tree.deleteWord("world");
    cout << "prefixNumber: " << tree.prefixNumber("world") << endl;

    tree.deleteWord("world");
    cout << "prefixNumber: " << tree.prefixNumber("world") << endl;
}

int main() {
    test01();
    return 0;
}
