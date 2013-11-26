CC      = g++
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CCFLAGS  = -g -Wall -I/usr/include/boost -I./lib/threadpool/boost -I/usr/include -I/usr/include/c++/4.7 -I/usr/lib/gcc/x86_64-linux-gnu/4.7/include -I/usr/include/c++/4.7/x86_64-linux-gnu -Wall -c -fmessage-length=0 -std=c++11 -MMD
LDFLAGS = -lboost_thread-mt -lboost_program_options -lsqlite3 -lgmp -lgmpxx -lpthread

all: pam clean

debug: CCFLAGS += -g3 -O0 -DDEBUG -rdynamic
debug: pam 
 
release: CCFLAGS += -Ofast
release: pam

pam: $(OBJ_FILES) 
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.cpp src/%.h
	$(CC) $(CCFLAGS) -c -o $@ $<

.PHONY: clean cleanest

clean:
	rm obj/*

cleanest: clean
	rm pam

-include $(OBJFILES:.o=.d)
