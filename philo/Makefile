NAME	=	Philosophers
FLAGS	=	-pthread
SRC_DIR	=	src/
SRC		=	$(addprefix $(SRC_DIR), main.c setup.c philosopher.c death_thread.c ft_atoi.c)
OBJ_DIR	=	obj/
OBJ		=	$(addprefix $(OBJ_DIR), $(notdir $(SRC:.c=.o)))

all: $(NAME)

$(NAME): $(OBJ_DIR) $(MAIN) $(LIB) $(OBJ)
	@cc $(FLAGS) $(OBJ) -I include/ -o $(NAME)
	@echo "compiling $(NAME)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@cc $(FLAGS) -c $< -I include/ -o $@
	@echo "compiling file $@"

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	@rm -rf $(OBJ)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
