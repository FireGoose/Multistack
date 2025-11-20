file(GLOB headers "*.h")
file(GLOB sources "*.cpp")

add_library(${library} STATIC ${sources} ${headers})