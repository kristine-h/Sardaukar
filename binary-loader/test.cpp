#include "loader.h"

int main(int argc, char* argv[]) {
    Binary bin;
    std::string fname;
    if (argc < 2) {
        printf("Usage: %s <binary>\n", argv[0]);
        return 1;
    }
    fname.assign(argv[1]);
    if (load_binary(fname, &bin, Binary::BIN_TYPE_AUTO) < 0) {
        return 1;
    }
    printf("loaded binary '%s'\n%s/%s (%u bits) \nentry@0x%016jx\n"
            "%s endian \nstatic symbol table contains %lu symbols\n"
            "dynamic symbol table contains %lu symbols\n", 
            bin.filename.c_str(),
            bin.type_str.c_str(),
            bin.arch_str.c_str(), 
            bin.bits, 
            bin.entry, 
            bin.endian_str.c_str(), 
            bin.symcount, 
            bin.dynsymcount);
    return 0;
}