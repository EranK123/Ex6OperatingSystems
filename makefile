CFLAGS= -Wall
COMP = clang++

all: main1 guard singelton pollserver reactor pollclient

main1: main1.o
	$(COMP) $(CFLAGS) main1.o -o main1 -lpthread

main1.o: main1.cpp
	$(COMP) -c $(CFLAGS) main1.cpp -lpthread

guard: guard.o
	$(COMP) $(CFLAGS) guard.o -o guard

guard.o: guard.cpp
	$(COMP) -c $(CFLAGS) guard.cpp

singelton: singelton.o
	$(COMP) $(CFLAGS) singelton.o -o singelton

singelton.o: singelton.cpp
	$(COMP) -c $(CFLAGS) singelton.cpp

pollserver: pollserver.o
	$(COMP) $(CFLAGS) pollserver.o -o pollserver -lpthread

pollserver.o: pollserver.cpp
	$(COMP) -c $(CFLAGS) pollserver.cpp -lpthread

pollclient: pollclient.o
	$(COMP) $(CFLAGS) pollclient.o -o pollclient -lpthread

pollclient.o: pollclient.cpp
	$(COMP) -c $(CFLAGS) pollclient.cpp -lpthread


reactor: reactor.cpp
	$(COMP) $(CFLAGS) -c reactor.cpp


.PHONY: clean all

clean:
	rm *.o main1 guard singelton pollserver pollclient