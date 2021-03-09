# target: target name to set xcode Swift Objective-C bridging header
# header_path: bridging header path
function(set_swift_bridging_header)
    set(prefix param)
    set(options)
    set(single_value_args target header_path)
    set(multi_value_args)

    cmake_parse_arguments(${prefix} "${options}" "${single_value_args}" "${multi_value_args}" ${ARGN})

    if(NOT DEFINED param_target OR NOT DEFINED param_header_path)
        return()
    endif()

    if(APPLE)
        set_target_properties(${param_target} PROPERTIES 
            XCODE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER ${param_header_path}
        )
    endif()
endfunction()