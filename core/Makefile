
CXX=g++
RM=rm -f


BUILD_DIR=../$(BUILDDIR)

IMGMATCH_LIB=$(BUILD_DIR)/libimgmatch.a


SOURCES=$(wildcard *.cpp)

HEADERS=$(wildcard *.h)

OBJS=$(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

DEPEND=$(BUILD_DIR)/.depend


CXXFLAGS+=-Wall


all: $(IMGMATCH_LIB)


$(IMGMATCH_LIB): $(OBJS)
	ar r $(IMGMATCH_LIB) $(OBJS)
	ranlib $(IMGMATCH_LIB)


$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@


$(DEPEND): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $^ > $@

-include $(DEPEND)


clean:
	-$(RM) $(BUILD_DIR)/*.o $(IMGMATCH_LIB) $(DEPEND)
