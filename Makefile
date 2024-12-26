WARNING = -Wall -Wextra -Wpedantic -Wno-unused-result -Werror
CFLAGS = -std=c99 -O0 $(WARNING) -ggdb
LDLIBS = -lX11 -lpng

NAME = si
OUTDIR = .build
OBJ = \
      $(OUTDIR)/main.o \
      $(OUTDIR)/img.o \
      $(OUTDIR)/png.o \

all: $(NAME)

run: $(NAME)
	$(OUTDIR)/$(NAME) res/test.png

$(OUTDIR)/%.o: src/%.c
	@mkdir -p $(OUTDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(NAME): $(OBJ)
	$(CC) -o $(OUTDIR)/$@ $^ $(LDLIBS) $(LDFLAGS)

clean:
	rm -rf .build/ log core

