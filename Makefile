#Definition
NAME        	=   webserv
CC				=	c++
USER_NAME1		=	eozmert
USER_NAME2		=	cyanliz
USER_NAME3		=	fozturk
CFLAGS			=	-Wall -Wextra -Werror -std=c++98 -g -I
RM				=	rm -f

#Directory
SRC_DIR			= 	src/
INC_DIR			= 	inc/
OBJ_DIR			=	obj/

#Files
SRC_FILE		=   main
INC_FILE		= 	

#FileCreate
SRC 			+= 	$(addprefix $(MAIN_DIR), $(addsuffix .cpp, $(MAIN_FILE)))
SRC 			+= 	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_FILE)))
INC 			= 	$(addprefix $(INC_DIR), $(addsuffix .hpp, $(INC_FILE)))
OBJ 			= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILE)))

OBJF			=	.cache_exists

all:		welcome $(NAME)

$(OBJF):
			@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp | $(OBJF)
			@$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@
			@echo Compiling Success file is : $< $(SRC_DIR) $@ 

$(NAME):	$(OBJ) $(INC)
			@$(CC) $(OBJ) $(CFLAGS) $(INC_DIR) -o $(NAME) 
			@echo $(NAME) compiled!

clean:
			@$(RM) -rf $(OBJ_DIR)
			@$(RM) -f $(OBJF)const
			@echo Objects files cleaned!

fclean:		clean
			@$(RM) -f $(NAME)
			@echo $(NAME) executable files cleaned!

re:			fclean all
			@echo Cleaned and rebuilt

welcome:
			@echo "CPP"
			@echo "\n"

.PHONY:		all clean fclean re bonus norm