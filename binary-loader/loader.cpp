#include <bfd.h>
#include <stdlib.h>

#include "loader.h"

static bfd* open_bfd(std::string& fname);
static int load_binary_bfd(std::string& fname, Binary* bin, Binary::BinaryType type);

int load_binary(std::string& fname, Binary* bin, Binary::BinaryType type) {
    return load_binary_bfd(fname, bin, type);
}

static bfd* open_bfd(std::string& fname) {
    static int bfd_inited = 0;
    bfd* abfd;
    if (!bfd_inited) {
        bfd_init();
        bfd_inited = 1;
    }
    abfd = bfd_openr(fname.c_str(), nullptr);
    if (!abfd) {
        fprintf(stderr, "failed to open binary '%s' (%s)\n", fname.c_str(), bfd_errmsg(bfd_get_error()));
        return nullptr;
    }
    if (!bfd_check_format(abfd, bfd_object)) {
        fprintf(stderr, "file '%s' does not look like an executable (%s)\n", fname.c_str(), bfd_errmsg(bfd_get_error()));
        return nullptr;
    }
    bfd_set_error(bfd_error_no_error);
    if (bfd_get_flavour(abfd) == bfd_target_unknown_flavour) {
        fprintf(stderr, "unrecognized format for binary '%s' (%s)\n", fname.c_str(), bfd_errmsg(bfd_get_error()));
        return nullptr;
    }
    return abfd;
}

static int load_binary_bfd(std::string& fname, Binary* bin, Binary::BinaryType type) {
    bfd* abfd = nullptr;
    const bfd_arch_info_type* bfd_info;

    abfd = open_bfd(fname);
    if (!abfd) {
        bfd_close(abfd);
        return -1;
    }
    bin->filename = std::string(fname);
    bin->entry = bfd_get_start_address(abfd);
    bin->symcount = bfd_canonicalize_symtab(abfd, nullptr);
    bin->dynsymcount = bfd_canonicalize_dynamic_symtab(abfd, nullptr);
    bin->type_str = std::string(abfd->xvec->name);
    bin->endian = bfd_big_endian(abfd) ? Binary::Endian::BIG : Binary::Endian::LITTLE;
    switch(bin->endian) {
        case Binary::Endian::LITTLE:
            bin->endian_str.assign("little");
            break;
        case Binary::Endian::BIG:
            bin->endian_str.assign("big");
            break;
        default:
            bin->endian_str.assign("none");
    }
    switch(abfd->xvec->flavour) {
        case bfd_target_elf_flavour:
            bin->type = Binary::BIN_TYPE_ELF;
            break;
        case bfd_target_coff_flavour:
            bin->type = Binary::BIN_TYPE_PE;
            break;
        case bfd_target_unknown_flavour:
        default:
            fprintf(stderr, "unsupported binary type (%s)\n", abfd->xvec->name);
            bfd_close(abfd);
            return -1;
    }
    bfd_info = bfd_get_arch_info(abfd);
    bin->arch_str = std::string(bfd_info->printable_name);
    switch(bfd_info->mach) {
        case bfd_mach_i386_i386:
            bin->arch = Binary::BinaryArch::ARCH_X86;
            bin->bits = 32;
            break;
        case bfd_mach_x86_64:
            bin->arch = Binary::BinaryArch::ARCH_X86;
            bin->bits = 64;
            break;
        default:
            fprintf(stderr, "unsupported architecture (%s)\n", bfd_info->printable_name);
            bfd_close(abfd);
            return -1;
    }
    return 1;
}
