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

SRC_ENTITY		=	entity/
SRC_PARSER		=	parser/
SRC_FILE		=	file/
SRC_SERVER		=	server/
SRC_REQUEST		=	request/
SRC_RESPONSE	=	response/
SRC_CGI			=	cgi/
SRC_UTILS		=	utils/

#Files
ENTITY			=	ErrorPage HttpScope LocationScope RedirectionUri ServerScope	
PARSER			=	Parser
FILE			=	File
SERVER			=	Server Cluster
REQUEST			=	Request
RESPONSE		=
CGI				=	Cgi

SRC_MAIN		= 	main

SRC_FILES		+=	$(addprefix $(SRC_ENTITY),$(ENTITY))
SRC_FILES		+=	$(addprefix $(SRC_PARSER),$(PARSER))
SRC_FILES		+=	$(addprefix $(SRC_FILE),$(FILE))
SRC_FILES		+=	$(addprefix $(SRC_SERVER),$(SERVER))
SRC_FILES		+=	$(addprefix $(SRC_REQUEST),$(REQUEST))
SRC_FILES		+=	$(addprefix $(SRC_RESPONSE),$(RESPONSE))
SRC_FILES		+=	$(addprefix $(SRC_CGI),$(CGI))
SRC_FILES		+=	$(SRC_MAIN)

INC_FILES		+=	$(addprefix $(SRC_ENTITY),$(ENTITY))
INC_FILES		+=	$(addprefix $(SRC_PARSER),$(PARSER))



#FileCreate
SRC 			= 	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_FILE)))
INC 			= 	$(addprefix $(INC_DIR), $(addsuffix .hpp, $(INC_FILES)))
OBJ 			= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))

OBJF			=	.cache_exists

all:		welcome $(NAME)

$(OBJF):
			@mkdir -p $(OBJ_DIR)
			@mkdir -p $(OBJ_DIR)$(SRC_ENTITY)
			@mkdir -p $(OBJ_DIR)$(SRC_PARSER)
			@mkdir -p $(OBJ_DIR)$(SRC_FILE)
			@mkdir -p $(OBJ_DIR)$(SRC_SERVER)
			@mkdir -p $(OBJ_DIR)$(SRC_REQUEST)
			@mkdir -p $(OBJ_DIR)$(SRC_RESPONSE)
			@mkdir -p $(OBJ_DIR)$(SRC_CGI)

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