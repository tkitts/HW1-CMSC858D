Variables:
    b
        this variable is a compact::vector copied from a input vector during a constructor or construct() function to represent a bit vector
    r1
        This variable is a compact::vector which stores the super blocks used in rank1
    r2
        This variable is a compact::vector which stores the blocks used in rank1


Constructors:
    Rank_support()
        Generates an empty Rank_support. rank1, overhead or save
        will not work until you call construct(bitvector) on the new Rank_support, passing in a compact::vector<uint64_t>{1}

    Rank_support(compact::vector<uint64_t> &inputVect)
        Calls construct(inputVect) to generate the tables needed to perform rank1 on that inputVect
Functions:
    void copy(Rank_support in)
        Copies the bitvector and tables from the Rank_support in to this Rank_support

    void construct(compact::vector<uint64_t> &inputVect)
        Sets b to a copy of inputVect then computes r1 and r2 (the tables needed for performing rank1)

    uint64_t rank1(uint64_t i)
        Returns the number of 1s in the underlying bit-vector up to position i (inclusive).
    uint64_t overhead()
        Returns the size of the rank data structure (in bits) that is required to support constant-time rank on the current bitvector.
    void save(string& fname)
        Saves the rank data structure for this bit vector to the file fname. This rank data structure includes the bit vector.
    void load(string& fname)
        Loads the rank data structure for this bit vector from the file fname. This rank data structure includes the bit vector.
GITHUB LINK:
    https://github.com/tkitts/HW1-CMSC858D
CITATION:
    Files in the include folder(compact_vector, compact_iterator, const_iterator_traits, parallel_iterator_traits, prefetch_iterator_traits) are from
    <gmarcais, dnbaker and benrobby> (<3/10/2022>) <compact_vector> (release/v0.1.0). https://github.com/gmarcais/compact_vector