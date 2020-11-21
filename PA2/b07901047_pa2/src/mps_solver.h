#include <vector>
#include <fstream>
#include <cassert>
#include <iostream>
using namespace std;

#define MAX_CHORD_NUM 90000
#define MAX_STR_SIZE 100
static unsigned MAX_USHORT =  (1<<16)-1; // 65535
static unsigned MAX_UINT = 4294967295;

struct ChordLookup{
    unsigned* lut;
    // chort[idx] = {msb1[idx],msb2[idx], lut[idx]}
    void addChord(unsigned p1, unsigned p2){
        lut[p1] = p2;
        lut[p2] = p1;
    }
    unsigned operator[](unsigned idx){
        return (unsigned)lut[idx];
    }
};

struct DpMatrix{
    ushort** lsb_16;
    bool**  msb;
    unsigned max;
    void init(){
        lsb_16 = new ushort* [max*2];   
        msb = new bool* [max*2];
        for(unsigned i=0; i<max*2; ++i){
            lsb_16[i] = new ushort[max*2-i];// dpMatrix[i][j] i<j
            msb[i] = new bool[max*2-i];
            for(unsigned j=0; j<max*2-i; ++j){
                lsb_16[i][j] = MAX_UINT;
                msb[i][j] = 1;
            }
        }
    }
    unsigned get(unsigned i, unsigned j){
        return (unsigned)(msb[i][j-i]<<16) + (unsigned)(lsb_16[i][j-i]); 
    }
    void set(unsigned i, unsigned j, unsigned val){
        //cout << "Setting... " << val << endl;   
        if(val<MAX_USHORT)
            msb[i][j-i] = 0;
        lsb_16[i][j-i] = (ushort)(val);
        //cout << msb[i][j-i] << ", " << lsb_16[i][j-i] << endl;
    }
    bool isUndef(unsigned i, unsigned j){
        return get(i,j) > max;
    }
    bool clear(){
        for(unsigned i=0; i<max*2; ++i){
            delete[] lsb_16[i];
            delete[] msb[i];
        }
        delete[] msb;
        delete[] lsb_16;
    }
};


class MPSSolver{
public:
    MPSSolver(fstream& fi);
    unsigned solve(fstream& fo);
    ~MPSSolver(){ 
        delete chordLut.lut;
        dpMatrix.clear();   
    }
private:
    unsigned    maxChord;
    ChordLookup chordLut;
    DpMatrix    dpMatrix;  
    bool        readGraph(fstream& f);
    unsigned    topDownSolve(unsigned i, unsigned j);
    void        writeOutput(unsigned i, unsigned j, fstream& f);
};












