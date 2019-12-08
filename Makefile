CC=g++
CFLAGS=-Wall -g -pthread
#OBJ = frontendserver.o servercommon.o singleconnserverhtml.o \
#singleconnservercontrol.o cookierelay.o backendrelay.o mailservice.o #loadbalancer.o

TARGETS = frontendserver
#TARGETS = servercommon frontendserver singleconnserverhtml singleconnservercontrol \
#	cookierelay backendrelay mailservice
#
all: $(TARGETS)

#servercommon: servercommon.o
#	$(CC) $^ $(CFLAGS) -c

%.o: %.cc %.h
	$(CC) $(CFLAGS) -o $@ -c $<

frontendserver: servercommon.o frontendserver.o singleconnserverhtml.o singleconnservercontrol.o cookierelay.o backendrelay.o mailservice.o
	$(CC) $(CFLAGS) $^ -o $@
	
#singleconnserverhtml: singleconnserverhtml.o servercommon.o mailservice.o
#	$(CC) $^ $(CFLAGS) -c
#	
#singleconnservercontrol: singleconnservercontrol.o servercommon.o
#	$(CC) $^ $(CFLAGS) -c
#	
#cookierelay: cookierelay.o servercommon.o
#	$(CC) $^ $(CFLAGS) -c
#	
#backendrelay: backendrelay.o servercommon.o
#	$(CC) $^ $(CFLAGS) -c
#	
#mailservice: mailservice.o
#	$(CC) $^ $(CFLAGS) -c
#	

#frontendserver: frontendserver.cc
#	g++ -pthread $< -o $@

pack:
	rm -f submit-finalproject.zip
	zip -r submit-finalproject.zip *.cc README Makefile

clean:
	rm -fv $(TARGETS) *.o
