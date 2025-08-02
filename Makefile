# Name of the final executable
NAME = ft_nm

# Directories
S_DIR = src
O_DIR = obj
O_DIR_64 = $(O_DIR)/64
O_DIR_32 = $(O_DIR)/32

# Source files
# List all your .c files here
SRCS_LIST = main.c \
			utils.c \
			print.c \
			endian.c \
			process_elf.c \
			symbol.c

# Prepend source directory to the list
SRCS = $(addprefix $(S_DIR)/,$(SRCS_LIST))

# Separate main.c and utils.c from the generic sources
NON_GENERIC_SRCS = $(filter %/main.c %/utils.c %/print.c %/endian.c, $(SRCS))
GENERIC_SRCS = $(filter-out %/main.c %/utils.c %/print.c %/endian.c, $(SRCS))

# Object files
OBJS_NON_GENERIC = $(patsubst $(S_DIR)/%.c, $(O_DIR)/%.o, $(NON_GENERIC_SRCS))
OBJS_64 = $(patsubst $(S_DIR)/%.c, $(O_DIR_64)/%.o, $(GENERIC_SRCS))
OBJS_32 = $(patsubst $(S_DIR)/%.c, $(O_DIR_32)/%.o, $(GENERIC_SRCS))

# Header files
HDRS = $(wildcard $(S_DIR)/*.h)

# Compiler and flags
CC = gcc
# -I$(S_DIR) tells gcc where to find ft_nm.h
CFLAGS = -Wall -Wextra -Ilibft -Llibft -lft -g

# Default rule
all: $(NAME)

# Linking rule - combines non-generic, 64-bit, and 32-bit objects
$(NAME): $(OBJS_NON_GENERIC) $(OBJS_64) $(OBJS_32)
	make -C libft
	$(CC) -o $@ $^ $(CFLAGS)

# Rule for non-generic .o files (main.o, utils.o)
$(O_DIR)/%.o: $(S_DIR)/%.c $(HDRS)
	@mkdir -p $(O_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for 64-bit objects (compiles without any special -D flag)
$(O_DIR_64)/%.o: $(S_DIR)/%.c $(HDRS)
	@mkdir -p $(O_DIR_64)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for 32-bit objects (compiles with -D X32 and -m32)
$(O_DIR_32)/%.o: $(S_DIR)/%.c $(HDRS)
	@mkdir -p $(O_DIR_32)
	$(CC) $(CFLAGS) -D X32 -c $< -o $@

clean:
	rm -rf $(O_DIR)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re