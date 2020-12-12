#include <iostream>
#include "BinaryTree.h"
#include <ctime>
int main() {
    BinaryTree<int, int> tree;
//    tree.add(10,10);
//    tree.add(8,8);
    const int q = 10000;
    for(int i = q/4; i < 3*q/4; ++i){
        tree.add(i, i);
        std::cout << i << " added\n";
    }
    for(int i = 0; i < q/4; ++i){
        tree.add(i, i);
        std::cout << i << " added\n";
    }
    for(int i = 3*q/4; i < q; ++i){
        tree.add(i, i);
        std::cout << i << " added\n";
    }
    bool success;
    int result;
    for(int i = 0; i < q; ++i){
        std::tie(success, result) = tree.find(i);
        if(!success){
            std::cout << i <<" Didn't find" << std::endl;
        }
    }
    std::tie(success, result) = tree.find(q+3);
    std::cout << "_______________________________\n" << success << std::endl
    <<"_______________________________\n"<< std::endl;
    srand(time(0));
    for(int i = q/4; i < q/4 + 10; ++i){
        tree.remove(i);
    }
    for(int i = 0; i < 10; ++i){
        tree.remove(i);
    }
    std::cout << "_______________________________\n";
    return 0;
}
