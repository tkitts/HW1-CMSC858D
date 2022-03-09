#include "../include/compact_vector.hpp"
#include "../include/compact_iterator.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "../select_support/select_support.cpp"
#include <stdlib.h>     /* srand, rand */
#include <chrono>
using namespace std;
class sparseArray{
    vector<string> values;
    Select_support sSup;
    public:
        sparseArray(){ 
            create(0);
        }
    void create(uint64_t size){
        sSup.r.b.resize(size);
        Rank_support rSup;
        rSup.construct(sSup.r.b);
        sSup.construct(rSup);
    }
    void append(string elem, uint64_t pos){
        if (pos < sSup.r.b.size()){
            sSup.r.b[pos] = 1;
            values.push_back(elem);
            //then update the support tables
            sSup.r.copy(Rank_support(sSup.r.b));
        }
    }
    bool get_at_rank(uint64_t r, std::string& elem){
        if(r < values.size()){
            elem = values[r];
            return true;
        }
        else{
            return false;
        }
    }
    bool get_at_index(uint64_t r, std::string& elem){
        if(r < sSup.r.b.size()){
            if(sSup.r.b[r] == 1){
                elem = values[sSup.r.rank1(r)-1];
                return true;
            }
        }
        return false;
    }
    uint64_t num_elem_at(uint64_t r){
        if(r < sSup.r.b.size()){
            return sSup.r.rank1(r);
        }
        else{
            return sSup.r.rank1(sSup.r.b.size()-1);
        }
    }
    uint64_t size(){
        return sSup.r.b.size();
    }
    uint64_t num_elem(){
        return values.size();
    }
    void save(string& fname){
        auto size = sSup.r.b.size();
        sSup.r.save(fname);
        ofstream myfile;
        myfile.open(fname, ios::app);
        uint64_t valSize = values.size();
        uint64_t eleSize;
        myfile.write(reinterpret_cast<char *>(&valSize), sizeof(uint64_t));
        for(int i=0;i<values.size();i++){
            eleSize = (values[i]).length();
            myfile.write(reinterpret_cast<char *>(&eleSize), sizeof(uint64_t));

            myfile.write(values[i].c_str(), values[i].size());
        }

        myfile.close();
    }
    void load(string& fname){
        uint64_t valSize;
        uint64_t eleSize;
        int bitVSize;
        char * element;
        values.clear();

        compact::vector<uint64_t> r1New{1};
        compact::vector<uint64_t> r2New{1};
        ifstream ifile(fname, std::ios::binary);
        uint64_t bits_per_element = compact::get_bits_per_element(fname);
        uint64_t bits_per_table2 = log2(pow(bits_per_element,2));
        r1New.set_m_bits(bits_per_element);
        r1New.deserialize(ifile);

        sSup.r.b.set_m_bits(1);
        sSup.r.b.deserialize(ifile);
        r2New.set_m_bits(bits_per_table2);
        r2New.deserialize(ifile);

        sSup.r.r1.set_m_bits(bits_per_element);
        sSup.r.r1 = r1New;
        sSup.r.r2.set_m_bits(bits_per_table2);
        sSup.r.r2 = r2New;

        ifile.read(reinterpret_cast<char *>(&valSize),sizeof(uint64_t));
        for(int i=0;i<valSize;i++){
            ifile.read(reinterpret_cast<char *>(&eleSize), sizeof(uint64_t));
            element = new char[eleSize];
            ifile.read(element, eleSize);
            values.push_back(string(element, eleSize));
            delete[] element;
        }
        
        ifile.close();
    }
};
int main(){
    compact::vector<uint64_t> bits{1};
    bits.assign({1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0 , 1, 1, 1, 1, 1, 1 , 1,1 ,1 ,1 ,1 ,1 , 1,1 ,1 });//33 long
    Rank_support rSup(bits);
    sparseArray sA = sparseArray();
    sA.create(16);
    string fname = "saveF.txt";
    string fname2 = "saveF2.txt";
    rSup.save(fname);
    Rank_support rS2 = Rank_support();
    rS2.load(fname);
    ifstream ifs(fname2, std::ios::binary);
    sA.append("bok", 0);
    Select_support sSup(rS2);
    int a = sSup.select1(9);
    sA.append("as", 12);
    sA.append("pre end", 14);
    sA.append("end", 15);
    string elem;
    string file3 = "file3.txt";
    sA.save(file3);
    sparseArray sA2 = sparseArray();
    sA2.load(file3);
    cout << sA2.get_at_index(14, elem) << endl;
    cout << sA2.get_at_rank(1,elem) << endl;
    cout << elem << endl;
    /*compact::vector<uint64_t> bits{1};
    time_t timer;
    srand (time(NULL));
    long len = pow(rand(), 1.8);
    for(long i=0;i<len;i++){
        bits.push_back(rand() %2);
    }
    double seconds;
    Rank_support rSup(bits);
    Select_support sSup(rSup);
    auto start = std::chrono::steady_clock::now();
    for(int i=0;i<10000;i++){
        sSup.select1(rand() % bits.size());
    }
    auto end = std::chrono::steady_clock::now();
    seconds = (end-start).count();

    cout << len << endl;
    cout << sSup.overhead() << endl;
    cout << seconds/1000000000 << endl;*/
}