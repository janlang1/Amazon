#include <vector>
#include <iostream>
#include "msort.h"

using namespace std;

struct Comp1 {
  bool operator()(int a, int b)
  {
    return a > b;
  }
}c1;
struct Comp2 {
  bool operator()(int a, int b)
  {
    return a < b;
  }
}c2;

int main(){
	vector<int> test1 = {};
	mergeSort(test1, c2);
  for(auto i : test1)
    cout << i << " ";
  cout << endl;
  mergeSort(test1, c1);
  for(auto i : test1)
    cout << i << " ";
  cout << endl;
}