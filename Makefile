
NAME				=	miniRT
LIBFT				=	libft/libft.a
MLX42				=	MLX42/build/libmlx42.a

VPATH				=	src src/parser src/primitives src/utils src/hooks

SRC					=	main.c cleanup.c select.c \
						loader.c parser.c parse_utils1.c parse_utils2.c parse_utils3.c parse_default_objs.c parse_lights.c parse_primitives.c \
						init_hooks.c key_hook.c cursor_hook.c mouse_hook.c resize_hook.c loop_hook.c \
						movement.c init_miniRT.c \
						render.c plane.c sphere.c cylinder.c get_diffuse_color.c get_hitpoint.c \
						vec3_utils1.c vec3_utils2.c vec4_utils1.c vec3_rotation.c color_convert.c time.c image.c
OBJ					=	$(addprefix ./obj/, $(SRC:%.c=%.o))

CFLAGS				=	-Wall -Wextra -Werror -flto -Ofast -march=native
LDFLAGS				=	-lm -ldl -lglfw -flto
FSANITIZE			=	-g -fsanitize=address
DEBUG				=	-g
CC					=	cc

COL_GREEN			= 	\033[32m
COL_RED				=	\033[31m
COL_YELLOW			= 	\033[38;2;214;189;28m
COL_PINK			= 	\033[95m
COL_DEFAULT			= 	\033[0m

.SILENT:

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	echo "$(COL_YELLOW)Building $(NAME)...$(COL_DEFAULT)"
	$(CC) $(OBJ) $(LIBFT) $(MLX42) -o $(NAME) $(LDFLAGS)
	echo "$(COL_GREEN)Successfully built $(NAME).$(COL_DEFAULT)"

$(LIBFT):
#	echo "$(COL_YELLOW)Building $(LIBFT)...$(COL_DEFAULT)"
#	$(MAKE) -C libft
#	echo "$(COL_GREEN)Successfully built $(LIBFT).$(COL_DEFAULT)"

mlx42:
	git clone https://github.com/codam-coding-college/MLX42.git
	cd MLX42 && cmake -B build && cmake --build build -j4

./obj/%.o: %.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
#	$(MAKE) -C libft clean > /dev/null
	rm -rf obj
	echo "$(COL_GREEN)Object files have been removed.$(COL_DEFAULT)"

fclean: clean
#	$(MAKE) -C libft fclean > /dev/null
#	echo "$(COL_GREEN)$(LIBFT) has been removed.$(COL_DEFAULT)"
	rm -f $(NAME)
	echo "$(COL_GREEN)$(NAME) has been removed.$(COL_DEFAULT)"

re: fclean all

f: LDFLAGS += $(FSANITIZE)
f: CFLAGS += $(FSANITIZE)
f: re
	# ./$(NAME)

d: LDFLAGS += $(DEBUG)
d: CFLAGS += $(DEBUG)
d: re

.PHONY: all clean fclean re f d
