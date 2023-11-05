cmake_minimum_required(VERSION 3.5)

project(UIChat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(SYSTEM cereal/include)

#add_subdirectory(ed25519)
include_directories(SYSTEM ed25519/src)

if (APPLE)
    add_definitions(-DBOOST_ASIO_HAS_STD_INVOKE_RESULT)
    include_directories(SYSTEM /usr/local/include)
    set(CMAKE_PREFIX_PATH "~/Qt/6.4.0/macos")
    set(CMAKE_PREFIX_PATH "~/Qt/6.4.0/macos")
    set(BOOST_INCLUDE_DIR "/usr/local/include")
    set(BOOST_LIB_DIR "/usr/local/lib")

    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
    set(OPENSSL_LIBRARIES "/usr/local/opt/openssl/lib")

    # for -lcrypto (I do not know who uses it!)
    link_directories(/usr/local/opt/openssl/lib)
endif()

if (WIN32)
    include_directories(SYSTEM ../../../Boost/boost_1_83_0/)
    set(CMAKE_PREFIX_PATH "C:/Qt6/6.6.0/msvc2019_64/lib/cmake")
    # for -lcrypto (I do not know who uses it!)
    #link_directories(/usr/local/opt/openssl/lib)
endif()

set(PROJECT_SOURCES
        main.cpp
	mainwindow.cpp
	mainwindow.h
	mainwindow.ui
	Settings.cpp
	Settings.h
	Utils.h
	Client/ChatClient.h
	Client/ClientInterfaces.h
	Client/TcpClient.h
	Client/Terminal.h
	Client/Protocol.h
	ed25519/src/add_scalar.cpp
	ed25519/src/fe.cpp
	ed25519/src/ge.cpp
	ed25519/src/key_exchange.cpp
	ed25519/src/keypair.cpp
	ed25519/src/sc.cpp
	ed25519/src/seed.cpp
	ed25519/src/sha512.cpp
	ed25519/src/sign.cpp
	ed25519/src/verify.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(UIChat
	MANUAL_FINALIZATION
	${PROJECT_SOURCES}
	CreateChatRoom.h CreateChatRoom.cpp CreateChatRoom.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Chat APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
	add_library(UIChat SHARED
	    ${PROJECT_SOURCES}
	)
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
	add_executable(UIChat
	    ${PROJECT_SOURCES}
	)
    endif()
endif()


target_link_libraries(UIChat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Chat ed25519/lib)
endif()
set_target_properties(UIChat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS UIChat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UIChat)
endif()
