cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("initializer")

set(Boost_NO_SYSTEM_PATHS TRUE)
set(Boost_USE_STATIC_LIBS TRUE)

find_package(Boost REQUIRED COMPONENTS regex)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})

  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

  add_executable(${PROJECT_NAME} 
    "src/main.cpp"
  )

  target_link_libraries(${PROJECT_NAME} 
    ${Boost_LIBRARIES}
  )
endif()
