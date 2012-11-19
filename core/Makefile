
IMGMATCH_LIB = libimgmatch.a


SOURCES = $(wildcard *.cpp)

HEADERS = $(wildcard *.h)

OBJS = $(SOURCES:%.cpp=%.o)

DEPEND = .depend


CXX = g++

CFLAGS = -Wall


all: IMGMATCH_LIB


IMGMATCH_LIB: $(OBJS)
	ar r $(IMGMATCH_LIB) $(OBJS)
	ranlib $(IMGMATCH_LIB)


%.o: %.cpp
	$(CXX) -c $<


$(DEPEND): $(SOURCES) $(HEADERS)
	$(CXX) $(CFLAGS) -MM $^ > $@

-include $(DEPEND)


clean:
	-rm -f *.o $(IMGMATCH_LIB) $(DEPEND)
