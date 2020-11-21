// **************************************************************************
//  File       [main.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The main program of 2019 fall Algorithm PA1]
//  Modify     [2020/9/15 Mu-Ting Wu]
// **************************************************************************

#include <cstring>
#include <iostream>
#include <fstream>
#include "../lib/tm_usage.h"
#include "mps_solver.h"

using namespace std;
/*
void help_message() {
    cout << "usage: NTU_sort -[IS|MS|QS|HS] <input_file> <output_file>" << endl;
    cout << "options:" << endl;
    cout << "   IS - Insersion Sort" << endl;
    cout << "   MS - Merge Sort" << endl;
    cout << "   QS - Quick Sort" << endl;
    cout << "   HS - Heap Sort" << endl;
}
*/

int main(int argc, char* argv[])
{
    if(argc != 3) {
       //help_message();
       return 0;
    }
    
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;

    //////////// read the input file /////////////
    
    char buffer[200];
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);
    /* testing dpMatrix
    DpMatrix dp;
    dp.max = 5;
    dp.init();
    dp.set( 0,1, (unsigned)(1<<16)+(unsigned)MAX_USHORT );
    dp.set(0, 2, 90000);
    cout << dp.get(0, 1) << endl;
    cout << dp.get(0, 2) << endl;
    */

    //////////// the sorting part ////////////////
    tmusg.periodStart();
    MPSSolver mps = MPSSolver(fin);
    cout << "MPS: " << mps.solve(fout) << endl; 
    //ortTool NTUSortTool; 

    /*
    if(!strcmp(argv[1],"-QS")) {
        NTUSortTool.QuickSort(data);
    }
    else if(!strcmp(argv[1],"-IS")) {
        NTUSortTool.InsertionSort(data);
    }
    else if(!strcmp(argv[1],"-MS")) {
        NTUSortTool.MergeSort(data);
    }
    else if(!strcmp(argv[1],"-HS")) {
        NTUSortTool.HeapSort(data);
    }
    else {
        help_message();
        return 0;
    }
    */

    tmusg.getPeriodUsage(stat);
    cout <<"The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms" << endl;
    cout <<"memory: " << stat.vmPeak << "KB" << endl; // print peak memory

    //////////// write the output file ///////////
    /*
    fout << "# " << data.size() << " data points" <<endl;
    fout << "# index number" << endl;
    for (int i = 0; i < data.size(); i++)
        fout << i << " " <<data[i] << endl;
    */
    fin.close();
    fout.close();
    return 0;

}
