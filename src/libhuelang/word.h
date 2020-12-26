#ifndef WORD_H
#define WORD_h

namespace huelang {

    struct Word
    {
        unsigned short major;
        unsigned long minor;
    };
    bool operator==(const Word& a, const Word& b);

}

#endif