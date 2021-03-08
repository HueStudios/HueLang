add_executable(huelang interpreter.cpp interpreter.h logo.h main.cpp)

target_link_libraries(huelang lhuelang)
target_link_libraries(huelang readline)
target_link_libraries(huelang dl)