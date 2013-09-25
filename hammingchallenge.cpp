#include <vector>
#include <iostream>
#include <cassert>
#include <time.h>
#include <functional>

// Types
// In C++, a vector<bool> is stored as a very efficient bit map.
typedef std::vector<bool> Codeword;
typedef std::vector<Codeword> CodewordList;

// Functions to print these types
std::ostream &operator<<(std::ostream &os, const Codeword &cw) {
    for(unsigned int i = 0; i < cw.size(); ++i) {
        os << (cw[i] ? "1" : "0");
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const CodewordList &cwl) {
    os << "[";
    for(int i = 0; i < cwl.size(); ++i) {
        if(i != 0) os << ",";
        os << cwl[i];
    }
    os << "](" << cwl.size() << ")";
    return os;
}

//! A code word generator to help iterate through all code words
struct CodewordGenerator {
    CodewordGenerator(int bitLength) : codeword(bitLength, 0) {
        assert(bitLength > 0);
    }

    CodewordGenerator(Codeword start) : codeword(start) {
        assert(codeword.size() > 0);
    }

    bool operator++() {
        bool isLast = false;
        incrementCodeword(codeword, isLast);
        return !isLast;
    }

    operator const Codeword&() const {
        return codeword;
    }

    static void incrementCodeword(Codeword &cw, bool &isLast) {
        const unsigned int bitLength = cw.size();

        int firstZero = bitLength - 1;
        for(; firstZero > 0; --firstZero) {
            if(!cw[firstZero]) break;
        }
        if(cw[firstZero]) {
            isLast = true;
            return;
        }
        cw[firstZero] = true;
        for(firstZero++; firstZero < bitLength; ++firstZero) {
            cw[firstZero] = false;
        }
    }

private:
    Codeword codeword;
};

bool hammingDistanceAtLeast(int atLeast, const Codeword &a, const Codeword &b) {
    assert(a.size() == b.size());
    for(unsigned int i = 0; i < a.size(); ++i) {
        if(a[i] != b[i]) atLeast--;
        if(atLeast == 0) return true;
    }
    return false;
}

std::ostream &get_ostream(time_t start_time) {
    time_t time_diff = time(NULL) - start_time;
    std::cout << time_diff << " - ";
    return std::cout;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " bitlength hammingdist" << std::endl;
        return 1;
    }

    const int bitLength = atoi(argv[1]);
    const int hammingDistance = atoi(argv[2]);

    if(bitLength <= 0) {
        std::cerr << "Not satisfied: bit length > 0" << std::endl;
        return 1;
    }
    if(hammingDistance <= 0 || hammingDistance > bitLength) {
        std::cerr << "Not satisfied: 0 < hammingDistance <= bitLength" << std::endl;
        return 1;
    }

    CodewordList longestList;

    const time_t start_time = time(NULL);

    std::function<void(CodewordList)> listCalculator;
    std::vector<bool> level_done_once;

    listCalculator = [&longestList, &bitLength, &hammingDistance, &listCalculator, &level_done_once, &start_time](CodewordList cl) {
        // cl is a list of selected code words with at least given hamming distance
        // for the depth-first search, find the next ones
        CodewordGenerator ci(bitLength);
        if(cl.size() > 0) {
            ci = CodewordGenerator(cl.back());
        }
        do {
            bool enough_distance = true;
            for(Codeword &cw : cl) {
                if(!hammingDistanceAtLeast(hammingDistance, ci, cw)) {
                    enough_distance = false;
                    break;
                }
            }
            if(enough_distance) {
                CodewordList cl_new = cl;
                cl_new.push_back(ci);
                if(longestList.size() < cl_new.size()) {
                    get_ostream(start_time) << "Found a new longest list: " << cl_new << std::endl;
                    longestList = cl_new;
                }
                listCalculator(cl_new);
            }
        } while(++ci);
        const int level = cl.size();
        if(level_done_once.size() <= level)
            level_done_once.resize(level + 1, false);
        if(!level_done_once[level]) {
            get_ostream(start_time) << "Exit level " << level << std::endl;
            level_done_once[level] = true;
        }
    };

    listCalculator(CodewordList());

    get_ostream(start_time) << "Longest list first found: " << longestList << std::endl;
}
