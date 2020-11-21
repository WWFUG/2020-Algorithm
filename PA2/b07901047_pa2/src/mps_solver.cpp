#include "mps_solver.h"
#include <string>
#include <iostream>
#include <fstream>

#define MAX(a,b) a>b?a:b



MPSSolver::MPSSolver(fstream& fi){
    if(!readGraph(fi))
        std::cerr << "Error while reading input file!!" << std::endl;
    dpMatrix.max = maxChord;
    dpMatrix.init();
}

bool MPSSolver::readGraph(fstream& f){
    assert(f);
    unsigned buf1, buf2;
    f >> buf1;
    maxChord = buf1/2;
    assert(maxChord <= MAX_CHORD_NUM);
    //cout << maxChord << endl;
    chordLut.lut = new unsigned[maxChord*2];

    for(unsigned i=0;i<maxChord;++i){
        f >> buf1;
        f >> buf2;
        chordLut.addChord(buf1, buf2);
        // cout << chordLut[ stoi(buffer2) ] << " " << chordLut[stoi(buffer1)] << endl;
    }
    return true;
}

unsigned MPSSolver::solve(fstream& fo){
    //init dp matrix
    assert(fo);
    cout << "Solving ..." << endl;
    unsigned mps = topDownSolve(0, maxChord*2-1);
    fo << mps << '\n';
    cout << "Output solution ..." << endl;
    writeOutput(0, maxChord*2-1, fo);
    return mps;
}

unsigned MPSSolver::topDownSolve(unsigned i, unsigned j){
    if(i==j){
        dpMatrix.set(i, j, 0);
        return 0;
    }
    if( !dpMatrix.isUndef(i, j) )
        return dpMatrix.get(i, j);

    unsigned end = chordLut[j];
    if( end==i ) {
        dpMatrix.set(i, j, topDownSolve(i+1, j-1)+1 );
    }
    else if( end>i && end<j ){
        dpMatrix.set(i, j, MAX( topDownSolve(i, j-1),
                                topDownSolve(i, end-1) + topDownSolve(end+1, j-1)+1 ) );
    }
    else{
        dpMatrix.set(i, j, topDownSolve(i, j-1) );
    }
    //cout << i << " " << j << " " << dpMatrix[i][j-i] << endl;
    return dpMatrix.get(i, j);
}

void MPSSolver::writeOutput(unsigned i, unsigned j, fstream& fo){
    if(i==j) return;

    unsigned end = chordLut[j];
    if( end==i ){
        writeOutput(i+1, j-1, fo);
        fo << j << ' ' << end << '\n';
    }
    else if( end>i && end<j ){
        if( dpMatrix.get(i, j-1) >= dpMatrix.get(i, end-1)+dpMatrix.get(end+1, j-1)+1 ){
            writeOutput(i, j-1, fo);
        }
        else{
            writeOutput(i, end-1, fo);
            writeOutput(end+1, j-1, fo);
            fo << j << ' ' << end << '\n';
        }
    }
    else{
        writeOutput(i, j-1, fo);
    }
}


