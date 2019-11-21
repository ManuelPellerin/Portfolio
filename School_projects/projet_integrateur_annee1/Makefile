########       AVR Project Makefile       ########
#####                                        #####
#####      Makefile produit et ecrit par     #####
#####   Simon Barrette & Jerome Collin pour  #####
#####           INF1900 - 2016               #####
#####                                        #####
#####         Inspire de Pat Deegan -        #####
#####  Psychogenic Inc (www.psychogenic.com) #####
##################################################

# Ce Makefile vous permet de compiler des projets
# pour les microcontroleurs Atmel AVR sur 
# Linux ou Unix, en utilisant l'outil AVR-GCC. 
# Ce Makefile supporte C & C++


#####      Details specifique a la cible       #####
#####  Vous devez les adapter a votre projet   #####

# Nom du microcontroleur cible
# (exemple: 'at90s8515')
MCU=atmega324pa

# Nom de votre projet
# (utilisez un seul mot, exemple: 'monprojet')
PROJECT_NAME=projet

SRC_DIR=src

INC_DIR=include

BUILD_DIR=build

EXEC_DIR=exec

HEX_DIR=hex

INSTRUCTION_INPUT_FILE=instructionsrick.txt

LIB_DIR=librobot

LIB_NAME=robot

# Fichiers sources
# Utilisez le suffixe .cpp pour les fichiers C++
# Listez tous les fichiers a compiler, separes par
# un espace. exemple: 'tp1.c tp2.cpp':
PRJSRC= $(wildcard $(SRC_DIR)/*.cpp)

# Inclusions additionnels (ex: -I/path/to/mydir)
INC= -I$(INC_DIR) -I$(LIB_DIR)

# Libraires a lier (ex: -lmylib)
LIBS= -L$(LIB_DIR) -l$(LIB_NAME)

# Niveau d'optimization
# Utilisez s (size opt), 1, 2, 3 ou 0 (off)
OPTLEVEL=s

# Programmer ID - Ne pas changer 
# Liste complete des IDs disponible avec avrdude
AVRDUDE_PROGRAMMERID=usbasp



####################################################
#####         Configuration terminee           #####
#####                                          #####
#####  Le reste de cette section contient les  #####
##### details d'implementation vous permettant #####
##### de mieux comprendre le fonctionnement de ##### 
#####   ce Makefile en vue de sa modification  #####
####################################################



####### variables #######

#compilateur utilise
CC=avr-gcc
#pour copier le contenu d'un fichier objet vers un autre
OBJCOPY=avr-objcopy
#pour permettre le transfert vers le microcontroleur
AVRDUDE=avrdude
#pour supprimer les fichiers lorsque l'on appel make clean
REMOVE=rm -f
# HEXFORMAT -- format pour les fichiers produient .hex
HEXFORMAT=ihex
# pour convertir le fichier d'instructions .txt en .bin
TXT_TO_BIN=progmem



####### Options de compilation #######

# Flags pour le compilateur en C
CFLAGS=-I. -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall                                        

# Flags pour le compilateur en C++
CXXFLAGS=-fno-exceptions



# Linker pour lier les librairies utilisees
LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU)



####### Cible (Target) #######

#Nom des cibles par defaut
TRG=$(EXEC_DIR)/$(PROJECT_NAME).out
HEXROMTRG=$(HEX_DIR)/$(PROJECT_NAME).hex
HEXTRG=$(HEXROMTRG) $(HEX_DIR)/$(PROJECT_NAME).ee.hex



####### Definition de tout les fichiers objets #######

# Cette fonction permet de differencier les fichiers .c
# des fichiers .cpp
# Fichier C
CFILES=$(filter %.c, $(PRJSRC))
# Fichier C++
CPPFILES=$(filter %.cpp, $(PRJSRC))

# Liste de tout les fichiers objet que nous devons creer
OBJDEPS=$(subst $(SRC_DIR), $(BUILD_DIR), $(CFILES:.c=.o)) \
	$(subst $(SRC_DIR), $(BUILD_DIR), $(CPPFILES:.cpp=.o)) 
	
# Pour plus d'information sur cette section, consulter :
# http://bit.ly/257R53E	
# Les fonctions $(filter patternâ€¦,text) &
# $(patsubst pattern,replacement,text) sont pertinentes
	


####### Creation des commandes du Makefile ####### 

# Creation des cibles Phony (Phony Target)
# En plus de la commande make qui permet de compiler
# votre projet, vous pouvez utilisez les commandes
# make all, make install et make clean
.PHONY: all install clean 

# Make all permet simplement de compiler le projet
#
all: $(TRG)

make:
	(cd librobot/; make clean; make)
	make clean
	make install

# set the eeprom and debug
eeprom_debug: clean
eeprom_debug: CXXFLAGS += -DPROG_EEPROM -DDEBUG
eeprom_debug: CFLAGS += -DPROG_EEPROM -DDEBUG
eeprom_debug: $(TRG)

# program the eeprom
eeprom: clean
eeprom: CXXFLAGS += -DPROG_EEPROM
eeprom: CFLAGS += -DPROG_EEPROM
eeprom: $(TRG)
eeprom:
	$(TXT_TO_BIN) -o $(SRC_DIR)/instructions.bin \
	$(SRC_DIR)/$(INSTRUCTION_INPUT_FILE)

# Add debug function. 
# Based on code given in :
# https://stackoverflow.com/questions/1079832/how-can-i-configure-my-makefile-for-debug-and-release-builds
debug:
	(cd librobot/; make debug)
	make clean
	make debug1

debug1: CXXFLAGS += -DDEBUG
debug1: CFLAGS += -DDEBUG
debug1: install

# Implementation de la cible
$(TRG): $(OBJDEPS)
	$(CC) $(LDFLAGS) -o $(TRG) $(OBJDEPS) \
	-lm $(LIBS)

# Production des fichiers object
# De C a objet
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
# De C++ a objet
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

# Verification des dependances (header dependencies)
-include *.d

# Pour plus d'information sur cette section, consulter:
# http://bit.ly/2580FU8

# Production des fichiers hex a partir des fichiers elf
$(HEX_DIR)/%.hex: $(EXEC_DIR)/%.out
	$(OBJCOPY) -j .text -j .data \
		-O $(HEXFORMAT) $< $@ 

# Make install permet de compiler le projet puis
# d'ecrire le programme en memoire flash dans votre
# microcontroleur. Celui-ci doit etre branche par cable USB
install: $(HEXROMTRG)				
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	-p $(MCU) -P -e -U flash:w:$(HEXROMTRG)

# Make clean permet d'effacer tout les fichiers generes
# lors de la compilation
clean:
	$(REMOVE) $(TRG) $(TRG).map $(OBJDEPS) $(HEXTRG) $(BUILD_DIR)/*.d

# Pour plus d'information sur les phony target, consulter:
# http://bit.ly/1WBQe61

# De plus, pour mieux comprendre les makefiles et 
# leur fonctionnement, consulter la documentation de GNU Make:
# http://bit.ly/23Vpk8s

# Finalement, ce tutoriel en ligne constitut une bonne 
# introduction au Makefile:
# http://bit.ly/1XvxsN3

#####                    EOF                   #####
