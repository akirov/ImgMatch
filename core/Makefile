
BUILD_DIR=../$(BUILDDIR)

IMGMATCH_LIB=$(BUILD_DIR)/libimgmatch.a


SOURCES=$(wildcard *.cpp)

HEADERS=$(wildcard *.h)

OBJS=$(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

DEPEND=$(BUILD_DIR)/.depend


CXX=g++

CXXFLAGS+=-Wall


all: IMGMATCH_LIB


IMGMATCH_LIB: $(OBJS)
	ar r $(IMGMATCH_LIB) $(OBJS)
	ranlib $(IMGMATCH_LIB)


$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(DEPEND): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -MM $^ > $@

-include $(DEPEND)


clean:
	-rm -f $(BUILD_DIR)/*.o $(IMGMATCH_LIB) $(DEPEND)
