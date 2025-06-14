#include "Wordlist.h"
#include <iostream>
#include <string>

int main(){

    //Create Wordlist 
    Wordlist wl("test.txt");
    std::cout << "Words in AVL tree (in order): \n";
    wl.printWords();

    //Print stats
    std::cout << "\nStatistics: \n";
    wl.printStatistics();

    //Test remove 
    std::cout << "\nRemoving 'is'";
    wl.remove("is");
    std::cout <<"\nUpdated words: \n";
    std::cout<<"\nUpdated statistics: \n";

    return 0;


}