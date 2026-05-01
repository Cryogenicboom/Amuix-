# reference : https://web.stanford.edu/class/archive/cs/cs107/cs107.1174/guide_make.html

# target : dependencies 
#	Command
#	Command 

# to build target (.o file) --> use depedencies (.c files)
# if depedencies changed --> build them using Command


CC = gcc # set compiler 

CFLAGS = -g -Wall -Iinclude

SOURCES = src/main.c \
src/parser/parser.c \
src/parser/tokenizer.c \
src/executor/executor.c \
src/builtins/builtins.c

OUT = AMUNIX

# rules below 

#whenever we run "make" in cmd, it runs first rule defined which is "all" here and this combines all src .c files in "SOURCES" alias
all: 
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUT)

# "make run" to run the final .o file
run: 
# 	exec ./$(OUT)
	gnome-terminal --geometry=120x30 -- ./$(OUT)

# deletes the .o file created.
clean: 
	rm -f $(OUT)


