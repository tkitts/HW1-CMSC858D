Variables:
    values
        this variable is a vector<string> used to store elements in the sparseArray
    sSup
        This variable is a Select_support used to search elements in the sparseArray


Constructors:
    Select_support()
        Generates an empty Select_support. select1, overhead or save
        will not work until you call construct(rank support) on the new Select_support, passing in a Rank_support that has a bitvector

    Select_support(Rank_support &input)
        Calls construct(input) to generate the Rank_support needed to perform select1 on that inputVect
Functions:
    void create(uint64_t size) : 
        Creates an empty sparse array of length size.
    void append(string elem, uint64_t pos) : 
        Appends the element elem at index pos of the sparse array.
    bool get_at_rank(uint64_t r, std::string& elem) : 
        This function places a reference to the r-th present item in the array in the reference elem. It returns true if there was >= r items in the sparse array and false otherwise.
    bool get_at_index(uint64_t r, std::string& elem): 
        This function looks at the r-th index in the sparse bitvector; if that bit is 1, it fetches the corresponding value and binds it to the reference elem and returns true, if that bit is a 0, it simply returns false.
    uint64_t num_elem_at(uint64_t r): 
        This function returns the count of present elements (1s in the bit vector) up to and including index r.
    uint64_t size() : 
        Returns the size of the sparse array.
    uint64_t num_elem() : 
        Returns the number of present elements in the sparse array (i.e. the number of 1s in the bitvector).
    save(string& fname) : 
        Saves the sparse array to the file fname.
    void load(string& fname) : 
        Loads the sparse array from the file fname.
CITATION:
    Files in the include folder(compact_vector, compact_iterator, const_iterator_traits, parallel_iterator_traits, prefetch_iterator_traits) are from
    <gmarcais, dnbaker and benrobby> (<3/10/2022>) <compact_vector> (release/v0.1.0). https://github.com/gmarcais/compact_vector