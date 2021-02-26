#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../libhuelang/environment.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace boost::algorithm;
using namespace huelang;
using namespace std;

class Interpreter {
    public:
        Environment env;
        void InterpretLine(string line);
};

#endif