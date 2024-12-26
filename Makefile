WARNING = -Wall -Wextra -Wpedantic -Wno-unused-result -Werror
CFLAGS = -std=c99 -O0 $(WARNING)
LDLIBS = -lX11

NAME = si
OUTDIR = .build
OBJ = \
      $(OUTDIR)/main.o \
      $(OUTDIR)/img.o \

all: $(NAME)

run: $(NAME)
	$(OUTDIR)/$(NAME)

$(OUTDIR)/%.o: src/%.c
	@mkdir -p $(OUTDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(NAME): $(OBJ)
	$(CC) -o $(OUTDIR)/$@ $^ $(LDLIBS) $(LDFLAGS)

clean:
	rm -rf .build/ log core

