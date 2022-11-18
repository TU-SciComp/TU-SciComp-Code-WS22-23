.PHONY:		doc clean

DOXY		=	doxygen
CXXFLAGS	+=	-std=c++17 -Wall -Wextra -g -O $(EXTRA_FLAGS)
OBJECT		=	$(SOURCE:.cpp=.o)

$(BINARY): $(OBJECT)
	$(CXX) $(LDFLAGS) $^ -o $@

doc:
	-mkdir doc
	$(DOXY) $(BINARY).dxy

clean:
	-rm $(OBJECT) $(BINARY)

%.o:	%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<




