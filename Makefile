NAME				=	miniRT
VPATH				=	src
SRC					=	main.c
OBJ					=	$(addprefix ./obj/, $(SRC:%.c=%.o))
CFLAGS				=	-Wall -Wextra -Werror
LDFLAGS				=
FSANITIZE			=	-g -fsanitize=address
DEBUG				=	-g
CC					=	cc
COL_GREEN			= 	\033[32m
COL_RED				=	\033[31m
COL_YELLOW			= 	\033[38;2;214;189;28m
COL_PINK			= 	\033[95m
COL_DEFAULT			= 	\033[0m

.SILENT:

all: $(NAME)

$(NAME): $(OBJ)
	echo "$(COL_YELLOW)Building $(NAME)...$(COL_DEFAULT)"
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)
	echo "$(COL_GREEN)Successfully built $(NAME).$(COL_DEFAULT)"

./obj/%.o: %.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj
	echo "$(COL_GREEN)Object files have been removed.$(COL_DEFAULT)"

fclean: clean
	rm -f $(NAME)
	rm -rf obj
	echo "$(COL_GREEN)$(NAME) has been removed.$(COL_DEFAULT)"

re: fclean all

f: LDFLAGS += $(FSANITIZE)
f: CFLAGS += $(FSANITIZE)
f: re
	./$(NAME)

d: LDFLAGS += $(DEBUG)
d: CFLAGS += $(DEBUG)
d: re

.PHONY: all clean fclean re f d
