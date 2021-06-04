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
#include "cycle_breaker.h"

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
    unsigned n, m;
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);

    tmusg.periodStart();
    fin >> buffer;
    bool u=false;
    if(buffer[0]=='u' && buffer[1]=='\0'){
        cout << "Undirected Graph" << endl;
        u = true;
    }
    else if(buffer[0]=='d' && buffer[1]=='\0'){
        cout << "Directed Graph" << endl;
    }
    else{
        cerr << "Error of input graph type!!" << endl;
    }

    fin >> n >> m;
    cout << "#Node: " << n << "\n#Edge: " << m << endl;

    CycleBreaker cb = CycleBreaker(fin, n, m, u);
    cb.breakCycle();
    cb.writeOutput(fout);
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
