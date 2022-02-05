#include <iostream>

template <typename T>
class MaxHeap{
private:
    T* data;
    int count;
    void shiftUp(int count){
        while (count > 1 && data[count / 2] < data[count]){
            std::swap(data[count/2], data[count]);
            count /= 2;
        }
    }

    void shiftDown(int count){
        while (2 * count <= this->count){
            int i = 2 * count;
            if (i + 1 <= this->count && data[i + 1] > data[i]) i += 1;
            if (data[count] >= data[i]) break;
            std::swap(data[count],data[i]);
            count = i;
        }
    }
public:
    MaxHeap(int capacity){
        data = new T[capacity + 1];
        count = 0;
    }
    ~MaxHeap(){
        delete[] data;
    }
    int size(){
        return count;
    }
    bool isEmpty(){
        return count == 0;
    }
    void insert(T data){
        this->data[count + 1] = data;
        count++;
        shiftUp(count);
    }
    void travelHeap(){
        for (int i = 0 ; i < count ; i++) std::cout << data[i] << " ";
        std::cout << std::endl;
    }
    T extractMax(){
        T ret = data[1];
        std::swap(data[1],data[count]);
        count--;
        shiftDown(1);
    }
};

void test01(){
    MaxHeap<int> heap(50);
    srand(time(NULL));
    for (int i = 0 ; i < 50 ; i++){
        heap.insert(rand() % 100);
    }

    heap.travelHeap();

    heap.extractMax();
    heap.travelHeap();


}

int main() {
    test01();
    return 0;
}
