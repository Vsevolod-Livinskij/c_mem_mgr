CXX=clang
CXXFLAGS=-std=c11 -Wall -Wpedantic -Werror -Wno-pointer-arith
OPT=-O3
LDFLAGS=-L./ -std=c11
SOURCES=main.c mem_mgr.c test_struct.c test.c
OBJS=$(addprefix objs/, $(SOURCES:.c=.o))
HEADERS=util.h test_struct.h mem_mgr.h test.h
EXECUTABLE=mem_mgr

default: $(EXECUTABLE)

$(EXECUTABLE): dir $(SOURCES) $(HEADERS) $(OBJS)
	$(CXX) $(OPT) $(LDFLAGS) -o $@ $(OBJS)

dir:
	/bin/mkdir -p objs

clean:
	/bin/rm -rf objs $(EXECUTABLE)

debug: $(EXECUTABLE)
debug: OPT=-O0 -g

ubsan: $(EXECUTABLE)
ubsan: CXXFLAGS+=-fsanitize=undefined

objs/%.o: %.c $(HEADERS)
	$(CXX) $(OPT) $(CXXFLAGS) -o $@ -c $< -DTEST_NUM=$(TEST_NUM)
