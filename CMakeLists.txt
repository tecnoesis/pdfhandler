cmake_minimum_required(VERSION 3.16)

project(pdfhandler VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEV_ROOT_DIR "D:/development")
set(POPPLER_DIR ${DEV_ROOT_DIR}/poppler)

# Add the executable
add_executable(pdfhandler 
                main.cpp
                Paragraph.cpp
            )

target_include_directories(pdfhandler 
                                PRIVATE ${POPPLER_DIR}/cpp
                                PRIVATE ${POPPLER_DIR}/build/cpp
                            )

if (CMAKE_BUILD_TYPE STREQUAL "Debug") # CMAKE_BUILD_TYPE is empty by default. To link against Debug Qt explicitly set: -DCMAKE_BUILD_TYPE Debug
    set(POPPLER_BIN_DIR ${POPPLER_DIR}/build/Debug)
    set(POPPLER_CPP_BIN_DIR ${POPPLER_DIR}/build/cpp/Debug)
else()
    set(POPPLER_BIN_DIR ${POPPLER_DIR}/build/Release)
    set(POPPLER_CPP_BIN_DIR ${POPPLER_DIR}/build/cpp/Release)
endif()

# Set path to libraries to be linked (poppler-cpp.lib and poppler.lib):
target_link_directories(pdfhandler 
                            PRIVATE ${POPPLER_BIN_DIR}
                            PRIVATE ${POPPLER_CPP_BIN_DIR}
                        )

# Links with poppler-cpp.lib and poppler.lib
target_link_libraries(pdfhandler
                        PRIVATE poppler
                        PRIVATE poppler-cpp
                    )

# Tecnoesis project root directory
set(SRC_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC) 
    # For '/Zc:__cplusplus' see also: https://stackoverflow.com/questions/57102212/cannot-set-cplusplus-to-c17-standard-with-visual-studio-and-cmake
    # For '/permissive-' see also: https://forum.qt.io/topic/131374/6-2-1-msvc-static-missing-permissive-compiler-option/4
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /Zc:__cplusplus /permissive-")
    
    set(CMAKE_LINKER_TYPE MSVC)

    if(DEFINED CMAKE_DISTRIB) # 'CMAKE_DISTRIB' could be used immediately but silly warning is emitted. See also: https://stackoverflow.com/questions/36451368/get-rid-of-cmake-warning-manually-specified-variables-were-not-used-by-the-proj
        set(DISTRIB_SETUP "${CMAKE_DISTRIB}")
        message("Setup for Distribution!")
    else()
        set(DISTRIB_SETUP False)
    endif()
endif()


if(MSVC)

    # Create a .ico file from an image (https://www.wikihow.com/Create-a-Windows-Icon)
    # Create a .rc resource file from Visual Studio (Project -> Add New Item -> Resource -> Resource File), add the .ico file to it and save it.
    # Add the .rc file to the target executable:
    set(CMAKE_VS_SDK_INCLUDE_DIRECTORIES ${CUSTOM_INCLUDE_DIRECTORIES})

    if(DISTRIB_SETUP)
    
    else()
        target_link_options(pdfhandler PRIVATE /MANIFEST /SUBSYSTEM:CONSOLE)
    endif()
    # '/SUBSYSTEM:CONSOLE' adds a console to the otherwise GUI application so that std::cerr, cout qDebug() etc print there!
    # See also: https://stackoverflow.com/questions/192294/how-to-write-to-the-console-in-a-gui-application

    set(VS_ENV_PATH_DEBUG "PATH=%PATH%" ${POPPLER_BIN_DIR} ${POPPLER_CPP_BIN_DIR})
    set(VS_ENV_PATH_RELEASE "PATH=%PATH%" ${POPPLER_BIN_DIR} ${POPPLER_CPP_BIN_DIR})
    set(VS_ENV_PATH_RELWITHDEBINFO "PATH=%PATH%" ${POPPLER_BIN_DIR} ${POPPLER_CPP_BIN_DIR})

    set_target_properties(pdfhandler PROPERTIES VS_DEBUGGER_ENVIRONMENT "$<$<CONFIG:Debug>:${VS_ENV_PATH_DEBUG}>$<$<CONFIG:Release>:${VS_ENV_PATH_RELEASE}>$<$<CONFIG:RelWithDebInfo>:${VS_ENV_PATH_RELWITHDEBINFO}>") # See also: https://stackoverflow.com/questions/1005901/how-to-set-path-environment-variable-using-cmake-and-visual-studio-to-run-test
    
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
