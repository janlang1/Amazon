#include <vector>
#include <iostream>

template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, Comparator comp){
	mergeSortHelper(myArray, 0, myArray.size()-1, comp);
}
template <class T, class Comparator>
void mergeArray(std::vector<T>& myArray, int start, int middle, int end, Comparator comp) 
{ 
    int leftArray_max_index = middle - start + 1; 
    int rightArray_max_index =  end - middle; 
	
    std::vector<T> leftArray;
	std::vector<T> rightArray; 
	
	//copy myArray into temperary vectors to merge
    for (int i = 0; i < leftArray_max_index; ++i) 
        leftArray.push_back(myArray[start + i]); 
    for (int j = 0; j < rightArray_max_index; ++j) 
        rightArray.push_back(myArray[middle + 1 + j]); 
  
    int i = 0; // Initial index of first subarray 
    int j = 0; // Initial index of second subarray 
    int k = start; // Initial index of merged subarray 
    while (i < leftArray_max_index && j < rightArray_max_index) 
    { 
        if (comp(leftArray[i],rightArray[j])){ 
            myArray[k] = leftArray[i]; 
            i++; 
        } 
        else{ 
            myArray[k] = rightArray[j]; 
            j++; 
        } 
        k++; 
    } 
  
    //check if there's still data in rightArray
    while (i < leftArray_max_index) 
    { 
        myArray[k] = leftArray[i]; 
        i++; 
        k++; 
		//can i put the ++ in the brakets?
    } 
  
    //check if there's still data in rightArray
    while (j < rightArray_max_index){ 
        myArray[k] = rightArray[j]; 
        j++; 
        k++; 
    } 
} 
template <class T, class Comparator>
void mergeSortHelper(std::vector<T>& myArray, int start, int end, Comparator comp){
	if (start < end)
    {
        int middle = (start+end)/2;
        mergeSortHelper(myArray, start, middle, comp); //first half
        mergeSortHelper(myArray, middle+1, end, comp); //second half
        mergeArray(myArray, start, middle, end, comp);
    }
}