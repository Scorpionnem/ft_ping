NAME = ft_ping

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

INCLUDES =	-I ./includes/

SRCS =	src/main.c

OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.c=$(OBJDIR)/%.d)

all: $(NAME)

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

clean:
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean re run

-include $(DEPS)
