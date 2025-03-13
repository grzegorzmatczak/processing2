cmake_minimum_required (VERSION 3.8)
project(processing2
    VERSION 0.0.1
    DESCRIPTION "processing2"
    LANGUAGES C CXX
)
set(TARGET_NAME "processing2")
include(GNUInstallDirs)

if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd /")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT ")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_OPTIONS}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_PREFIX_PATH ${utils2_DIR_CUSTOM})
find_package(utils2 REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${Qt5_DIR})
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

list(APPEND CMAKE_PREFIX_PATH ${gtest_DIR})
find_package(GTest REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${OpenCV_DIR})
find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc imgcodecs videoio video tracking bgsegm)
#find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
MESSAGE(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")

file(GLOB_RECURSE ADDER_SRC src/Adder/*.cc src/Adder/*.c)
file(GLOB_RECURSE ADDER_HEADERS src/Adder/*.h src/Adder/*.hpp)

file(GLOB_RECURSE FILTER_SRC src/Filter/*.cc src/Filter/*.c)
file(GLOB_RECURSE FILTER_HEADERS src/Filter/*.h src/Filter/*.hpp)

file(GLOB_RECURSE SUBTRACTOR_SRC src/Subtractor/*.cc src/Subtractor/*.c)
file(GLOB_RECURSE SUBTRACTOR_HEADERS src/Subtractor/*.h src/Subtractor/*.hpp)

set(SRC
    src/processing.cc
    ${ADDER_SRC}
    ${FILTER_SRC}
    ${SUBTRACTOR_SRC}
)

set(HEADERS
    include/processing.h
    include/processing_global.h
    ${ADDER_HEADERS}
    ${FILTER_HEADERS}
    ${SUBTRACTOR_HEADERS}
)

set(SRC_HEADERS_TEST
	gtest/gtest.cc
    gtest/gtest_filter_custom_addmultipledron.cc
    gtest/gtest_filter_custom_addmultipledron.h
    gtest/gtest_filter_custom_velocityfilter.cc
    gtest/gtest_filter_custom_velocityfilter.h
    gtest/gtest_filter_custom_viterbi.cc
    gtest/gtest_filter_custom_viterbi.h
    gtest/gtest_filter_median_impl_2.cc
    gtest/gtest_filter_median_impl_2.h
)

include_directories( "src" )
include_directories( "include" )
include_directories( "src/Subtractor" )
include_directories( "src/Subtractor/subtractors" )
include_directories( "src/Filter" )
include_directories( "src/Filter/filters" )
include_directories( "src/Adder" )
include_directories( "src/Adder/adders" )

add_library(${PROJECT_NAME} STATIC
    ${SRC} ${HEADERS}
)
target_include_directories(${PROJECT_NAME} PUBLIC ${utils2_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    utils2
    Qt::Core
    Threads::Threads
    ${OpenCV_LIBS} opencv_bgsegm
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
MESSAGE(STATUS "PROJECT_NAME: ${PROJECT_NAME}")

MESSAGE(STATUS "CMAKE_INSTALL_LIBDIR: ${CMAKE_INSTALL_LIBDIR}")
MESSAGE(STATUS "CMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}")


install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)
install(EXPORT ${PROJECT_NAME}Config DESTINATION share/${PROJECT_NAME}/cmake)


export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)
add_executable(${PROJECT_NAME}_test ${SRC_HEADERS_TEST})
target_include_directories(${PROJECT_NAME}_test PUBLIC ${gtest_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} GTest::gtest)

#export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)

if (WIN32)
    get_target_property(QMAKE_EXE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    message(STATUS "QT_BIN_DIR:" ${QT_BIN_DIR})
    find_program(WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${QT_BIN_DIR}")
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
endif(WIN32)

if (WIN32)
    set(PREFIX_BUILD_TYPE "")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(PREFIX_BUILD_TYPE "d")
    endif()
    message(STATUS "OPENCV_VERSION_WINDOWS:" ${OPENCV_VERSION_WINDOWS})
    message(STATUS "CMAKE_CURRENT_BINARY_DIR:" ${CMAKE_CURRENT_BINARY_DIR})
    message(STATUS "PREFIX_BUILD_TYPE:" ${PREFIX_BUILD_TYPE})
    message(STATUS "CMAKE_BINARY_DIR:" ${CMAKE_BINARY_DIR})
    
    MESSAGE(STATUS "utils2_DIR ${utils2_DIR_CUSTOM}/lib/utils2.dll ")
    MESSAGE(STATUS "copy to:${CMAKE_BINARY_DIR}/${PREFIX_BUILD_TYPE}")

    
    set(CUSTOM_BIN_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    add_custom_command ( TARGET ${PROJECT_NAME}_test
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${utils2_DIR_CUSTOM}lib/utils2.dll                   ${CUSTOM_BIN_DIR}  

        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Test${PREFIX_BUILD_TYPE}.dll        ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Gui${PREFIX_BUILD_TYPE}.dll         ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Multimedia${PREFIX_BUILD_TYPE}.dll  ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Network${PREFIX_BUILD_TYPE}.dll     ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Core${PREFIX_BUILD_TYPE}.dll        ${CUSTOM_BIN_DIR}

        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll  ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_bgsegm${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll     ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_tracking${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_video${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll      ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgcodecs${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll  ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgproc${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_core${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll       ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_calib3d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_features2d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_flann${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll      ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_dnn${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll        ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_highgui${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll  ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_gapi${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll       ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio_ffmpeg${OPENCV_VERSION_WINDOWS}_64.dll              ${CUSTOM_BIN_DIR}
    
    )
endif(WIN32)