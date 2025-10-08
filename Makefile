NAME = ft_nm

S_DIR = src/
O_DIR = obj/
L_DIR = libmb/

ALL_SRCS = $(wildcard $(S_DIR)*.c)
SRCS_32 = elf_info.c symbols.c

OBJ = $(patsubst $(S_DIR)%.c, $(O_DIR)%.o, $(ALL_SRCS))
OBJ_32 = $(addprefix $(O_DIR), $(addprefix 32_, $(SRCS_32:.c=.o)))

HDR = $(addprefix $(S_DIR), ft_nm.h)
LIB = $(addprefix $(L_DIR), libmb.a)

CFLAGS = -Wall -Wextra -Werror -g
LIBFLAGS = -Llibmb -lmb

all: $(NAME)

$(NAME): $(LIB) $(HDR) $(OBJ) $(OBJ_32)
	gcc -o $@ $(OBJ) $(OBJ_32) $(LIBFLAGS)

$(LIB):
	make -C $(L_DIR)

$(O_DIR)%.o: $(S_DIR)%.c $(HDR) | $(O_DIR)
	gcc $(CFLAGS) -c $< -o $@

$(O_DIR)32_%.o: $(S_DIR)%.c $(HDR) | $(O_DIR)
	gcc $(CFLAGS) -D X32 -c $< -o $@

$(O_DIR):
	mkdir $@

docker-build-image:
	docker build \
		--build-arg NAME=$$(id -un) \
		--build-arg UID=$$(id -u) \
		--build-arg GID=$$(id -g) \
		-t ft_nm_image .

docker-run:
	docker run \
		-d \
		--name ft_nm_container \
		-v $(CURDIR):/home/$$(id -un)/ft_nm \
		ft_nm_image

docker-enter:
	docker exec -it ft_nm_container /bin/bash

tester-download:
	test -d tester || git clone https://github.com/jemercie/nm_tester.git tester

clean:
	rm -rf $(O_DIR)
	make -C $(L_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(L_DIR) fclean

re: fclean all

.PHONY: all clean fclean re