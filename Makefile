NAME				=	miniRT
LIBFT				=	include/libft/libft.a
VPATH				=	include/glad src src/init src/parser src/shader src/primitives src/utils src/hooks
SRC					=	main.c cleanup.c \
						init_agx_lut_tbo.c init_lights_tbo.c init_texture_ubo.c init_material_ubo.c init_objects_tbo.c init_objects_primitives.c \
						load_textures.c init_vertex_buffer_objects.c init_glfw.c init_environment_map.c init_environment_map_importance.c \
						shader_assembler.c shader_compiler.c shader_program.c draw_gizmo.c bind_buffer.c bind_buffer2.c \
						loader.c parser.c parse_utils1.c parse_utils2.c parse_utils3.c parse_utils4.c parse_material.c parse_material2.c parse_texture.c \
						parse_default_objs.c parse_lights.c parse_primitives.c parse_primitives2.c parser_validations.c \
						init_hooks.c key_hook.c cursor_hook.c mouse_hook.c resize_hook.c scroll_hook.c \
						movement.c init_miniRT.c \
						update.c update_window_title.c render.c \
						string_utils.c vec3_utils1.c vec3_utils2.c vec3_rotation.c time.c glad.c
OBJ					=	$(addprefix ./obj/, $(SRC:%.c=%.o))

CFLAGS				=	-Wall -Wextra -Werror -flto -Ofast -march=native
LDFLAGS				=	-lm -ldl -lglfw -flto
INCLUDES			=	-I./inc/
FSANITIZE			=	-g -fsanitize=address
DEBUG				=	-g
CC					=	cc

COL_GREEN			= 	\033[32m
COL_RED				=	\033[31m
COL_YELLOW			= 	\033[38;2;214;189;28m
COL_PINK			= 	\033[95m
COL_DEFAULT			= 	\033[0m

IS_WSL				=	$(shell grep -i -q 'Microsoft' /proc/version 2>/dev/null && echo 1 || echo 0)
IS_MNT				=	$(shell echo $(PWD) | grep -q '^/mnt/' && echo 1 || echo 0)
ifeq ($(IS_WSL), 1)
    CFLAGS += -DUSING_WSL
    ifeq ($(IS_MNT), 1)
        $(warning WARNING: You are in a /mnt/ directory, which will cause performance issues in WSL! The program might not even load at all. Please move the repository to your internal WSL file system.)
    endif
endif



.SILENT:

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	echo "$(COL_YELLOW)Building $(NAME)...$(COL_DEFAULT)"
	$(CC) $(OBJ) $(LIBFT) $(MLX42) -o $(NAME) $(LDFLAGS)
	echo "$(COL_GREEN)Successfully built $(NAME).$(COL_DEFAULT)"

$(LIBFT):
	echo "$(COL_YELLOW)Building $(LIBFT)...$(COL_DEFAULT)"
	$(MAKE) -C include/libft
	echo "$(COL_GREEN)Successfully built $(LIBFT).$(COL_DEFAULT)"

./obj/%.o: %.c
	mkdir -p obj
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C include/libft clean > /dev/null
	rm -rf obj
	echo "$(COL_GREEN)Object files have been removed.$(COL_DEFAULT)"

fclean: clean
	$(MAKE) -C include/libft fclean > /dev/null
	echo "$(COL_GREEN)$(LIBFT) has been removed.$(COL_DEFAULT)"
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
