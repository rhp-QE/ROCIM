include_directories(${CMAKE_CURRENT_SOURCE_DIR}/implement)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/interface)

file(GLOB SOURCESDK "implement/**/*.cpp")
file(GLOB SOURCESDK1 "implement/**/*.cc")

add_library(sdk ${SOURCESDK} ${SOURCESDK1})