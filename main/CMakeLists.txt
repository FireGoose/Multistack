file(GLOB headers "*.h")
file(GLOB sources "*.cpp")

add_executable(${application} ${sources} ${headers})
target_link_libraries(${application} ${library})
