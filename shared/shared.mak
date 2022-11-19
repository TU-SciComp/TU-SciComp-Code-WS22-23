.PHONY:		lint doc cppcheck valgrind coverage analyze check clean

LINT		=	pclp64_linux /opt/pclint/config/co-clang.lnt ../shared/shared.lnt
CPPCHECK	=	cppcheck --enable=all 
VALGRIND	=	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes
ANALYZER	=	scan-build-14
#
DOXY		=	doxygen
CPPFLAGS	=	-I.
CXXFLAGS	+=	-std=c++17 -Wall -Wextra -Wpedantic -D_GLIBCXX_DEBUG -g -O $(EXTRA_FLAGS)
CXXF_FAST	+=	-std=c++17 -g -Ofast -DNDEBUG -march=native $(EXTRA_FLAGS)
CXXF_COVERAGE	+=	-std=c++17 -g3 -Og --coverage $(EXTRA_FLAGS)

CXXSRC		= $(filter %.cpp, $(SOURCE))
OBJECT		= $(CXXSRC:.cpp=.o)

$(BINARY): 	$(OBJECT)
		$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

fast:
		make clean
		make CXXFLAGS="$(CXXF_FAST)"


lint:
		$(LINT) $(LINTFLAGS) $(CXXSRC)

doc:
		-mkdir doc
		$(DOXY) $(BINARY).dxy

cppcheck:
		$(CPPCHECK) $(CXXSRC)

valgrind:
		-bash test.sh "$(VALGRIND) ./$(BINARY)" 

coverage:
		-mkdir -p gcov
		lcov -d . -z
		make clean
		make CXX=g++ LINKCXX=g++ CXXFLAGS="$(CXXF_COVERAGE)"
		bash ./test.sh ./$(BINARY)
		lcov -d . -c >gcov/$(BINARY).capture  # --include  \*/knapsack/\* 
		genhtml -o gcov gcov/$(BINARY).capture
		-rm gcov/$(BINARY).capture

analyze:
		make clean
		$(ANALYZER) make

check:
		-bash test.sh ./$(BINARY) 

clean:
		-rm -f $(OBJECT) $(BINARY) *.gcno *.gcda

%.o:		%.cpp
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<




