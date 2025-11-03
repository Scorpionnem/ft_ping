NAME = ft_ping

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

INCLUDES =	-I ./includes/\
			-I ./libftgetopt/includes/

SRCS =	src/main.c\
		src/ctx.c\
		src/checksum.c\
		src/dns.c

OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.c=$(OBJDIR)/%.d)

LIBFTGETOPTDIR = libftgetopt
LIBFTGETOPT = ./$(LIBFTGETOPTDIR)/libftgetopt.a

all: libftgetopt $(LIBFTGETOPT) $(NAME)

libftgetopt:
	@if ls | grep -q "$(LIBFTGETOPTDIR)"; then \
		echo "\033[32;1;4mlibftgetopt Found\033[0m"; \
	else \
		echo "\033[31;1;4mlibftgetopt Not Found\033[0m"; \
		echo "\033[31;1mCloning libftgetopt from github\033[0m"; \
		git clone https://github.com/Scorpionnem/libftgetopt $(LIBFTGETOPTDIR); \
		make -C $(LIBFTGETOPTDIR) ;\
	fi

re: fclean all

$(LIBFTGETOPT):
	make -C $(LIBFTGETOPTDIR)

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBFTGETOPT)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

clean:
	@make -C $(LIBFTGETOPTDIR) clean
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@make -C $(LIBFTGETOPTDIR) fclean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)

dclean: fclean
	@rm -rf $(LIBFTGETOPTDIR)

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean re run libftgetopt

-include $(DEPS)
