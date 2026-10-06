cmake_minimum_required(VERSION 3.28...3.30)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(${_name} VERSION ${_version})

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" ON)
option(ENABLE_QT "Use Qt functionality" ON)

include(compilerconfig)
include(defaults)
include(helpers)

add_library(${CMAKE_PROJECT_NAME} MODULE)

find_package(libobs REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

if(ENABLE_FRONTEND_API)
  find_package(obs-frontend-api REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)
endif()

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        src/plugin-main.cpp
        src/ScoreboardDock.cpp
        src/ScoreboardDock.hpp
        src/ScoreboardDockHor.ui
        src/ScoreboardDockVert.ui
        src/OBSTextWriter.hpp
        src/OBSTextWriter.cpp
)

if(ENABLE_QT)
  find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

  target_link_libraries(${CMAKE_PROJECT_NAME}
      PRIVATE
          Qt6::Core
          Qt6::Widgets
  )

  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header -Wno-comma>
  )

  set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
  )
endif()

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
    "E:/Program Files/obs-studio/obs-plugins/64bit/"
)
set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})
