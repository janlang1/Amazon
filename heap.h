#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);

private:
    /// Add whatever helper functions you need below

    void trickleDown(int index); //heapify
    void trickleUp(int index);

    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)
{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{

}

//heapify
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleDown(int index)
{
  //check if its a leaf
  int inbound = store_.size();
  //no children
  if(index * m_ + 1 >= inbound) return;
  int firstChild = m_ * index + 1; // start w/ left
  int i = 1;
  int bestChild = firstChild;
  inbound = store_.size();
  while(firstChild + i < inbound && i < m_) {
    int currIndex = firstChild + i;
    if(c_(store_[currIndex].first, store_[bestChild].first))
      bestChild = currIndex;
    i++;
  } 
  if(!c_(store_[index].first,store_[bestChild].first))
  {

    //swap map indexes as well
    //index is parent
    int index1 = keyToLocation_[store_[index].second];
    int index2 = keyToLocation_[store_[bestChild].second];
    keyToLocation_[store_[bestChild].second] = index2; 
    keyToLocation_[store_[index].second] = index1;

    //swap store
    std::pair<double, T> temp = store_[index];
    store_[index] = store_[bestChild];
    store_[bestChild] = temp;
  }
  else
  {
    return;
  }
  
  trickleDown(bestChild);
}

//trickle up
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleUp(int index)
{
  int parent = (index-1)/m_;
  while(parent >= 0 && c_(store_[index].first,store_[parent].first))
  {
    //swap function

    //swap map indexes as well
    int index1 = keyToLocation_[store_[index].second];
    int index2 = keyToLocation_[store_[parent].second];
    keyToLocation_[store_[parent].second] = index2; 
    keyToLocation_[store_[index].second] = index1;

    //swap pairs in store_
    std::pair<double, T> temp = store_[parent];
    store_[parent] = store_[index];
    store_[index] = temp;

    index = parent;
    parent = (index-1)/m_;
  }
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    store_.push_back(make_pair(priority, item));
    keyToLocation_.insert(make_pair(item, store_.size() - 1));
    trickleUp(store_.size()-1);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double priority, const T& item)
{
    // You complete
    typename std::unordered_map<T, size_t, Hasher, TComparator>::iterator it = keyToLocation_.find(item);
    if(it != keyToLocation_.end())
    {
        int index = (*it).second;
        store_[index].first = priority;
        trickleUp(index);
    }
    
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }

    // You complete
    return store_[0].second;

}

/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    // You complete
    if (store_.size() == 1){
        //delete from map
        keyToLocation_.erase(store_[0].second);
        store_.pop_back();
    }
    else{
        //delete from map
        keyToLocation_.erase(store_[0].second);
        store_[0] = store_.back();
        store_.pop_back();
        trickleDown(0); 
    }


}

/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}


#endif

