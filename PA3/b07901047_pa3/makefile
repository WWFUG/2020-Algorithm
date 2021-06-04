# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c -std=c++11
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O3

all	: bin/cb
	@echo -n ""

# optimized version
bin/cb	: cycle_breaker_opt.o main_opt.o lib
			$(CC) $(OPTFLAGS) cycle_breaker_opt.o main_opt.o -ltm_usage -Llib -o bin/cb
main_opt.o 	   	: src/main.cpp lib/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
cycle_breaker_opt.o	: src/cycle_breaker.cpp src/cycle_breaker.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# DEBUG Version
dbg : bin/cb_dbg
	@echo -n ""

bin/mps_dbg	: cycle_breaker_dbg.o main_dbg.o lib
			$(CC) $(DBGFLAGS) cycle_breaker_dbg.o main_dbg.o -ltm_usage -Llib -o bin/cb_dbg
main_dbg.o 	   	: src/main.cpp lib/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
cycle_breaker_dbg.o	: src/cycle_breaker.cpp src/cycle_breaker.h
			$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

lib: lib/libtm_usage.a

lib/libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<
tm_usage.o: lib/tm_usage.cpp lib/tm_usage.h
	$(CC) $(CFLAGS) $<

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*

