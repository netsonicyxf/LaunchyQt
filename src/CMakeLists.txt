
cmake_minimum_required (VERSION 3.14)

project ("Launchy")

set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_BUILD_PARALLEL_LEVEL 4)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

get_filename_component (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ ABSOLUTE)
get_filename_component (DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/ ABSOLUTE)
get_filename_component (TRANS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../translations/ ABSOLUTE)

message (STATUS "SRC_DIR: ${SRC_DIR}")
message (STATUS "DEPS_DIR: ${DEPS_DIR}")
message (STATUS "TRANS_DIR: ${TRANS_DIR}")

include_directories (${SRC_DIR} ${DEPS_DIR})

##################################################
# Qt
# add CMAKE_PREFIX_PATH = <qt installed dir> in system PATH
# eg c:/Qt/Qt5.12.10/5.12.10/msvc2017
set (QT_MAJOR_VERSION 5)
if (WIN32)
  if (NOT DEFINED ENV{CMAKE_PREFIX_PATH})
    if (QT_MAJOR_VERSION EQUAL 5)
      set (ENV{CMAKE_PREFIX_PATH} "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64")
    else ()
      set (ENV{CMAKE_PREFIX_PATH} "D:/SDK/Qt/6.5.3/msvc2019_64")
    endif ()
  endif ()
  message (STATUS "CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH}")
  find_package (Qt${QT_MAJOR_VERSION} COMPONENTS Widgets Network LinguistTools WinExtras REQUIRED)
else ()
  find_package (Qt${QT_MAJOR_VERSION} COMPONENTS Widgets Network LinguistTools REQUIRED)
  find_package (PkgConfig REQUIRED)
  pkg_check_modules (X11 x11 xcb REQUIRED)
endif ()

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

# source group for qt moc files
set_property (GLOBAL PROPERTY AUTOUIC_SOURCE_GROUP "Generated Files")
set_property (GLOBAL PROPERTY AUTOMOC_SOURCE_GROUP "Generated Files")
set_property (GLOBAL PROPERTY AUTORCC_SOURCE_GROUP "Generated Files")

# link qtmain.lib
if (POLICY CMP0020)
  cmake_policy (SET CMP0020 NEW)
endif ()

# hide console in release
if (WIN32)
  set (CMAKE_WIN32_EXECUTABLE TRUE)
endif ()

##################################################
# Python
if (WIN32)
  # set (ENV{Python_ROOT_DIR} "C:/Program Files/Python310")
  message (STATUS "Python_ROOT_DIR: $ENV{Python_ROOT_DIR}")
endif ()

find_package (Python COMPONENTS Interpreter Development REQUIRED)

message ("Python_FOUND: ${Python_FOUND}")
message ("Python_VERSION: ${Python_VERSION}")
message ("Python_Development_FOUND: ${Python_Development_FOUND}")
message ("Python_INCLUDE_DIRS:" ${Python_INCLUDE_DIRS})
message ("Python_LIBRARIES: ${Python_LIBRARIES}")

##################################################
# Platform
if (MSVC)
  add_compile_options (/W3 /utf-8)
  # add_compile_options (/wd4819) # disable warning C4819: unicode charset
  # add_compile_options (/MP) # multi-processor compilation

  add_compile_definitions (_CRT_SECURE_NO_WARNINGS)
  add_compile_definitions (NOMINMAX)
  # add_compile_definitions (WIN32_LEAN_AND_MEAN)

#  add_link_options (/DEBUG) # generate debug info in release config

#  set (CMAKE_EXE_LINKER_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
#  set (CMAKE_EXE_LINKER_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")

  set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

else ()
  add_compile_options (-Wall -Wextra)
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

  set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ~/Launchy)
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ~/Launchy)
endif ()


##################################################
# Launchy
add_executable (Launchy)

# source files
set (SOURCES_LAUNCHY
  ${SRC_DIR}/Launchy/AnimationLabel.h
  ${SRC_DIR}/Launchy/AnimationLabel.cpp
  ${SRC_DIR}/Launchy/AppBase.h
  ${SRC_DIR}/Launchy/AppBase.cpp
  ${SRC_DIR}/Launchy/Catalog.h
  ${SRC_DIR}/Launchy/Catalog.cpp
  ${SRC_DIR}/Launchy/CatalogBuilder.h
  ${SRC_DIR}/Launchy/CatalogBuilder.cpp
  ${SRC_DIR}/Launchy/CharLineEdit.h
  ${SRC_DIR}/Launchy/CharLineEdit.cpp
  ${SRC_DIR}/Launchy/CharListWidget.h
  ${SRC_DIR}/Launchy/CharListWidget.cpp
  ${SRC_DIR}/Launchy/CommandHistory.h
  ${SRC_DIR}/Launchy/CommandHistory.cpp
  ${SRC_DIR}/Launchy/Directory.h
  ${SRC_DIR}/Launchy/Directory.cpp
  ${SRC_DIR}/Launchy/DropListWidget.h
  ${SRC_DIR}/Launchy/DropListWidget.cpp
  ${SRC_DIR}/Launchy/Fader.h
  ${SRC_DIR}/Launchy/Fader.cpp
  ${SRC_DIR}/Launchy/FileBrowser.h
  ${SRC_DIR}/Launchy/FileBrowser.cpp
  ${SRC_DIR}/Launchy/FileBrowserDelegate.h
  ${SRC_DIR}/Launchy/FileBrowserDelegate.cpp
  ${SRC_DIR}/Launchy/FileSearch.h
  ${SRC_DIR}/Launchy/FileSearch.cpp
  ${SRC_DIR}/Launchy/GlobalVar.h
  ${SRC_DIR}/Launchy/GlobalVar.cpp
  ${SRC_DIR}/Launchy/IconDelegate.h
  ${SRC_DIR}/Launchy/IconDelegate.cpp
  ${SRC_DIR}/Launchy/IconExtractor.h
  ${SRC_DIR}/Launchy/IconExtractor.cpp
  ${SRC_DIR}/Launchy/IconProviderBase.h
  ${SRC_DIR}/Launchy/IconProviderBase.cpp
  ${SRC_DIR}/Launchy/InputDataList.h
  ${SRC_DIR}/Launchy/InputDataList.cpp
  ${SRC_DIR}/Launchy/LaunchyVersion.h
  ${SRC_DIR}/Launchy/LaunchyWidget.h
  ${SRC_DIR}/Launchy/LaunchyWidget.cpp
  ${SRC_DIR}/Launchy/Logger.h
  ${SRC_DIR}/Launchy/Logger.cpp
  ${SRC_DIR}/Launchy/main.cpp
  ${SRC_DIR}/Launchy/OptionDialog.h
  ${SRC_DIR}/Launchy/OptionDialog.cpp
  ${SRC_DIR}/Launchy/OptionItem.h
  ${SRC_DIR}/Launchy/OptionItem.cpp
  ${SRC_DIR}/Launchy/PluginHandler.h
  ${SRC_DIR}/Launchy/PluginHandler.cpp
  ${SRC_DIR}/Launchy/SettingsManager.h
  ${SRC_DIR}/Launchy/SettingsManager.cpp
  ${SRC_DIR}/Launchy/TranslationManager.h
  ${SRC_DIR}/Launchy/TranslationManager.cpp
  ${SRC_DIR}/Launchy/UpdateChecker.h
  ${SRC_DIR}/Launchy/UpdateChecker.cpp
)


# sources files for operation system
if (WIN32)
  file (GLOB SOURCES_PLATFORM_LAUNCHY
    ${SRC_DIR}/Launchy/Windows/*.h
    ${SRC_DIR}/Launchy/Windows/*.cpp
    ${SRC_DIR}/Launchy/Windows/launchy.rc)
  source_group ("Resource Files" FILES ${SRC_DIR}/Launchy/Windows/launchy.rc)
  target_include_directories (Launchy PRIVATE ${SRC_DIR}/Launchy/Windows)
else ()
  file (GLOB SOURCES_PLATFORM_LAUNCHY
    ${SRC_DIR}/Launchy/Linux/*.h
    ${SRC_DIR}/Launchy/Linux/*.cpp)
  target_include_directories (Launchy PRIVATE ${SRC_DIR}/Launchy/Linux)
endif (WIN32)

list (APPEND SOURCES_LAUNCHY ${SOURCES_PLATFORM_LAUNCHY})
target_include_directories (Launchy PRIVATE ${SRC_DIR}/Launchy)

# qt source files
file (GLOB SOURCES_QT_LAUNCHY
  ${SRC_DIR}/Launchy/*.ui
  ${SRC_DIR}/Launchy/*.qrc
  ${SRC_DIR}/Launchy/*.ts)
source_group ("Qt Files" FILES ${SOURCES_QT_LAUNCHY})
list (APPEND SOURCES_LAUNCHY ${SOURCES_QT_LAUNCHY})

target_sources (Launchy
  PRIVATE ${SOURCES_LAUNCHY}
  PRIVATE ${SOURCES_SINGLEAPP}
  PRIVATE ${SOURCES_HOTKEY})

if (WIN32)
target_link_libraries (Launchy
  LaunchyLib PluginPy
  SingleApplication QHotKey
  userenv netapi32
  Qt${QT_MAJOR_VERSION}::Widgets Qt${QT_MAJOR_VERSION}::Network Qt${QT_MAJOR_VERSION}::WinExtras)
else ()
target_link_libraries (Launchy
  LaunchyLib PluginPy
  SingleApplication QHotKey
  Qt${QT_MAJOR_VERSION}::Widgets Qt${QT_MAJOR_VERSION}::Network Qt${QT_MAJOR_VERSION}::GuiPrivate)
endif ()

##################################################
# LaunchyLib
# source files
add_library (LaunchyLib SHARED)

set_target_properties(LaunchyLib PROPERTIES
  OUTPUT_NAME "Launchy")

target_sources (LaunchyLib
  PRIVATE
  ${SRC_DIR}/LaunchyLib/CatalogItem.h
  ${SRC_DIR}/LaunchyLib/CatalogItem.cpp
  ${SRC_DIR}/LaunchyLib/InputData.h
  ${SRC_DIR}/LaunchyLib/InputData.cpp
  ${SRC_DIR}/LaunchyLib/LaunchyLib.h
  ${SRC_DIR}/LaunchyLib/LaunchyLib.cpp
  ${SRC_DIR}/LaunchyLib/PluginInfo.h
  ${SRC_DIR}/LaunchyLib/PluginInfo.cpp
  ${SRC_DIR}/LaunchyLib/PluginInterface.h
  ${SRC_DIR}/LaunchyLib/PluginInterface.cpp
  ${SRC_DIR}/LaunchyLib/PluginMsg.h
  ${SRC_DIR}/LaunchyLib/UnicodeTable.cpp)

target_link_libraries (LaunchyLib
  PRIVATE Qt${QT_MAJOR_VERSION}::Core Qt${QT_MAJOR_VERSION}::Widgets)

##################################################
# PluginPy
add_library (PluginPy SHARED)

target_sources (PluginPy
  PRIVATE
  ${SRC_DIR}/PluginPy/ExportPy.cpp
  ${SRC_DIR}/PluginPy/ExportPyCatItem.h
  ${SRC_DIR}/PluginPy/ExportPyCatItem.cpp
  ${SRC_DIR}/PluginPy/ExportPyInputData.h
  ${SRC_DIR}/PluginPy/ExportPyInputData.cpp
  ${SRC_DIR}/PluginPy/ExportPyPlugin.h
  ${SRC_DIR}/PluginPy/PluginLoader.h
  ${SRC_DIR}/PluginPy/PluginLoader.cpp
  ${SRC_DIR}/PluginPy/PluginMgr.h
  ${SRC_DIR}/PluginPy/PluginMgr.cpp
  ${SRC_DIR}/PluginPy/PluginPy.h
  ${SRC_DIR}/PluginPy/PluginWrapper.h
  ${SRC_DIR}/PluginPy/PluginWrapper.cpp)

target_include_directories (PluginPy
  PRIVATE ${Python_INCLUDE_DIRS})

target_link_libraries (PluginPy
  PUBLIC LaunchyLib
  PRIVATE Qt${QT_MAJOR_VERSION}::Core Qt${QT_MAJOR_VERSION}::Widgets
  PRIVATE ${Python_LIBRARIES})

add_custom_command (TARGET PluginPy POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:PluginPy> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/python/launchy.pyd)

# pluginpy util
add_custom_target (PluginPy.Util
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${SRC_DIR}/PluginPy/launchy_util.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/python
  COMMENT "copy launchy_util.py to target directory"
  SOURCES ${SRC_DIR}/PluginPy/launchy_util.py)

##################################################
# Launchy main translation
file (GLOB TS_FILES_LAUNCHY
  ${TRANS_DIR}/*.ts)

#qt_create_translation(QM_FILES_LAUNCHY
#  ${SRC_DIR}/Launchy
#  ${SRC_DIR}/LaunchyLib
#  ${SRC_DIR}/PluginPy
#  ${TS_FILE_LAUNCHY})

#qt_add_translations(Launchy
#  TS_FILES ${TS_FILES_LAUNCHY}
#  QM_FILES_OUTPUT_VARIABLE QM_FILES_LAUNCHY
#  SOURCES ${SOURCES_LAUNCHY})

if (QT_MAJOR_VERSION EQUAL 6)

qt_add_lupdate(Launchy
  TS_FILES ${TS_FILES_LAUNCHY}
  OPTIONS NO_GLOBAL_TARGET)

qt_add_lrelease(Launchy
  TS_FILES ${TS_FILES_LAUNCHY}
  QM_FILES_OUTPUT_VARIABLE QM_FILES_LAUNCHY
  OPTIONS NO_GLOBAL_TARGET)

#set_source_files_properties(${TS_FILES_LAUNCHY} PROPERTIES
#  OUTPUT_LOCATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations)

# add_custom_target (LaunchyQM
#  DEPENDS ${QM_FILES_LAUNCHY})

message (STATUS "QM_FILES_LAUNCHY: ${QM_FILES_LAUNCHY}")
add_custom_command (TARGET Launchy_lrelease POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${QM_FILES_LAUNCHY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/translations)

endif () # QT_MAJOR_VERSION

##################################################
# QSingleApplication
set(QAPPLICATION_CLASS QApplication)
add_subdirectory(${DEPS_DIR}/SingleApplication SingleApplication)

##################################################
# QHotKey
add_library (QHotKey STATIC)

target_sources (QHotKey PRIVATE ${DEPS_DIR}/QHotkey/QHotkey.cpp)

if (WIN32)
  target_sources(QHotKey PRIVATE ${DEPS_DIR}/QHotkey/qhotkey_win.cpp)
else ()
  find_package(X11 REQUIRED)
  include_directories(${X11_INCLUDE_DIR})
  target_link_libraries(QHotKey PRIVATE ${X11_LIBRARIES})
  target_sources(QHotKey PRIVATE ${DEPS_DIR}/QHotkey/qhotkey_x11.cpp)
endif ()

if (WIN32)
target_link_libraries (QHotKey
  PUBLIC Qt${QT_MAJOR_VERSION}::Core
  PRIVATE Qt${QT_MAJOR_VERSION}::Widgets)
else ()
target_link_libraries (QHotKey
  PUBLIC Qt${QT_MAJOR_VERSION}::Core
  PRIVATE Qt${QT_MAJOR_VERSION}::Widgets X11 xcb)
endif ()

##################################################
# plugin: Runner
add_library (Plugin.Runner MODULE)

set_target_properties(Plugin.Runner PROPERTIES
  OUTPUT_NAME "Runner")

file (GLOB SOURCES_RUNNER
  ${SRC_DIR}/Plugins/Runner/*.h
  ${SRC_DIR}/Plugins/Runner/*.cpp)

# qt source files
file (GLOB SOURCES_QT_RUNNER
  ${SRC_DIR}/Plugins/Runner/*.ui
  ${SRC_DIR}/Plugins/Runner/*.qrc
  ${SRC_DIR}/Plugins/Runner/*.ts)
source_group ("Qt Files" FILES ${SOURCES_QT_RUNNER})
list (APPEND SOURCES_RUNNER ${SOURCES_QT_RUNNER})

target_sources (Plugin.Runner
  PRIVATE ${SOURCES_RUNNER})

target_link_libraries (Plugin.Runner
  PUBLIC Qt${QT_MAJOR_VERSION}::Core
  PRIVATE Qt${QT_MAJOR_VERSION}::Widgets
  PRIVATE LaunchyLib)

set_target_properties (Plugin.Runner PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug/plugins/Runner"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release/plugins/Runner")

file (GLOB RESOURCES_RUNNER
  ${SRC_DIR}/Plugins/Runner/*.png)

add_custom_command (TARGET Plugin.Runner POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${RESOURCES_RUNNER} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$<CONFIG>/plugins/Runner)

##################################################
if (WIN32)
# plugin: Tasky
add_library (Plugin.Tasky MODULE)

set_target_properties(Plugin.Tasky PROPERTIES
  OUTPUT_NAME "Tasky")

file (GLOB SOURCES_TASKY
  ${SRC_DIR}/Plugins/Tasky/*.h
  ${SRC_DIR}/Plugins/Tasky/*.cpp)

# qt source files
file (GLOB SOURCES_QT_TASKY
  ${SRC_DIR}/Plugins/Tasky/*.ui
  ${SRC_DIR}/Plugins/Tasky/*.qrc
  ${SRC_DIR}/Plugins/Tasky/*.ts)
source_group ("Qt Files" FILES ${SOURCES_QT_TASKY})
list (APPEND SOURCES_TASKY ${SOURCES_QT_TASKY})

target_sources (Plugin.Tasky
  PRIVATE ${SOURCES_TASKY})

target_link_libraries (Plugin.Tasky
  PUBLIC Qt${QT_MAJOR_VERSION}::Core
  PRIVATE Qt${QT_MAJOR_VERSION}::Widgets
  PRIVATE Qt${QT_MAJOR_VERSION}::WinExtras
  PRIVATE LaunchyLib)

set_target_properties (Plugin.Tasky PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug/plugins/Tasky"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release/plugins/Tasky")

file (GLOB RESOURCES_TASKY
  ${SRC_DIR}/Plugins/Tasky/*.png)

add_custom_command (TARGET Plugin.Tasky POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${RESOURCES_TASKY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$<CONFIG>/plugins/Tasky)
endif ()
##################################################
if (WIN32)
# plugin: UWPApp
add_library (Plugin.UWPApp MODULE)

set_target_properties(Plugin.UWPApp PROPERTIES
  OUTPUT_NAME "UWPApp")

file (GLOB SOURCES_UWPAPP
  ${SRC_DIR}/Plugins/UWPApp/*.h
  ${SRC_DIR}/Plugins/UWPApp/*.cpp)

# qt source files
file (GLOB SOURCES_QT_UWPAPP
  ${SRC_DIR}/Plugins/UWPApp/*.ui
  ${SRC_DIR}/Plugins/UWPApp/*.qrc
  ${SRC_DIR}/Plugins/UWPApp/*.ts)
source_group ("Qt Files" FILES ${SOURCES_QT_UWPAPP})
list (APPEND SOURCES_UWPAPP ${SOURCES_QT_UWPAPP})

target_sources (Plugin.UWPApp
  PRIVATE ${SOURCES_UWPAPP})

target_link_libraries (Plugin.UWPApp
  PUBLIC Qt${QT_MAJOR_VERSION}::Core
  PRIVATE Qt${QT_MAJOR_VERSION}::Widgets
  PRIVATE LaunchyLib
  PRIVATE user32 shell32 propsys)

set_target_properties (Plugin.UWPApp PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug/plugins/UWPApp"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release/plugins/UWPApp")
endif ()
##################################################
# plugin: Verby
add_library (Plugin.Verby MODULE)

set_target_properties(Plugin.Verby PROPERTIES
  OUTPUT_NAME "Verby")

file (GLOB SOURCES_VERBY
  ${SRC_DIR}/Plugins/Verby/*.h
  ${SRC_DIR}/Plugins/Verby/*.cpp)

# qt source files
file (GLOB SOURCES_QT_VERBY
  ${SRC_DIR}/Plugins/Verby/*.ui
  ${SRC_DIR}/Plugins/Verby/*.qrc
  ${SRC_DIR}/Plugins/Verby/*.ts)
source_group ("Qt Files" FILES ${SOURCES_QT_VERBY})
list (APPEND SOURCES_VERBY ${SOURCES_QT_VERBY})

target_sources (Plugin.Verby
  PRIVATE ${SOURCES_VERBY})

target_link_libraries (Plugin.Verby
  PUBLIC Qt${QT_MAJOR_VERSION}::Core
  PRIVATE Qt${QT_MAJOR_VERSION}::Widgets
  PRIVATE LaunchyLib)

set_target_properties (Plugin.Verby PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug/plugins/Verby"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release/plugins/Verby")

file (GLOB RESOURCES_VERBY
  ${SRC_DIR}/Plugins/Verby/*.png)

add_custom_command (TARGET Plugin.Verby POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${RESOURCES_VERBY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$<CONFIG>/plugins/Verby)

##################################################
# plugin: CalcPy
add_custom_target (PluginPy.CalcPy
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${SRC_DIR}/Plugins/CalcyPy ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$<CONFIG>/plugins/CalcyPy
  COMMENT "copy calcpy to plugin directory")

##################################################
# plugin: PydiryPy
add_custom_target (PluginPy.PydiryPy
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${SRC_DIR}/Plugins/PydiryPy ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$<CONFIG>/plugins/PydiryPy)

##################################################
# plugin: WebSearchPy
add_custom_target (PluginPy.WebSearchPy
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${SRC_DIR}/Plugins/WebSearchPy ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$<CONFIG>/plugins/WebSearchPy)
