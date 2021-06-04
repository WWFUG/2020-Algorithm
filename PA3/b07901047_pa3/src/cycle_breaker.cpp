#include "cycle_breaker.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#define MAX(a,b) a>b?a:b
#define INF 1<<10
#define MAXW 100

int CycleBreaker::globalFlag = 0;

bool CompareEdge(Edge* e1, Edge* e2){
    return e1->w > e2->w;
}


// non-decreasing order
void countingSort(vector<Edge*>& v_i, vector<Edge*>& v_s){
    int countArr[2*MAXW+1] = {0};
    for(unsigned i=0; i<v_i.size(); ++i){
        ++countArr[ v_i[i]->w+MAXW ];
    }
    for(unsigned i=1; i < 2*MAXW+1; ++i){
        countArr[i] += countArr[i-1];
    }
    for(int i=v_i.size()-1; i>=0; --i){
        v_s[ v_i.size() - countArr[v_i[i]->w+MAXW]-- ] = v_i[i];
    }
}


CycleBreaker::CycleBreaker(fstream& fi, unsigned n, unsigned m, bool u): nodeNum(n), edgeNum(m), cycleNum(0){
    undirected = u;
    totalWeight = 0;
    
    if(u){
        subset = Subset(n);
        sortedEdges = vector<Edge*>(m);
    }
    else{
        subset = Subset(n);
        adjMat = new Edge**[n];
        for(unsigned i=0; i<n; ++i){
            adjMat[i] = new Edge*[n];
        }
        adjList.resize(n, list<Edge*>());
        dfsFlag = new unsigned[n];
        dfsExplore = new bool[n];
        predecessor = new int[n];
        resetPredecessor();
    }
    edges.resize(m);

    unsigned x,y;
    int negSum = 0;
    int weight;
    for(unsigned i=0; i<m; ++i){
        fi >> x >> y >> weight;
        if(weight<0) negSum+=weight;
        Edge* e = new Edge(x, y, weight);
        edges[i] = e;
        if(!u){
            adjMat[x][y] = e;
            adjList[x].push_back(e);
            edge2Cycle.insert(std::pair<Edge*, list<unsigned>>(e, list<unsigned>()) );
        }
    }
 
}

void CycleBreaker::breakCycle(){
    if(undirected){
        // counting sort
        Kruskals();
    }
    else{
        cycleNum = 0;
        detectCycle();
        while(cycleNum){
            breakEdge();
            cycleNum = 0;
            detectCycle();
        }
        vector<Edge*> rEdge;
        vector<Edge*> sEdge;
        unsigned j=0;
        for(unsigned k=0; k<nodeNum; ++k){
            for(auto it=adjList[k].begin(); it!=adjList[k].end(); ++it, ++j){
                rEdge.push_back(*it);
            }
        }
        sEdge.resize(j);
        countingSort(rEdge, sEdge);
        for(unsigned k=0; k<sEdge.size(); ++k){
            if( subset.findSetCollapse(sEdge[k]->x)!=subset.findSetCollapse(sEdge[k]->y) ){
                subset.unionSet(sEdge[k]->x, sEdge[k]->y);
            }
            else if(sEdge[k]->w < 0){
                breakedEdge.push_back(sEdge[k]);
            }
        }

    }
}


void CycleBreaker::writeOutput(fstream& fo){
    totalWeight = 0;
    for(unsigned i=0; i<breakedEdge.size(); ++i){
        totalWeight+=breakedEdge[i]->w;
    }
    fo << totalWeight << '\n';
    for(unsigned i=0; i<breakedEdge.size(); ++i){
        fo << breakedEdge[i]->x << " " << breakedEdge[i]->y << " " << breakedEdge[i]->w << '\n';
    }
}

void CycleBreaker::Kruskals(){
    countingSort(edges, sortedEdges);
    for(unsigned i=0; i<edges.size(); ++i){
        if( subset.findSetCollapse(sortedEdges[i]->x)!=subset.findSetCollapse(sortedEdges[i]->y) ){
            subset.unionSet(sortedEdges[i]->x, sortedEdges[i]->y);
        }
        else{
            breakedEdge.push_back(sortedEdges[i]);
            totalWeight += sortedEdges[i]->w;
        }
    }
}


bool CycleBreaker::detectCycle(){
    globalFlag++;
    resetPredecessor();
    //Edge* backEdge = NULL;
    for(unsigned i=0; i<nodeNum; ++i){
        if(dfsFlag[i]!=globalFlag && !dfsExplore[i]){
            //cout << "Starting from source " << i << endl;
            dfsVisit(i);
            //if(backEdge) break;
        }
    }
    return true;
    //breakedEdge.swap(backEdges);
}

void CycleBreaker::dfsVisit(unsigned i){
    dfsFlag[i] = globalFlag;
    //cout << "visit " << i << " ... \n";
    unsigned cnt  = 0;
    for(auto it = adjList[i].begin(); it!=adjList[i].end(); ++it, ++cnt){
        //if(backEdge) return ;
        assert( (*it)->x==i );
        unsigned j = (*it)->y;
        if(dfsFlag[j]!=globalFlag){
            predecessor[j] = i;
            dfsVisit(j);
        }
        else if(!dfsExplore[j]){
            //cout << "Cycle!!" << endl;
            //cout << "Back Edge (" << (*it)->x << "," << (*it)->y << ")" << endl; 
            //backEdge = (*it);
            //return;
            unsigned s = (*it)->y;
            unsigned x = (*it)->x;
            Edge* e = adjMat[ (*it)->x ][ (*it)->y ]; 
            assert(e);
            //cout << "Cycle " << s << " ~> " << x << " ~> " << s << endl;
            //cout << "Pred(" << x << ") = " << predecessor[x] << endl;
            while(predecessor[x]!=s){
                // push back edge->cycle lookup
                e = adjMat[predecessor[x]][x];
                //cout << "Pushing " << cycleNum << endl;
                edge2Cycle[e].push_back(cycleNum);
                x = predecessor[x];
                //cout << "Pred(" << x << ") = " << predecessor[x] << endl;
            }
            e = adjMat[s][x];
            edge2Cycle[e].push_back(cycleNum);
            cycleNum++;
        }
    }
    dfsExplore[i] = true;
}

void CycleBreaker::resetPredecessor(){
    for(unsigned i=0; i<nodeNum; ++i){
        predecessor[i] = -1;
        dfsExplore[i] = false;
    }   
}

void CycleBreaker::breakEdge(){
    // break edges according to e->w/#cycles
    // (1) choose min edge
    // (2) push_back breaked edge, update adjList
    // (3) update map
    cout << "Total " << cycleNum << " cycles"  << endl;
    vector<bool> isBreak(cycleNum, false);
    unsigned breakNum = 0;
    while(1){
        list<unsigned>* delList = NULL;
        Edge* e = NULL;
        float cost = INF;
        for(auto it=edge2Cycle.begin(); it!=edge2Cycle.end(); ++it){
            auto& list = (*it).second;
            if(!list.empty()){
                //cout << (float)(*it).first->w/list.size() << endl;
                if( (float)(*it).first->w/list.size() < cost ){
                    cost = (float)(*it).first->w/list.size();
                    delList = &list;
                    e = (*it).first;
                }
            } 
        }
        if(delList){
            bool needDel = false;
            for(auto it=delList->begin(); it!=delList->end(); ++it){
                unsigned cycle = *it;
                if(isBreak[cycle] == false){
                    needDel = true;
                    breakNum++;
                    isBreak[cycle] = true;
                }
                //cout << "Deleting cycle " << cycle << "..." << endl;
                /*
                for(auto em=edge2Cycle.begin(); em!=edge2Cycle.end(); ++em){
                    auto& list = (*em).second;
                    if(!list.empty() && &list!=delList){
                        for(auto st=list.begin(); st!=list.end(); ++st){
                            if(*st==cycle){
                                list.erase(st);
                                break;
                            }
                        }
                    }
                }*/
                
            }
            
            if(needDel){
                adjList[e->x].remove(e);
                /*
                for(auto it=adjList[e->x].begin(); it!=adjList[e->x].end(); ++it){
                    if(e==*it){
                        auto beg = adjList[e->x].erase(it);
                        break;
                    }  
                }
                */
                breakedEdge.push_back(e);
                //if(breakNum==cycleNum){
                //    return;
            } 
            /*
            for(auto it=adjList[e->x].begin(); it!=adjList[e->x].end(); ++it){
                if(e==*it){
                    adjList[e->x].erase(it);
                    break;
                }  
            }
            */
            delList->clear();
            //breakedEdge.push_back(e);
        }
        else{
            return;
        }
    }
}