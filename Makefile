NAME =  minishell
PRINT = libft/libft.a
SRC = minishell.c split_line.c launch_command.c environ.c built_in.c g_line.c
# OBJ = $(SRC:.c=.o)
OBJ = bin/*.o
all: $(NAME)
$(NAME):
	@make -sC libft/
	@gcc -c $(SRC)
	@mv *.o bin
	@gcc -o $(NAME) $(OBJ) -L. $(PRINT)
clean:
	@make clean -sC libft/
	@rm -rf $(OBJ)
fclean: clean
	@make fclean -sC libft/
	@rm -rf $(NAME)
re: fclean all clean