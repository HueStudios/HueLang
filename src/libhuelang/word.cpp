#include "word.h"

namespace huelang {

    bool operator==(const Word& a, const Word& b) {
        return (a.major == b.major && a.minor == b.minor);
    }    

}