CC=gcc
CFLAGS= -Iinclude -Wall -Wextra -Wshadow -Wformat=2 -Wformat-overflow -Wformat-truncation=2 -Wundef -fno-common -Wconversion -std=c99 -O2 -g
CPPFLAGS= -MMD
GTKCFLAGS= `pkg-config --cflags gtk4`
GTKLIBS= `pkg-config --libs gtk4-x11`
GLLIBS = `pkg-config --libs glew`
X11LIB = `pkg-config --libs x11`
LDFLAGS= $(GTKLIBS)  $(GLLIBS) $(X11LIB) -lm


OBJDIR = ./obj
SRCDIR = ./src
SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEP := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.d,$(SRC))
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

.PHONY: clean all $(current_dir)



all: $(current_dir)


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(GTKCFLAGS) -c $< -o $@
-include $(DEP)

${current_dir}: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@


#-shared


clean:
	${RM} ${OBJ} ${DEP} fractalView



# END
