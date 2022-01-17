set(CMAKE_AUTOMOC ON)

#qt
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
# mpv
find_package(PkgConfig REQUIRED)
pkg_check_modules(MPV REQUIRED mpv)

add_executable(MusicPlayer main.cpp player.cpp player.hpp)

target_include_directories(
  MusicPlayer
  PRIVATE
  Qt5::Gui
  Qt5::Core
  Qt5::Widgets
  ${MPV_INCLUDES}
)
target_link_libraries(
  MusicPlayer
  PRIVATE
  Qt5::Gui
  Qt5::Core
  Qt5::Widgets
  ${MPV_LIBRARIES}
)
