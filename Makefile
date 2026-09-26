#=============================================================================
#	Nom         : Makefile
#	Auteur      : Dominique ERIN
#	Rôle        : Compilation du programme avec interface graphique
#	Version     : V01
#	Licence     : Réalisé dans le cadre du Cours d'ALGO (2025/2026)
#	Compilation : make
#	Usage       : make / make clean
#============================================================================
# === Makefile pour le dictionnaire français-japonais avec interface OpenMotif ===

CC = gcc
CXX = g++

SRC = src
INC = include

CFLAGS = -Wall -Wextra -g -I$(INC)
CXXFLAGS = -Wall -Wextra -g -I$(INC)

LIBS = -lm -lXm -lXt -lX11

TARGET = Diko
TEST_TARGET = Diko_test

OBJ = dico_gui.o main_initial.o cons.o free.o parse.o recherche.o init.o interaction.o

all: $(TARGET)
	@echo "Compilation complète. Saisir './$(TARGET)' dans le terminal."

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

dico_gui.o: $(SRC)/dico_gui.cpp $(INC)/dico_gui.h $(INC)/widget.h $(INC)/commode.h $(INC)/sys.h $(INC)/structures.h $(INC)/main_initial.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main_initial.o: $(SRC)/main_initial.c $(INC)/main_initial.h $(INC)/sys.h $(INC)/structures.h
	$(CC) $(CFLAGS) -c $< -o $@

cons.o: $(SRC)/cons.c $(INC)/sys.h $(INC)/structures.h
	$(CC) $(CFLAGS) -c $< -o $@

free.o: $(SRC)/free.c $(INC)/sys.h $(INC)/structures.h
	$(CC) $(CFLAGS) -c $< -o $@

parse.o: $(SRC)/parse.c $(INC)/sys.h $(INC)/structures.h
	$(CC) $(CFLAGS) -c $< -o $@

recherche.o: $(SRC)/recherche.c $(INC)/sys.h $(INC)/structures.h
	$(CC) $(CFLAGS) -c $< -o $@

init.o: $(SRC)/init.c $(INC)/sys.h $(INC)/structures.h
	$(CC) $(CFLAGS) -c $< -o $@

interaction.o: $(SRC)/interaction.c $(INC)/sys.h $(INC)/structures.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	@echo "Lancement des tests..."
	./$(TEST_TARGET)
	@echo "Tests terminés."

$(TEST_TARGET): test_main.o cons.o free.o parse.o recherche.o init.o interaction.o test.o
	$(CC) $(CFLAGS) -o $@ test_main.o cons.o free.o parse.o recherche.o init.o interaction.o test.o -lm

test_main.o: $(SRC)/main.c $(INC)/sys.h $(INC)/structures.h
	$(CC) $(CFLAGS) -DTEST -c $< -o $@

test.o: $(SRC)/test.c $(INC)/sys.h $(INC)/structures.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_TARGET) test_main.o test.o test_cat.txt dico_copie.txt
	@echo "Nettoyage terminé."

.PHONY: all clean test