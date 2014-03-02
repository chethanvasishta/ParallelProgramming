CC = g++

CPPFLAGS = -std=c++0x -I./include
LDFLAGS = -pthread

all: main
	$(CC) obj/*.o $(LIBS) $(LDFLAGS) -o Par
main:
	find -name '*.cpp' -print0 | xargs -0 $(CC) -c $(CPPFLAGS) && mkdir -p obj && mv *.o ./obj/
#debug

debug: maind
	$(CC) obj/*.o $(LIBS) $(LDFLAGS) -o ParD
maind:
	find -name '*.cpp' -print0 | xargs -0 $(CC) -g -c $(CPPFLAGS) && mkdir -p obj && mv *.o ./obj/

#clean
clean:
	rm -rf obj Par ParD *.dot temp.* a.out
