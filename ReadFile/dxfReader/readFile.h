#ifndef __READ_FILE_H__
#define __READ_FILE_H__

#include <fstream>
#include <string>
#include <vector>

#include "EntityCommon.h"

using namespace std;

class Reader {
    vector<float> Zar;
    vector<int> Iar;
    void ReadSize(ifstream& dxfFile, ESpace *pEntSpace, bool min);
    void ReadArc(ifstream& dxfFile, vector<EntRaw>& era, string&dxfValue, ESpace *pEntSpace);
    void ReadCircle(ifstream& dxfFile, vector<EntRaw>& era, string&dxfValue, ESpace *pEntSpace);
    void ReadLine(ifstream& dxfFile, vector<EntRaw>& era, string&dxfValue, ESpace *pEntSpace);
    void ReadEntAdd(ESpace *pEntSpace, vector<float>*pZar, vector<int>*pIar, vector<EntRaw>*pear, EntRaw*pe, float Lz);
    void ReadArrange(ESpace *pEntSpace, vector<float>*pZar, vector<int>*pIar);
public:
    int readDxfFile(char* szFile, ESpace *pEntSpace);
};

#endif