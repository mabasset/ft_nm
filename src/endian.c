#include "ft_nm.h"

extern t_elf_file   g_elf_file;
extern t_flags      g_flags;

int define_endianess(int file_endianess) {
    int i, local_endianess;

    i = 1;
    local_endianess = (*(char *)&i == 1) ? ELFDATA2LSB : ELFDATA2MSB;
    return (local_endianess == file_endianess) ? 1 : 0;
}

uint16_t    bswap_16(uint16_t val) {
    return (val >> 8) | (val << 8);
}

uint32_t    bswap_32(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0x00FF00FF);
    return (val << 16) | (val >> 16);
}

uint64_t    bswap_64(uint64_t val) {
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
    return (val << 32) | (val >> 32);
}
