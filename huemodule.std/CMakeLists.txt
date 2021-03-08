add_library(huemodule.std SHARED std.cpp std.h typeddefinition.cpp
            typeddefinition.h)

target_link_libraries(huemodule.std lhuelang)
set_target_properties(huemodule.std PROPERTIES PREFIX "")