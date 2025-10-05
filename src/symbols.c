#include "ft_nm.h"

extern t_flags  g_flags;

static char *x_(get_section_name)(t_elf_info elf_info, uint16_t index) {
    Elf_Shdr    shdr;
    Elf_Word    sh_name;

    if (index >= elf_info.shcount)
        return NULL;
    shdr = elf_info.shdrs[index];
    sh_name = resolve_endianess(shdr.sh_name);
    if (sh_name >= elf_info.shstrtab.size)
        return NULL;

    return elf_info.shstrtab.content + sh_name;
}

static int  x_(set_sym_name)(t_sym *sym, t_string strtab) {
    if (sym->st_name >= strtab.size)
        return 1;
    if (sym->st_name != 0)
        sym->name = strtab.content + sym->st_name;
    else if (sym->st_shndx == SHN_ABS)
        sym->name = "";
    else if (sym->st_type == STT_SECTION)
        sym->name = sym->sh_name;
    else
        return 1;
    return 0;
}

static void x_(set_sym_value)(t_sym *sym, Elf_Sym symtab) {
    uint_t      st_value;
    const char  *hex_digits = "0123456789abcdef";

    sym->value = NULL;
    if (sym->st_shndx == SHN_UNDEF)
        return ;
    sym->value = ft_calloc(Elf_Addr_len + 1, sizeof(char));
    st_value = resolve_endianess(symtab.st_value);
    sym->value[Elf_Addr_len] = '\0';
    for (int i = Elf_Addr_len - 1; i >= 0; i--) {
        sym->value[i] = hex_digits[st_value & 0xF];
        st_value >>= 4;
    }
}

static void x_(find_section_type)(t_sym *sym, t_elf_info elf_info) {
    Elf64_Word  sh_flags, sh_type;

    if (sym->st_shndx >= SHN_LORESERVE || sym->st_shndx >= elf_info.shcount)
        return;
    if (ft_strncmp(sym->sh_name, ".debug", 6) == 0) {
        sym->type = 'N';
        return;
    }

    sh_flags = resolve_endianess(elf_info.shdrs[sym->st_shndx].sh_flags);
    sh_type = resolve_endianess(elf_info.shdrs[sym->st_shndx].sh_type);

    if (sym->st_type == STT_FUNC || (sh_flags & SHF_EXECINSTR))
        sym->type = 'T';
    else if (sh_type == SHT_NOBITS)
        sym->type = 'B';
    else if (sh_flags & SHF_WRITE)
        sym->type = 'D';
    else if (sh_flags & SHF_ALLOC)
        sym->type = 'R';
    else
        sym->type = 'n';
}

static void x_(find_special_type)(t_sym *sym) {
    switch (sym->st_shndx) {
        case SHN_UNDEF:
            sym->type = 'U'; return;
        case SHN_COMMON:
            sym->type = 'C'; return;
        case SHN_ABS:
            sym->type = 'A'; return;
    }
    if (sym->st_bind == STB_GNU_UNIQUE)
        sym->type = 'u';
    else if (sym->st_type == STT_GNU_IFUNC)
        sym->type = 'i';
}

static void x_(resolve_visibility)(t_sym *sym) {
    if (sym->st_bind == STB_WEAK && sym->st_type != STT_GNU_IFUNC) {
        if (sym->st_type == STT_OBJECT)
            sym->type = (sym->st_shndx == SHN_UNDEF) ? 'v' : 'V';
        else
            sym->type = (sym->st_shndx == SHN_UNDEF) ? 'w' : 'W';
    }
    if (sym->st_bind == STB_LOCAL && sym->type != 'N' && sym->type != 'n')
        sym->type += ('a' - 'A');
}

static void x_(set_sym_type)(t_sym *sym, t_elf_info elf_info) {
    sym->type = '?';
    x_(find_section_type)(sym, elf_info);
    x_(find_special_type)(sym);
    if (sym->type != '?')
        x_(resolve_visibility)(sym);
}

static t_sym    *x_(create_symbol)(Elf_Sym symtab, t_elf_info elf_info) {
    t_sym   *sym;

    sym = malloc(sizeof(t_sym));
    if (sym == NULL)
        return NULL;
    sym->st_bind = ELF_ST_BIND(symtab.st_info);
    sym->st_type = ELF_ST_TYPE(symtab.st_info);
    sym->st_name = resolve_endianess(symtab.st_name);
    sym->st_shndx = resolve_endianess(symtab.st_shndx);
    sym->sh_name = x_(get_section_name)(elf_info, sym->st_shndx);
    if (x_(set_sym_name)(sym, elf_info.symstrtab)) {
        free(sym);
        return NULL;
    }
    x_(set_sym_value)(sym, symtab);
    x_(set_sym_type)(sym, elf_info);

    return sym;
}

t_list *x_(get_symbols)(t_elf_info elf_info) {
    t_list  *sym_list;
    t_sym   *sym;

    sym_list = NULL;
    for (Elf_Word i = 0; i < elf_info.symcount; i++) {
        sym = x_(create_symbol)(elf_info.symtabs[i], elf_info);
        if (sym == NULL)
            continue ;
        ft_list_push_back(&sym_list, sym);
    }

    return sym_list;
}
