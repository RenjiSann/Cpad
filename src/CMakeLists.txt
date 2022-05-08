
set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/utils.cc
    ${CMAKE_SOURCE_DIR}/src/scaner.cc
    ${CMAKE_SOURCE_DIR}/src/executor.cc
    ${CMAKE_SOURCE_DIR}/src/error-handling.cc
    ${CMAKE_SOURCE_DIR}/src/display.cc
    ${CMAKE_SOURCE_DIR}/src/convertor.cc
    ${CMAKE_SOURCE_DIR}/src/argument-type.cc
    ${CMAKE_SOURCE_DIR}/src/messages-manager.cc

    ${CMAKE_SOURCE_DIR}/src/entities/config.cc
    ${CMAKE_SOURCE_DIR}/src/entities/element.cc
    ${CMAKE_SOURCE_DIR}/src/entities/folder.cc
    ${CMAKE_SOURCE_DIR}/src/entities/command.cc
    ${CMAKE_SOURCE_DIR}/src/entities/combo_command.cc

    ${CMAKE_SOURCE_DIR}/src/cpad_system/cpad.cc
PARENT_SCOPE)

set(EXECUTABLE_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/main/main.cc
PARENT_SCOPE)
