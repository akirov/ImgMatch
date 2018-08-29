
ifeq ($(OS),Windows_NT)
  ifeq ($(shell echo $$OSTYPE),cygwin)
    RM=rm -f
    BUILD_DIR=../$(BUILDDIR)/
  else
    RM=del /F /Q
    BUILD_DIR=..\$(BUILDDIR)\\
  endif
else
  RM=rm -f
  BUILD_DIR=../$(BUILDDIR)/
endif

CXX=g++

CXXFLAGS+=-Wall


IMGMATCH_LIB=$(BUILD_DIR)libimgmatch.a

#SOURCES=$(wildcard *.cpp)
SOURCES=Logger.cpp ModScale.cpp

#HEADERS=$(wildcard *.h)
HEADERS=Image.h ImgMatch.h Logger.h ModScale.h

ifneq (,$(findstring OPENCV, $(SIFT_IMP)))
    SOURCES+=ModOcvSIFT.cpp
    HEADERS+=ModOcvSIFT.h
endif

OBJS=$(SOURCES:%.cpp=$(BUILD_DIR)%.o)

DEPEND=$(BUILD_DIR).depend


all: $(IMGMATCH_LIB)


$(IMGMATCH_LIB): $(OBJS)
	ar r $(IMGMATCH_LIB) $(OBJS)
	ranlib $(IMGMATCH_LIB)


$(DEPEND): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $^ > $@

-include $(DEPEND)


$(BUILD_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@


clean:
	-$(RM) $(BUILD_DIR)*.o $(IMGMATCH_LIB) $(DEPEND)
