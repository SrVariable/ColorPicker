NAME = ColorPicker

CC = gcc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I./ -I$(RAYLIB_DIR)/include
LDFLAGS = -L$(RAYLIB_DIR)/lib

ifeq ($(OS), Windows_NT)
	LDLIBS = $(RAYLIB_DIR)/lib/libraylib.a -lgdi32 -lwinmm
else
	LDLIBS = -lraylib -lGL -lm
endif

ifeq ($(OS), Windows_NT)
	RAYLIB_DIR = ./raylib/raylib-5.0_win64_mingw-w64
else
	RAYLIB_DIR = ./raylib/raylib-5.0_linux_amd64
endif

FILES = main.c

all: $(NAME)

$(NAME): $(FILES)
	$(CC) $(FILES) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $(NAME)

clean:
ifeq ($(OS), Windows_NT)
	$(RM) $(NAME).exe
else
	$(RM) $(NAME)
endif

fclean: clean

re: fclean all

run: $(NAME)
	./$(NAME)

.PHONY = all clean fclean re run
