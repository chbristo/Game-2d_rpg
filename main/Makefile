# path
MODULES = ../modules

# Compile options. Το -I<dir> χρειάζεται για να βρει ο gcc τα αρχεία .h
CFLAGS = -Wall -Werror -g -I$(INCLUDE)

# Αρχεία .o, εκτελέσιμο πρόγραμμα και παράμετροι
OBJS = main.o $(MODULES)/game.o $(MODULES)/map.o $(MODULES)/mapelements.o $(MODULES)/entity.o $(MODULES)/npc.o $(MODULES)/functions.o $(MODULES)/avatar.o
 
EXEC = main
ARGS =


$(EXEC): $(OBJS)
	g++ $(OBJS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)