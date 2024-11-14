#ifndef PNG_INIT_H
#define PNG_INIT_H

#include <fstream>

using namespace std;

class png_t {
public:
    void init(ofstream* file);

private:
    ofstream* m_file = nullptr;

    void png_structure();
};



#endif //PNG_INIT_H
