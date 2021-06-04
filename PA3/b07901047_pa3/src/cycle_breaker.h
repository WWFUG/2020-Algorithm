
#ifndef CYCLE_BREAKER_H
#define CYCLE_BREAKER_H
#include <vector>
#include <fstream>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <map>
#include <list>
using namespace std;

struct Edge{
    unsigned x;
    unsigned y;
    int w;
    Edge(unsigned i, unsigned j, int k){
        x = i;
        y = j;
        w = k;
    }
    void print(){
        cout << setw(4) << x << " " <<  setw(4) << y << " " << setw(4) << w;
    }
};

struct Subset{
    int* table;
    unsigned maxN;
    Subset(){};
    Subset(unsigned n){
        maxN = n;
        table = new int[n];
        for(unsigned i=0; i<n ;i++)
            table[i] = -1;
    }

    void free(){
        delete[] table;
    }

    void reset(){
        for(unsigned i=0; i<maxN ;i++)
            table[i] = -1;
    }

    unsigned findSetCollapse(unsigned i){
        assert(i<maxN);
        int root;
        for (root = i; table[root] >= 0; root = table[root]);

        while(i!=root){
            int parent = table[i];
            table[i] = root;
            i = parent;
        }
        return root;
    }

    void unionSet(unsigned x, unsigned y){
        unsigned rx = findSetCollapse(x);
        unsigned ry = findSetCollapse(y);

        if( -table[rx] >= -table[ry]){
            table[rx] += table[ry];
            table[ry] = rx;
        }
        else{
            table[ry] += table[rx];
            table[rx] = ry;
        }
    }

    bool isRoot(unsigned i){
        assert(i<maxN);
        return table[i]<0;
    }
    unsigned predecessor(unsigned i){
        assert(i<maxN);
        if(i<0) return i;
        else    return table[i];
    }
};

class CycleBreaker{
public:
    CycleBreaker(fstream& fi, unsigned n, unsigned m, bool u);
    ~CycleBreaker(){  
        subset.free();
        //sortedEdges.clear();
        //breakedEdge.clear();
        //remainedEdges.clear();
        for(unsigned i=0; i<edges.size(); ++i)
            delete edges[i];
        //free adjMat
        
        if(!undirected){
            for(unsigned i=0; i<nodeNum; ++i)
                delete[] adjMat[i];
            delete[] adjMat;
            delete[] dfsFlag;
            delete[] dfsExplore;
            delete[] predecessor;
        }
    }
    void breakCycle();
    void writeOutput(fstream& f);
private:
    // implement adjList
    Edge***    adjMat;
    map<Edge*, list<unsigned>>   edge2Cycle;
    vector<list<Edge*>>  adjList;
    int        nodeNum;
    int        edgeNum;
    bool       undirected;
    int        totalWeight;// breaked cycles' weight
    //undirected graph Kruskal's algorithm
    Subset          subset; 
    vector<Edge*>   edges; 
    vector<Edge*>   sortedEdges;
    
    //directed
    // remainedEdge needed?
    vector<Edge*>   remainedEdges;
    unsigned*       dfsFlag;
    bool*           dfsExplore;
    int*            predecessor; // -1 means NIL
    static int      globalFlag;

    unsigned        cycleNum;

    
    vector<Edge*>   breakedEdge;
    bool        readEdge(fstream& f, unsigned m);
    void        Kruskals();
    bool        detectCycle();
    void        resetPredecessor();
    void        breakEdge();
    void        dfsVisit(unsigned i);
};

#endif










