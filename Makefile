NAME =	ft_nm

S_DIR =	src/
O_DIR =	obj/

SRC =	main.c \
		utils.c \
		globals.c

OBJ =	$(addprefix $(O_DIR),$(SRC:.c=.o))

HDR =	src/ft_nm.h

FLG =	-Wall -Wextra -Werror

$(NAME): $(OBJ) $(HDR)
	gcc -o $@ $^

$(O_DIR)%.o: $(S_DIR)%.c
	@mkdir -p $(O_DIR)
	gcc -c $^ -o $@ $(FLG)

all: $(NAME)

clean:
	rm -rf $(O_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re