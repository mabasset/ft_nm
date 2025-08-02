#include "ft_nm.h"

#include "ft_nm.h"

static int is_little_endian(int file_endianess) {
    return file_endianess == ELFDATA2LSB;
}

static int is_big_endian(int file_endianess) {
    return file_endianess == ELFDATA2MSB;
}

static int (*resolve_endianess_checker(void))(int) {
    int i = 1;
    if (*(char *)&i == 1)
        return is_little_endian;
    return is_big_endian;
}

int define_endianess(int file_endianess) __attribute__((ifunc("resolve_endianess_checker")));

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
