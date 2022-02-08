CC = gcc
OBJ = main.o io.o grille.o jeu.o
LDFLAGS += -lcairo -lm -lX11
CFLAGS = -g -Wall -Iinclude -I/usr/include/cairo
DISTNAME = SadayArhun-GoL-v5.0.1
DISTFILE = *.tar.xz

INCDIR = include/
OBJDIR = obj/
SRCDIR = src/
BINDIR = bin/
DOCDIR = doc/
OBJDIR = obj/
LIBDIR = lib/

vpath %.c $(SRCDIR)
vpath %.h $(INCDIR)
vpath %.o $(OBJDIR)
vpath %.a $(LIBDIR)

%.o: %.c
	@mkdir -p $(OBJDIR)
	@$(CC) -DMODE$(MODE) $(CFLAGS) -o $(OBJDIR)/$@ -c $<

main: $(OBJ) libjeu.a
	@mkdir -p $(BINDIR)
	@rm -f $(OBJDIR)/grille.o $(OBJDIR)/jeu.o
	@$(CC) -DMODE$(MODE) $(CFLAGS) -lm -o $(BINDIR)$@ $(OBJDIR)* -L$(LIBDIR) -ljeu $(LDFLAGS)

libjeu.a:
	@ar rcs $@ $(OBJDIR)/grille.o $(OBJDIR)/jeu.o 
	@mkdir -p $(LIBDIR)
	@mv *.a $(LIBDIR)

docs:
	doxygen

dist:
	tar -Jcvf $(DISTNAME).tar.xz $(SRCDIR) $(INCDIR) Makefile Doxyfile

clean:
	@rm -rf $(BINDIR)
	@rm -rf $(OBJDIR)
	@rm -rf $(DOCDIR)
	@rm -rf $(LIBDIR)
	@rm -rf $(DISTFILE)
