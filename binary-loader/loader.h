#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <vector>

class Binary {
public:
    enum BinaryType {
        BIN_TYPE_AUTO = 0,
        BIN_TYPE_ELF = 1,
        BIN_TYPE_PE = 2
    };
    enum BinaryArch {
        ARCH_NONE = 0,
        ARCH_X86 = 1
    };
    enum Endian {
        ENDIAN_NONE = 0,
        LITTLE = 1,
        BIG = 2
    };

public:
    Binary() : type{BIN_TYPE_AUTO}, arch{ARCH_NONE}, bits{0}, endian{ENDIAN_NONE}, entry{0}, symcount{0}, dynsymcount{0} {}

public:
    std::string         filename;
    BinaryType          type;
    std::string         type_str;
    BinaryArch          arch;
    std::string         arch_str;
    unsigned            bits; 
    Endian              endian;
    std::string         endian_str;
    uint64_t            entry;
    unsigned long       symcount;
    unsigned long       dynsymcount;
};

int load_binary(std::string& fname, Binary* bin, Binary::BinaryType type);

#endif // LOADER_H