ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc
CC_FLAGS = -Wall -Werror -Wextra
LIB = -L./libft -lft -lpthread
INC = -I ./inc -I ./libft/inc

SRC = show_alloc_mem.c malloc.c
SRC_DIR = ./src/
SRCS = $(addprefix $(SRC_DIR), $(SRC))

OBJ = $(SRC:.c=.o)
OBJ_DIR = ./obj/
OBJS = $(addprefix $(OBJ_DIR), $(OBJ))

#.SILENT:

all: $(NAME)

lib:
	echo ${LD_LIBRARY_PATH}
	@make -C libft

$(NAME): lib
	@$(CC) -c $(SRCS) $(CC_FLAGS) $(INC)
	@mkdir -p $(OBJ_DIR)
	@mv $(OBJ) $(OBJ_DIR)
	@$(CC) -shared -o $@ $(OBJS) $(LIB)
	ln -s $(NAME) libft_malloc.so

clean:
	@make -C libft clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@make -C libft fclean
	@rm -rf $(NAME) libft_malloc.so

re: fclean all

runtest: re
	$(CC) -c src/main.c $(CC_FLAGS) $(INC)
	$(CC) -o test main.o  $(LIB) -L. -lft_malloc 
	env DYLD_LIBRARY_PATH=`pwd` DYLD_INSERT_LIBRARIES=libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1  ./test

