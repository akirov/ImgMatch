cmake_minimum_required(VERSION 3.1.0)


include_directories (${CMAKE_CURRENT_SOURCE_DIR}
                     ${COMMON_DIR})


if (MOD_SCALE)
  set (MOD_SOURCES ${MOD_SOURCES} ModScale.cpp)
endif (MOD_SCALE)

if (MOD_OCV_SIFT)
  include_directories (${OpenCV_INCLUDE_DIRS})
  set (MOD_SOURCES ${MOD_SOURCES} ModOcvSIFT.cpp)
endif (MOD_OCV_SIFT)


add_library (modules STATIC ${MOD_SOURCES})

target_link_libraries(modules common imageimp)

#if (MOD_OCV_SIFT)
#  target_link_libraries(modules ${OpenCV_LIBS})
#endif (MOD_OCV_SIFT)


#install (TARGETS modules DESTINATION ${INSTALL_DIR})
