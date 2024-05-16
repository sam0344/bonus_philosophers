NAME	:= philosophers
CC		:=	cc
CFLAGS	:= -Wextra -Wall -Werror -g

HEADERS	:= -I ./include
SRCDIR := src
OBJDIR := ./obj

SRCS  =
# BSRCS =

OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)
# BOBJS := $(BSRCS:%.c=$(OBJDIR)/%.o)

all: $(OBJS) $(NAME)


$(NAME) : $(OBJS) $(OBJDIR)
	@$(CC) $(OBJS) $(HEADERS) -o $(NAME)

bonus : $(BOBJS) $(LIBFT)
	@$(CC) $(BOBJS) $(HEADERS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< $(HEADERS) -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJS) $(BOBJS)

fclean: clean
	@rm -rf $(NAME) $(OBJDIR)

re: fclean all

.PHONY: all, clean, fclean, re, bonus