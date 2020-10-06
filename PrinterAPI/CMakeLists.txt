cmake_minimum_required (VERSION 3.12)


include(${CMAKE_SOURCE_DIR}/API/api.cmake)
api_target(PrinterAPI "${CMAKE_SOURCE_DIR}/VarAPI/include")


target_link_libraries(${RELEASE_TARGET} PUBLIC API VarAPI)
