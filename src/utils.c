#include "ft_nm.h"

extern t_elf_file   g_elf_file;
extern t_flags      g_flags;

size_t    ft_strlen(char *str) {
    size_t i;

    i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

int ft_strcmp(char *s1, char *s2) {
    int i;

    for (i = 0; s1[i]; i++) {
        if (s1[i] > s2[i] || !s2[i])
            return 1;
        else if (s1[i] < s2[i])
            return -1;
    }
    if (s2[i])
        return -1;
    return 0;
}

void    sort_symbols(t_sym_info *sym_info_arr, size_t n_sym) {
    t_sym_info  temp;
    int         diff;

    for (size_t i = 0; i < n_sym - 1; i++) {
        for (size_t j = 0; j < n_sym - i - 1; j++) {
            diff = ft_strcmp(sym_info_arr[j].name, sym_info_arr[j + 1].name);
            if ((diff > 0 && !g_flags.reverse) || (diff < 0 && g_flags.reverse)) {
                temp = sym_info_arr[j];
                sym_info_arr[j] = sym_info_arr[j + 1];
                sym_info_arr[j + 1] = temp;
            }
        }
    }
}

char *ft_strchr(char *str, int search_str) {
    while (*str != '\0') {
        if (*str == search_str)
            return str;
        str++;
    }
    return NULL;
}

void    display_symbols(t_sym_info *sym_info_arr, size_t n_sym) {
    static char *undefined_types = "Uw";
    static char *external_types = "BCDGRSTUWw";
    t_sym_info  sym_info;

    if (g_flags.path) {
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, g_elf_file.path, ft_strlen(g_elf_file.path));
        write(STDOUT_FILENO, ":\n", 2);
    }
    for (size_t i = 0; i < n_sym; i++) {
        sym_info = sym_info_arr[i];
        if (g_flags.undefined && !ft_strchr(undefined_types, sym_info.type))
            continue ;
        if (g_flags.external && !ft_strchr(external_types, sym_info.type))
            continue ;
        write(STDOUT_FILENO, sym_info.value, ft_strlen(sym_info.value));
        write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, &sym_info.type, 1);
        write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, sym_info.name, ft_strlen(sym_info.name));
        write(STDOUT_FILENO, "\n", 1);
    }
}

void    free_symbols(t_sym_info *sym_info_arr, size_t n_sym) {
    for (size_t i = 0; i < n_sym; i++)
        free(sym_info_arr[i].value);
    free(sym_info_arr);
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

// void   *swap_endian_if_needed(void *original, size_t data_size) {
//     const uint8_t   *original_bytes;
//     uint8_t         *result;
// 
//     if (g_elf_file.endian == ELFDATA2MSB)
//         return original;
// 
//     original_bytes = (const uint8_t *)original;
//     result = malloc(data_size);
//     if (!result)
//         return NULL;
// 
//     for (size_t i = 0; i < data_size; i++)
//         result[i] = original_bytes[data_size - i - 1];
// 
//     return result;
// }
