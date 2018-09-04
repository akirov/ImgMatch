cmake_minimum_required(VERSION 3.1.0)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories (${COMMON_DIR} ${IMAGE_DIR} ${MODULES_DIR})


set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTORCC ON)


qt5_wrap_ui(UI_HEADERS ImgMatchUI.ui)


if (WIN32)
  set(GUI_TYPE WIN32)
endif(WIN32)

add_executable(imgmatch ${GUI_TYPE} main.cpp ImgMatchUI.cpp ${UI_HEADERS})

target_link_libraries(imgmatch common imageimp modules)

if (Qt5Core_FOUND)
  include_directories (${Qt5Gui_INCLUDE_DIRS})
  target_link_libraries(imgmatch Qt5::Core Qt5::Gui Qt5::Widgets)
else (Qt5Core_FOUND)
  # Qt4
  include_directories (${QT_INCLUDES})
  target_link_libraries(imgmatch Qt4::QtGui)
endif (Qt5Core_FOUND)

if (MOD_OCV_SIFT OR ${IMAGEIMP} MATCHES "OCVIMAGE")
  target_link_libraries(imgmatch ${OpenCV_LIBS})
endif (MOD_OCV_SIFT OR ${IMAGEIMP} MATCHES "OCVIMAGE")


install (TARGETS imgmatch DESTINATION ${INSTALL_DIR})
