
NAME				=	miniRT
LIBFT				=	libft/libft.a
STB					=	inc/stb

VPATH				=	src src/init src/parser src/shader src/primitives src/utils src/hooks lib

SRC					=	main.c glad.c cleanup.c \
						init_agx_lut_tbo.c init_lights_tbo.c init_texture_ubo.c init_material_ubo.c \
						init_objects_tbo.c init_objects_primitives.c load_textures.c \
						shader_assembler.c shader_compiler.c shader_program.c run_shader.c bind_buffer.c \
						loader.c parser.c parse_utils1.c parse_utils2.c parse_utils3.c parse_material.c parse_material2.c parse_texture.c \
						parse_default_objs.c parse_lights.c parse_primitives.c parse_primitives2.c parser_validations.c \
						init_hooks.c key_hook.c cursor_hook.c mouse_hook.c resize_hook.c scroll_hook.c \
						movement.c init_miniRT.c \
						update.c\
						ray_utils.c string_utils.c vec3_utils1.c vec3_utils2.c vec4_utils1.c vec3_rotation.c time.c
#						render.c plane.c sphere.c cylinder.c get_diffuse_color.c get_specular_color.c get_reflection_color.c get_hitpoint.c 
OBJ					=	$(addprefix ./obj/, $(SRC:%.c=%.o))

CFLAGS				=	-Wall -Wextra -Werror #-flto -Ofast -march=native
LDFLAGS				=	-lm -ldl -lglfw #-flto
INCLUDES			=	-I./inc/
FSANITIZE			=	-g -fsanitize=address
DEBUG				=	-g
CC					=	cc

COL_GREEN			= 	\033[32m
COL_RED				=	\033[31m
COL_YELLOW			= 	\033[38;2;214;189;28m
COL_PINK			= 	\033[95m
COL_DEFAULT			= 	\033[0m

.SILENT:

all: $(LIBFT) $(STB) $(NAME)

$(NAME): $(OBJ)
	echo "$(COL_YELLOW)Building $(NAME)...$(COL_DEFAULT)"
	$(CC) $(OBJ) $(LIBFT) $(MLX42) -o $(NAME) $(LDFLAGS)
	echo "$(COL_GREEN)Successfully built $(NAME).$(COL_DEFAULT)"

$(LIBFT):
#	echo "$(COL_YELLOW)Building $(LIBFT)...$(COL_DEFAULT)"
#	$(MAKE) -C libft
#	echo "$(COL_GREEN)Successfully built $(LIBFT).$(COL_DEFAULT)"

$(STB):
	echo "$(COL_YELLOW)Cloning $(STB)...$(COL_DEFAULT)"
	git clone git@github.com:nothings/stb.git inc/stb
	echo "$(COL_GREEN)Successfully cloned $(STB).$(COL_DEFAULT)"

./obj/%.o: %.c
	mkdir -p obj
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
#	$(MAKE) -C libft clean > /dev/null
	rm -rf obj
	echo "$(COL_GREEN)Object files have been removed.$(COL_DEFAULT)"

fclean: clean
#	$(MAKE) -C libft fclean > /dev/null
#	echo "$(COL_GREEN)$(LIBFT) has been removed.$(COL_DEFAULT)"
	rm -rf $(STB)
	echo "$(COL_GREEN)$(STB) has been removed.$(COL_DEFAULT)"
	rm -f $(NAME)
	echo "$(COL_GREEN)$(NAME) has been removed.$(COL_DEFAULT)"

re: fclean all

f: LDFLAGS = -lm -ldl -lglfw -g -fsanitize=address
f: CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
f: re
	# ./$(NAME)

d: LDFLAGS += $(DEBUG)
d: CFLAGS += $(DEBUG)
d: re

.PHONY: all clean fclean re f d
