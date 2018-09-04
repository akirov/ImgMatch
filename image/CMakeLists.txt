cmake_minimum_required(VERSION 3.1.0)


include_directories (${CMAKE_CURRENT_SOURCE_DIR}
                     ${COMMON_DIR})


if (${IMAGEIMP} MATCHES "OCVIMAGE")
  include_directories (${OpenCV_INCLUDE_DIRS})
  set (IMG_SOURCES OCVImage.cpp)
elseif (${IMAGEIMP} MATCHES "QTIMAGE")
  if (Qt5Core_FOUND)
    include_directories (${Qt5Gui_INCLUDE_DIRS})
  else (Qt5Core_FOUND)
    include_directories (${QT_INCLUDES})
  endif (Qt5Core_FOUND)
  set (IMG_SOURCES QtImage.cpp)
else (${IMAGEIMP} MATCHES "OCVIMAGE")
  message(FATAL_ERROR "Unknown IMAGEIMP=${IMAGEIMP}!")
endif (${IMAGEIMP} MATCHES "OCVIMAGE")


set (IMG_SOURCES ${IMG_SOURCES} Image.cpp)


add_library (imageimp STATIC ${IMG_SOURCES})

target_link_libraries(imageimp common)


#install (TARGETS imageimp DESTINATION ${INSTALL_DIR})
