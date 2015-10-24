# TP1 - 
#
# @author Ilyas Toumlilt <toumlilt.ilyas@gmail.com>
# @copyright (c) 2015, toumlilt
#
# @version 1.0
# @package toumlilt/M2/OSEM

CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src
LPTHREAD = -lpthread


all: directories runTest

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

# regles generales :
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $< -I$(INC) 

$(BIN)/% : $(OBJ)/%.o
	$(CC) -o $@ $< $(LPTHREAD)
#fin regles generales

$(BIN)/test: $(OBJ)/test.o $(OBJ)/words_generator.o $(OBJ)/distributed_words_counter.o
	$(CC) -o $@ $^ $(LPTHREAD)

runTest: $(BIN)/test
	$(BIN)/test

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall:
	rm -rf ${OBJ} ${BIN} ${LIB}
	rm -f ${INC}/*~ ${SRC}/*~ *~
