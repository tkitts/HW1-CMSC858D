#include "../include/compact_vector.hpp"
#include "../include/compact_iterator.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "../rank_support/rank_support.cpp"
using namespace std;
class Select_support{
    public:
        Rank_support r;
        Select_support(){
        }
        Select_support(Rank_support &input){
            construct(input);
        }
    //this method sets the rank support
    void construct(Rank_support &input){
        r.copy(input);
    }
    uint64_t select1(uint64_t i){
        int bot = 0;
        int top = (((r).b)).size()-1;
        int cur = (bot+top)/2;
        int curR = (r).rank1(cur);
        while (curR != i){
            if(top < bot){
                return -1;
            }

            if(curR > i){
                top = cur-1;
            }
            else{
                bot = cur+1;
            }
            cur = (bot+top)/2;
            curR = (r).rank1(cur);
        }
        return cur;
    }
    //select support has no other variables to store other than the rank_support structure, so its just an additional pointer to store
    uint64_t overhead(){
        return sizeof(*this)+(r).overhead();
    }
    void save(string& fname){
        (r).save(fname);
    }
    void load(string& fname){
        (r).load(fname);
    }
};