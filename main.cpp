#include <iostream>
#include "BinaryTree.h"
#include <ctime>
int main() {
    BinaryTree<int, int> tree;
    const int q = 10000;
    for(int i = 0; i < q; ++i){
        tree.add(i, i);
    }
    srand(time(0));
    for(int i = 0; i < 10; ++i){
        tree.remove(i);
    }
    BinaryTree<int, int> copy = tree;
    for(int i = 10; i < q - 10; ++i){
        tree.remove(i);
    }
    copy.add(100001, 1);
    return 0;
}
