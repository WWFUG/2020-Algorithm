// **************************************************************************
//  File       [sort_tool.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The implementation of the SortTool Class]
//  Modify     [2020/9/15 Mu-Ting Wu]
// **************************************************************************

#include "sort_tool.h"
#include <iostream>
#include <cassert>
using namespace std;

#define INT_MAX 1<<20

// Constructor
SortTool::SortTool() {}

// Insertsion sort method
void SortTool::InsertionSort(vector<int>& data) {
    //TODO
    for(unsigned idx=1; idx < data.size(); ++idx){
        unsigned j = idx;
        while( j > 0 ){
            if( data[j] < data[j-1] ){
                swap(data[j], data[j-1]);
                --j;
            }
            else {
                break;
            }
        }
    }
}

// Quick sort method
void SortTool::QuickSort(vector<int>& data){
    QuickSortSubVector(data, 0, data.size() - 1);
}
// Sort subvector (Quick sort)
void SortTool::QuickSortSubVector(vector<int>& data, int low, int high) {
    // Function : Quick sort subvector
    // TODO : Please complete QuickSortSubVector code here
    // Hint : recursively call itself
    //        Partition function is needed
    if(low < high){
        int p = Partition(data, low, high);
        QuickSortSubVector(data, low, p-1);
        QuickSortSubVector(data, p+1, high);
    }
}

int SortTool::Partition(vector<int>& data, int low, int high) {
    // Function : Partition the vector 
    // TODO : Please complete the function
    // Hint : Textbook page 171
    /*
    cout << "Before sorted..." << endl;
    for(unsigned i=low; i<high; ++i){
        cout << data[i] << " ";
    }
    cout << endl;
    */
    int pivot  = data[high];
    int idxPivot = low;
    for(unsigned i=low; i<=high-1; ++i){
        //cout << data[i] << " " << pivot << endl;
        if(data[i] < pivot){
            //cout << "Swap" << data[idxPivot] << " & " << data[i] << endl;
            swap(data[idxPivot++], data[i]);
        }
    }
    swap(data[high], data[idxPivot]);
    //cout << "Swap" << data[idxPivot] << " & " << data[high] << endl;
    /*cout << "After sorted ... " << endl;
    for(unsigned i=low; i<high; ++i){
        cout << data[i] << " ";
    }
    cout << endl;
    */
    return idxPivot;
}

// Merge sort method
void SortTool::MergeSort(vector<int>& data){
    MergeSortSubVector(data, 0, data.size() - 1);
}

// Sort subvector (Merge sort)
void SortTool::MergeSortSubVector(vector<int>& data, int low, int high) {
    // Function : Merge sort subvector
    // TODO : Please complete MergeSortSubVector code here
    // Hint : recursively call itself
    //        Merge function is needed
    if(high-low>=1){
        int middle = (low+high)/2;
        MergeSortSubVector(data, low, middle);
        MergeSortSubVector(data, middle+1, high);
        Merge(data, low, middle, middle+1, high);
    }
}

// Merge
void SortTool::Merge(vector<int>& data, int low, int middle1, int middle2, int high) {
    // Function : Merge two sorted subvector
    // TODO : Please complete the function
    vector<int> subVec1(middle1-low+2, INT_MAX);
    vector<int> subVec2(high-middle2+2, INT_MAX);

    for(int i=low; i<= high; ++i){
        if( i<=middle1 ){
            subVec1[i-low] = data[i];
        }
        else{
            subVec2[i-middle2] = data[i];
        }
    }

    int idx = low;
    int i = 0;
    int j = 0;
    while(idx <= high){
        if(subVec1[i] < subVec2[j]){
            data[idx++] = subVec1[i++];
        }
        else{
            data[idx++] = subVec2[j++];
        }
    }
    
}

// Heap sort method
void SortTool::HeapSort(vector<int>& data) {
    // Build Max-Heap
    BuildMaxHeap(data);
    // 1. Swap data[0] which is max value and data[i] so that the max value will be in correct location
    // 2. Do max-heapify for data[0]
    for (int i = data.size() - 1; i >= 1; i--) {
        swap(data[0],data[i]);
        //cout << "Swap " << data[0] << " & " << data[i] << endl;
        heapSize--;
        MaxHeapify(data,1);
    }
}

//Max heapify
void SortTool::MaxHeapify(vector<int>& data, int root) {
    // Function : Make tree with given root be a max-heap if both right and left sub-tree are max-heap
    // TODO : Please complete max-heapify code here
    assert(root <= heapSize && root >= 0);
    int left  = root*2;
    int right = left+1;
    int idx;
    if( left <= heapSize && data[root-1] < data[left-1] ){
        idx = left;
    }
    else idx = root;
    if( right <= heapSize && data[idx-1] < data[right-1]){
        idx = right;
    }

    if(idx!=root){
        swap(data[root-1], data[idx-1]);
        MaxHeapify(data, idx);
    }
}

//Build max heap
void SortTool::BuildMaxHeap(vector<int>& data) {
    heapSize = data.size(); // initialize heap size
    // Function : Make input data become a max-heap
    // TODO : Please complete BuildMaxHeap code here
    for (unsigned i = heapSize/2; i>=1; --i){
        MaxHeapify(data, i);
    }
}

