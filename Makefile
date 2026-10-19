# Analyseur de logs Apache - compilation
#
#   make                 compile avec le log d'exemple (data/access.log)
#   make LOG=chemin      compile avec un autre fichier de log
#   make install-cgi     installe la version CGI sur un serveur Apache
#   make clean           supprime les fichiers generes
#
# Apres un changement de LOG / STYLE / RESULTATS, faire "make clean" avant.

CC        ?= gcc
CFLAGS    ?= -Wall -O2
SDL_CFLAGS = $(shell sdl-config --cflags)
SDL_LIBS   = $(shell sdl-config --libs) -lSDL_ttf -lm

LOG       ?= data/access.log
STYLE     ?= cgi/style.html
RESULTATS ?= resultats
FONT      ?= /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf
CGIDIR    ?= /usr/lib/cgi-bin

BIN  = analyseur
OBJ  = build/versiondynamique.o build/libgraphique.o
DEFS = -DLOG_FILE='"$(LOG)"' -DSTYLE_FILE='"$(STYLE)"' \
       -DATTAQUES_FILE='"$(RESULTATS)/Attaques.txt"' \
       -DNAVIGATEURS_FILE='"$(RESULTATS)/Navigateurs.txt"' \
       -DFONT_FILE='"$(FONT)"'

all: $(BIN) | $(RESULTATS)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(SDL_LIBS)

build/versiondynamique.o: src/versiondynamique.c lib/libgraphique.h | build
	$(CC) $(CFLAGS) $(DEFS) -Ilib $(SDL_CFLAGS) -c $< -o $@

build/libgraphique.o: lib/libgraphique.c lib/libgraphique.h | build
	$(CC) $(DEFS) $(SDL_CFLAGS) -c $< -o $@

build $(RESULTATS):
	mkdir -p $@

# Version CGI : lit le vrai log Apache, ecrit les resultats bonus dans /tmp
cgi:
	$(MAKE) clean
	$(MAKE) LOG=/var/log/apache2/access.log STYLE=$(CGIDIR)/style.html RESULTATS=/tmp

install-cgi: cgi
	sudo install -m 755 $(BIN) cgi/analyseur-txt $(CGIDIR)/
	sudo install -m 644 cgi/style.html $(CGIDIR)/

clean:
	rm -rf build $(BIN)

.PHONY: all cgi install-cgi clean
