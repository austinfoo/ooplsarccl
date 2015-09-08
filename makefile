INCDIRS:=/Users/dmielke/Documents/oopl/trees/googletest/googletest/include
LIBDIRS:=/Users/dmielke/Documents/oopl/trees/googletest/googletest/make

FILES :=                              \
    .travis.yml                       \
    collatz-tests/EID-RunCollatz.in   \
    collatz-tests/EID-RunCollatz.out  \
    collatz-tests/EID-TestCollatz.c++ \
    collatz-tests/EID-TestCollatz.out \
    Collatz.c++                       \
    Collatz.h                         \
    Collatz.log                       \
    html                              \
    RunCollatz.c++                    \
    RunCollatz.in                     \
    RunCollatz.out                    \
    TestCollatz.c++                   \
    TestCollatz.out                   \
    ColalatzBundle.c++

CXX        := g++
CXXFLAGS   := -pedantic -std=c++11 -Wall -I$(INCDIRS)
LDFLAGS    := -lgtest -lgtest_main -pthread -L$(LIBDIRS)
GCOV       := gcov-4.8
GCOVFLAGS  := -fprofile-arcs -ftest-coverage
GPROF      := gprof
GPROFFLAGS := -pg
VALGRIND   := valgrind

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f RunCollatz
	rm -f RunCollatz.tmp
	rm -f TestCollatz
	rm -f TestCollatz.tmp
	rm -f CollatzBundle

config:
	git config -l

bundle:
	cat Collatz.c++ RunCollatz.c++ > tmp
	sed -e "s/#include \"Collatz.h\"//g" tmp > CollatzBundle.c++
	rm tmp
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) CollatzBundle.c++ -o CollatzBundle

scrub:
	make  clean
	rm -f  Collatz.log
	rm -rf collatz-tests
	rm -rf html
	rm -rf latex

status:
	make clean
	@echo
	git branch
	git remote -v
	git status

#test: RunCollatz.tmp TestCollatz.tmp
test: RunCollatz.tmp

RunCollatz: Collatz.h Collatz.c++ RunCollatz.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) Collatz.c++ RunCollatz.c++ -o RunCollatz

RunCollatz.tmp: RunCollatz
	./RunCollatz < RunCollatz.in > RunCollatz.tmp
	diff RunCollatz.tmp RunCollatz.out

TestCollatz: Collatz.h Collatz.c++ TestCollatz.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) Collatz.c++ TestCollatz.c++ -o TestCollatz $(LDFLAGS)

TestCollatz.tmp: TestCollatz
	$(VALGRIND) ./TestCollatz                                       >  TestCollatz.tmp 2>&1
	$(GCOV) -b Collatz.c++     | grep -A 5 "File 'Collatz.c++'"     >> TestCollatz.tmp
	$(GCOV) -b TestCollatz.c++ | grep -A 5 "File 'TestCollatz.c++'" >> TestCollatz.tmp
	cat TestCollatz.tmp
