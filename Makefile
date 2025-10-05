NAME = ft_nm

S_DIR = src/
O_DIR = obj/
L_DIR = libmb/

ALL_SRCS = $(wildcard $(S_DIR)*.c)
SRCS_32 = $(elf_info.c symbols.c)

OBJ = $(ALL_SRCS:.c=.o)
OBJ_32 = $(addprefix $(S_DIR)32_, $(SRCS_32:.c=.o))

HDR = $(addprefix $(S_DIR), ft_nm.h)
LIB = $(addprefix $(L_DIR), libmb.a)

CFLAGS = -Wall -Wextra -Werror
LIBFLAGS = -Llibmb -lmb

all:
	make -C $(L_DIR)
	make $(NAME)

$(NAME): $(LIB) $(HDR) $(OBJ) $(OBJ_32)
	gcc -o $@ $(OBJ) $(OBJ_32) $(LIBFLAGS)

$(S_DIR)%.o: $(S_DIR)%.c
	gcc $(CFLAGS) -c $< -o $@

$(S_DIR)32_%.o: $(S_DIR)%.c
	gcc $(CFLAGS) -D X32 -c $< -o $@

clean:
	rm -rf $(O_DIR)
	make clean -C $(L_DIR)

fclean: clean
	make fclean -C $(L_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re