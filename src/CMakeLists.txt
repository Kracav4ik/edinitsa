# QTDIR must point to c:/Qt/5.7/mingw53_32/ or similar
message("qtdir is set to '$ENV{QTDIR}' (must be 'c:/Qt/5.7/mingw53_32/' or similar)")
set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

set(CLIENT_NAME "edinitsa")
set(SERVER_NAME "edinitsa_server")

set(COMMON_SOURCES transport.cpp transport.h enums.h messages.h)
add_library(edinitsa_common ${COMMON_SOURCES})
target_link_libraries(edinitsa_common Qt5::Core)

set(CLIENT_SOURCES
        main_client.cpp
        edinitsa.ui
        ClientMainWindow.cpp
        ClientMainWindow.h
        widgets/CanvasWidget.cpp
        widgets/CanvasWidget.h
        )

set(SERVER_SOURCES
        main_server.cpp
        edinitsa_server.ui
        ServerMainWindow.cpp
        ServerMainWindow.h
        )

add_executable(${CLIENT_NAME} ${CLIENT_SOURCES})
target_link_libraries(${CLIENT_NAME} edinitsa_common Qt5::Widgets Qt5::Core Qt5::Network)

add_executable(${SERVER_NAME} ${SERVER_SOURCES})
target_link_libraries(${SERVER_NAME} edinitsa_common Qt5::Widgets Qt5::Core Qt5::Network)
