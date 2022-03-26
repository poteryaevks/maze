set(TARGET_NAME maze)

set(HEADER_FILES
    mazegenerator.h
    mazegraphicsscene.h
    mazegraphicsview.h
    mazeitem.h
    mainwindow.h
    )
    
set(SRC_FILES
    main.cpp
    mainwindow.cpp
    mazegenerator.cpp
    mazegraphicsscene.cpp
    mazegraphicsview.cpp
    mazeitem.cpp
    )

qt5_wrap_ui(SRC_FILES mainwindow.ui)

add_executable(${TARGET_NAME} 
    ${HEADER_FILES}
    ${SRC_FILES}
    )

set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    )

target_compile_options(${TARGET_NAME} PRIVATE
    "$<$<CONFIG:Debug>:-Wall;-D_DEBUG>"
    "$<$<CONFIG:Release>:-O2;-DNDEBUG>"
    )

message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    )

message("CMAKE_THREAD_LIBS_INIT: ${CMAKE_THREAD_LIBS_INIT}")

target_link_libraries(${TARGET_NAME} PRIVATE
    Qt5::Core
    Qt5::Widgets
    ${CMAKE_THREAD_LIBS_INIT}
    )
