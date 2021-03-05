include(CMakeParseArguments)

# module_name: name of module
# module_sources: source files of module (cpp, h...)
# import_modules: module names to import
# third_party_libs: library to link
# third_party_include_paths: include paths
function(build_engine_module_function)
    set(prefix param)
    set(options)
    set(single_value_args module_name)
    set(multi_value_args module_sources import_modules third_party_libs third_party_include_paths)

    cmake_parse_arguments(${prefix} "${options}" "${single_value_args}" "${multi_value_args}" ${ARGN})

    if(NOT DEFINED param_module_name OR NOT DEFINED param_module_sources)
        message(WARNING "No module name or source!")
        return()
    endif()

    message(STATUS "Build ${param_module_name} module...")
    message(STATUS "Source files: ${param_module_sources}")

    if(BUILD_STATIC)
        add_library(${param_module_name} STATIC ${param_module_sources})
    else()
        add_library(${param_module_name} SHARED ${param_module_sources})
    endif()

    string(TOUPPER ${param_module_name} module_name_upper)

    set_target_properties(${param_module_name} PROPERTIES OUTPUT_NAME ${param_module_name})
    set_target_properties(${param_module_name} PROPERTIES PREFIX "")
    target_compile_definitions(${param_module_name} PRIVATE "EXPORT_${module_name_upper}")

    if(DEFINED param_import_modules)
        list(JOIN param_import_modules " " import_names)
        message(STATUS "Import modules: ${import_names}")

        foreach(import_name ${param_import_modules})
            string(TOUPPER ${import_name} import_name_upper)
            list(APPEND import_macros "IMPORT_${import_name_upper}")
        endforeach()

        add_dependencies(${param_module_name} ${param_import_modules})
        target_compile_definitions(${param_module_name} PUBLIC ${import_macros})
        target_link_libraries(${param_module_name} PUBLIC ${param_import_modules})
    endif()

    if(DEFINED param_third_party_libs)
        target_link_libraries(${param_module_name} PRIVATE ${param_third_party_libs})
        message(STATUS "Third party libraries: ${param_third_party_libs}")
    endif()

    if(DEFINED param_third_party_include_paths)
        target_include_directories(${param_module_name} PRIVATE ${param_third_party_include_paths})
        message(STATUS "Third party include paths: ${param_third_party_include_paths}")
    endif()

    message(STATUS "==============================")
endfunction()

# append_target: destination list to append file paths
# search_paths: paths to search files
# if: conditions to check before search files (if condition is false, will add nothing)
# recursive_mode: whether to find files in using recursive
function(search_and_append_files append_target)
    set(prefix param)
    set(options recursive_mode)
    set(single_value_args)
    set(multi_value_args search_paths if)

    cmake_parse_arguments(${prefix} "${options}" "${single_value_args}" "${multi_value_args}" ${ARGN})

    if(NOT DEFINED param_search_paths)
        message("No path!")
        return()
    endif()

    if(DEFINED param_if)
        foreach(condition ${param_if})
            if(NOT ${condition})
                return()
            endif()
        endforeach()
    endif()

    if(NOT DEFINED ${append_target})
        set(${append_target} "" PARENT_SCOPE)
    endif()

    set(result ${${append_target}})

    foreach(search_path ${param_search_paths})
        if(${param_recursive_mode})
            file(GLOB_RECURSE files ${search_path})
        else()
            file(GLOB files ${search_path})
        endif()

        list(APPEND result ${files})
    endforeach()

    set(${append_target} ${result} PARENT_SCOPE)
endfunction()