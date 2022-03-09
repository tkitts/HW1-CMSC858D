#include "../include/compact_vector.hpp"
#include "../include/compact_iterator.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;
namespace masks {
    static constexpr const uint64_t lo_set[65] = {0U, 1U, 3U, 7U, 15U, 31U, 63U, 127U, 255U, 511U, 1023U, 2047U, 4095U, 8191U,
                                               16383U, 32767U, 65535U, 131071U, 262143U, 524287U, 1048575U, 2097151U, 4194303U,
                                               8388607U, 16777215U, 33554431U, 67108863U, 134217727U, 268435455U, 536870911U,
                                               1073741823U, 2147483647U, 4294967295U, 8589934591U, 17179869183U, 34359738367U,
                                               68719476735U, 137438953471U, 274877906943U, 549755813887U, 1099511627775U,
                                               2199023255551U, 4398046511103U, 8796093022207U, 17592186044415U,
                                               35184372088831U, 70368744177663U, 140737488355327U, 281474976710655U,
                                               562949953421311U, 1125899906842623U, 2251799813685247U, 4503599627370495U,
                                               9007199254740991U, 18014398509481983U, 36028797018963967U, 72057594037927935U,
                                               144115188075855871U, 288230376151711743U, 576460752303423487U,
                                               1152921504606846975U, 2305843009213693951U, 4611686018427387903U,
                                               9223372036854775807U, 18446744073709551615U};
}
 class Rank_support {
    public:
        compact::vector<uint64_t> b{1};
        compact::vector<uint64_t> r1{1};
        compact::vector<uint64_t> r2{1};
        Rank_support(){

        }
        Rank_support(compact::vector<uint64_t> &inputVect){
            construct(inputVect);
        }
    //this method constructs the rank support tables based on an input bit vector
    void construct(compact::vector<uint64_t> &inputVect){
            b = inputVect;
            if((b).size() == 0){
                return;
            }
            uint64_t sizeR2 = log2((b).size());//these are the length of bits taken from the original vector
            uint64_t sizeR1 = pow(sizeR2,2);
            uint64_t R1bitSize = log2((b).size());//these are the number of bits required in each block of the respective table
            uint64_t R2bitSize = log2(sizeR1);
            uint64_t numR1s = (b).size()/sizeR1 + ((b).size() % sizeR1 != 0 ? 1:0);
            uint64_t numR2s = (b).size()/sizeR2 + ((b).size() % sizeR2 != 0 ? 1:0); //i have to give it the exact number of elements or it allocates extra space in capacity
            r1.clear();
            r2.clear();
            r1.set_m_bits(R1bitSize);
            r2.set_m_bits(R2bitSize);
            (r1).resize(numR1s);
            (r2).resize(numR2s);
            
            unsigned int rank = 0;
            //construct the tables as compact vectors
            for (int i=0;i<(b).size();i+=sizeR1){
                //for bit in rank add it to r1
                (r1)[i/sizeR1] = rank;
                for(int k=0; k<sizeR1 && i+k<(b).size();k++){
                    if ((b)[i+k] == 1) {
                        ++rank;
                    }
                }
            }
            //construct the second table of blocks
            rank = 0;
            for (int i=0;i<(b).size();i+=sizeR2){
                if(i%sizeR1 == 0){//if we've come to the end of a table 1 block reset the rank
                    rank = 0;
                }
                (r2)[i/sizeR2] = rank;
                for(int k=0; k<sizeR2 && i+k < (b).size();k++){
                    if ((b)[i+k] == 1) {
                        ++rank;
                    }
                }

            }
    }
    void copy(Rank_support in){
        b.set_m_bits(1);
        b = in.b;
        r1.set_m_bits(in.r1.bits());
        r1 = in.r1;
        r2.set_m_bits(in.r2.bits());
        r2 = in.r2;
    }
    uint64_t rank1(uint64_t i){
        uint64_t sizeR1 = pow((int) log2((b).size()),2);
        uint64_t sizeR2 = log2((b).size());
        uint64_t r3;
        uint64_t offset = i-i%sizeR2;
        uint64_t size = i%sizeR2 +1;
        uint64_t a = (*((b).get()) >> (offset));
        if(offset+size>64){
            r3 = a | ((*((b).get()+1) & masks::lo_set[(offset+size)&0x3F]) << (64-offset));
        }
        else{
            r3 = a & masks::lo_set[size];
        }
        //cout << sizeR1 << endl;
        return ((r1)[i/sizeR1] + (r2)[i/sizeR2] + __builtin_popcount(r3));
    }
    //sizeof(*this) does not include the dynamic length vectors, so the size of the actual table contents are added here
    uint64_t overhead(){
        return sizeof(*this)+r1.size()*r1.bits()+r2.size()*r2.bits()+b.bits();
    }
    void save(string& fname){
        ofstream myfile(fname);
        (r1).serialize(myfile);
        b.serialize(myfile);
        (r2).serialize(myfile);
        myfile.close();
    }
    void load(string& fname){
        compact::vector<uint64_t> r1New{1};
        compact::vector<uint64_t> r2New{1};
        ifstream ifile(fname, std::ios::binary);
        uint64_t bits_per_element = compact::get_bits_per_element(fname);
        uint64_t bits_per_table2 = log2(pow(bits_per_element,2));
        r1New.set_m_bits(bits_per_element);
        r1New.deserialize(ifile);

        b.set_m_bits(1);
        b.deserialize(ifile);
        r2New.set_m_bits(bits_per_table2);
        r2New.deserialize(ifile);
        r1.set_m_bits(bits_per_element);
        r1 = r1New;
        r2.set_m_bits(bits_per_table2);
        r2 = r2New;
        ifile.close();
    }
};