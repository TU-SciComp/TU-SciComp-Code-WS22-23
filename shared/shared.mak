.PHONY:		lint doc cppcheck valgrind coverage analyze check clean depend

LINT		=	pclp64_linux /opt/pclint/config/co-clang.lnt ../shared/shared.lnt
CPPCHECK	=	cppcheck --enable=all 
VALGRIND	=	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes
ANALYZER	=	scan-build-14
#
DOXY		=	doxygen
CPPFLAGS	=	-I.
CXXFLAGS	+=	-std=c++17 -Wall -Wextra -Wpedantic -Wshadow -D_GLIBCXX_DEBUG -g -O $(EXTRA_FLAGS)
CXXF_FAST	+=	-std=c++17 -g -Ofast -DNDEBUG -march=native $(EXTRA_FLAGS)
CXXF_COVERAGE	+=	-std=c++17 -g3 -Og --coverage $(EXTRA_FLAGS)

DCXX		=	g++ -MM

CXXSRC		= $(filter %.cpp, $(SOURCE))
OBJECT		= $(CXXSRC:.cpp=.o)

$(BINARY): 	$(OBJECT)
		$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

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
		lcov -d . -c >gcov/z.capture  
		lcov -d . -r gcov/z.capture "*11/*" >gcov/$(BINARY).capture
		genhtml -o gcov gcov/$(BINARY).capture
		-rm gcov/z.capture

analyze:
		make clean
		$(ANALYZER) make

check:
		-bash test.sh ./$(BINARY) 

clean:
		-rm -f $(OBJECT) $(BINARY) *.gcno *.gcda

depend:		$(SOURCE)
		$(SHELL) -ec '$(DCXX) $(CPPFLAGS) $(SOURCE) \
		| sed '\''s|^\([0-9A-Za-z\_]\{1,\}\)\.o|\1.o|g'\'' \
		>depend'

-include	depend

%.o:		%.cpp
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<




