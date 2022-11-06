#
# \file      Makefile
# \brief     Recursive Decent Expression Parser 
# \author    Thorsten Koch
# \version   1.1
# \date      09Feb2020-11Dec2021
# \copyright Copyright (C) 2022 by Thorsten Koch <koch@zib.de>,
#            licened under LGPL version 3 or later
#
.PHONY:		all clean depend fast lint doc doxygen check valgrind coverage

NAME		=	expreval
CXX		=	g++
LINKCXX   	=	g++
DCXX		=	g++
LINT		=	pclp64_linux # _debug
CPPCHECK	=	cppcheck --enable=all 
DOXY		=	doxygen
VALGRIND	=	valgrind --tool=memcheck --leak-check=full \
			--leak-resolution=high --show-reachable=yes
ANALYZER	=	scan-build-10
#
LINTFLAGS	=	/opt/pclint/config/co-gcc.lnt $(NAME).lnt
#
CPPFLAGS	=	-I.
CXXFLAGS	=	-std=c++17 -g3 -Og -Wall -Wextra  
CXXF_FAST	=	-std=c++17 -g -O3 -DNDEBUG -march=native 
CXXF_COVERAGE	=	-std=c++17 -g3 -Og --coverage
CXXF_CHECK	=	-std=c++17 -g3 -Og -Wall -Wextra \
			-Wsuggest-attribute=pure \
			-Wsuggest-attribute=const \
			-Wsuggest-attribute=noreturn \
			-Wsuggest-attribute=malloc \
			-Wsuggest-attribute=format \
			-Walloc-zero \
			-Wcast-align \
			-Wduplicated-branches \
			-Wduplicated-cond \
			-Winline \
			-Wlogical-op \
			-Wmissing-declarations \
			-Wmissing-noreturn \
			-Wnull-dereference \
			-Wpointer-arith \
			-Wredundant-decls \
			-Wshadow \
			-Wstrict-overflow=4 \
			-Wunsafe-loop-optimizations \
			-Wwrite-strings \
			-fno-omit-frame-pointer \
			-fstack-protector-strong \
			-fsanitize=address \
			-fsanitize=undefined \
			-fsanitize=shift \
			-fsanitize=shift-exponent \
			-fsanitize=shift-base \
			-fsanitize=integer-divide-by-zero \
			-fsanitize=unreachable \
			-fsanitize=vla-bound \
			-fsanitize=null \
			-fsanitize=bounds \
			-fsanitize=alignment \
			-fsanitize=object-size \
			-fsanitize=float-cast-overflow \
			-fsanitize=nonnull-attribute \
			-fsanitize=returns-nonnull-attribute \
			-fsanitize=bool \
			-fsanitize=enum \
			-fsanitize=signed-integer-overflow \
			-fsanitize=pointer-overflow \
			-fsanitize=builtin
#	-Wcast-qual because of strtod\
#	-fsanitize=float-divide-by-zero \
#	-fsanitize=bounds-strict \

DFLAGS		=	-MM
LDFLAGS		=
LDLIBS		=	-lm
OBJECT		=	expreval.o testit.o
SOURCE		=	$(OBJECT:.o=.cpp)
DEPEND		=	depend
BINARY          =	testit

$(BINARY): 	$(OBJECT)
		$(LINKCXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

fast:
		make clean
		make CXXFLAGS="$(CXXF_FAST)"

lint:
		$(LINT) $(LINTFLAGS) $(SOURCE)

cppcheck:
		$(CPPCHECK) $(SOURCE)

doxygen:
		-mkdir doc
		$(DOXY) $(NAME).dxy

valgrind:
		$(VALGRIND) ./$(BINARY) <test.txt
		$(VALGRIND) ./$(BINARY) <test2.txt

coverage:
#		-ln -s ../../src $(OBJDIR)
		-mkdir -p gcov
		lcov -d . -z
		make clean
		make CXX=g++ LINKCXX=g++ CXXFLAGS="$(CXXF_COVERAGE)"
		./testit <test.txt
		./testit -h
		./testit "12*7+3"
		lcov -d . -c --include  \*/expreval/\* >gcov/$NAME.capture
		genhtml -o gcov gcov/$NAME.capture
		-rm gcov/$NAME.capture

analyze:
		make clean
		$(ANALYZER) make

check:
		make clean
		make CXXFLAGS="$(CXXF_CHECK)"
		export GLIBC_TUNABLES=glibc.malloc.check=3
		./$(BINARY) <test.txt 2>&1 | diff -s - test.out.ok

clean:
		-rm -f *.d *.o *.gcno *.gcda depend $(BINARY)

depend:		$(SOURCE)
		$(SHELL) -ec '$(DCXX) $(DFLAGS) $(CPPFLAGS) $(SOURCE) \
		| sed '\''s|^\([0-9A-Za-z\_]\{1,\}\)\.o|\1.o|g'\'' \
		>$(DEPEND)'

-include	$(DEPEND)




