Variables:
    r
        This variable is a Rank_support


Constructors:
    Select_support()
        Generates an empty Select_support. select1, overhead or save
        will not work until you call construct(rank support) on the new Select_support, passing in a Rank_support that has a bitvector

    Select_support(Rank_support &input)
        Calls construct(input) to generate the Rank_support needed to perform select1 on that inputVect
Functions:
    void construct(Rank_support &input)
        Sets b to a copy of inputVect then computes r1 and r2 (the tables needed for performing rank1)

    uint64_t select1(uint64_t i)
        Returns the position, in the underlying bit-vector, of the ith 1.
    uint64_t overhead()
        Returns the size of the select data structure (in bits) that is required to support log-time select on the current bitvector.
    void save(string& fname)
        Saves the select data structure for this bit vector to the file fname. This includes the Rank_support and bitvector.
    void load(string& fname)
        Loads the select data structure for this bit vector from the file fname. This includes the Rank_support and bitvector.
GITHUB LINK:
    https://github.com/tkitts/HW1-CMSC858D
CITATION:
    Files in the include folder(compact_vector, compact_iterator, const_iterator_traits, parallel_iterator_traits, prefetch_iterator_traits) are from
    <gmarcais, dnbaker and benrobby> (<3/10/2022>) <compact_vector> (release/v0.1.0). https://github.com/gmarcais/compact_vector