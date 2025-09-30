NAME = ft_nm

S_DIR = src
O_DIR = obj
O_DIR_64 = $(O_DIR)/64
O_DIR_32 = $(O_DIR)/
LIBMB_DIR = libmb/

SRCS_LIST = main.c \
            utils.c \
            print.c \
            endian.c \
            elf_info.c \
            symbols.c

SRCS = $(addprefix $(S_DIR)/,$(SRCS_LIST))

NON_GENERIC_SRCS = $(filter %/main.c %/utils.c %/print.c %/endian.c, $(SRCS))
GENERIC_SRCS = $(filter-out $(NON_GENERIC_SRCS), $(SRCS))

OBJS_NON_GENERIC = $(patsubst $(S_DIR)/%.c, $(O_DIR)/%.o, $(NON_GENERIC_SRCS))
OBJS_64 = $(patsubst $(S_DIR)/%.c, $(O_DIR_64)/%.o, $(GENERIC_SRCS))
OBJS_32 = $(patsubst $(S_DIR)/%.c, $(O_DIR_32)/%.o, $(GENERIC_SRCS))

HDRS = $(wildcard $(S_DIR)/*.h)

CFLAGS = -Wall -Wextra -Werror
HDRFLAGS = -I.
LIBFLAGS = -Llibmb -lmb

all: $(NAME)

$(NAME): $(OBJS_NON_GENERIC) $(OBJS_64) $(OBJS_32)
	make -C $(LIBMB_DIR)
	gcc $(CFLAGS) $(HDRFLAGS) -o $@ $^ $(LIBFLAGS)

$(O_DIR)/%.o: $(S_DIR)/%.c $(HDRS)
	@mkdir -p $(O_DIR)
	gcc $(CFLAGS) $(HDRFLAGS) -c $< -o $@ $(LIBFLAGS)

$(O_DIR_64)/%.o: $(S_DIR)/%.c $(HDRS)
	@mkdir -p $(O_DIR_64)
	gcc $(CFLAGS) $(HDRFLAGS) -c $< -o $@ $(LIBFLAGS)

$(O_DIR_32)/%.o: $(S_DIR)/%.c $(HDRS)
	@mkdir -p $(O_DIR_32)
	gcc $(CFLAGS) $(HDRFLAGS) -D X32 -c $< -o $@ $(LIBFLAGS)

container:
	@docker build \
		--build-arg NAME=$$(id -un) \
		--build-arg UID=$$(id -u) \
		--build-arg GID=$$(id -g) \
		-t ft_nm_image .

exec: container
	@docker run -d --name ft_nm_container ft_nm_image
	@docker exec -it ft_nm_container /bin/bash

tester: $(NAME) container
	@test -d nm_tester || git clone https://github.com/jemercie/nm_tester.git
	@docker run --rm \
		-v $(CURDIR):/app \
		-v $(CURDIR)/nm_tester:/app/nm_tester \
		-w /app/nm_tester \
		ft_nm_image ./run all

clean:
	rm -rf $(O_DIR)
	make clean -C $(LIBMB_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBMB_DIR)
	rm -rf nm_tester

re: fclean all

.PHONY: all clean fclean re