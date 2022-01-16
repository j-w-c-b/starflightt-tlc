function(find_module_resources)
    # call with find_module_resources(encounter)
    # will generate EncounterResources.h which depends on data/encounter/*
    string(TOUPPER "${ARGV0}" RESOURCE_NAME)
    string(TOUPPER "${RESOURCE_NAME}_DEPENDENCIES" resourcedepname)
    set(RESOURCE_HEADER "${ARGV0}_resources.h")
    list(APPEND hdr "//This file is automatically generated. DO NOT EDIT\n")
    list(APPEND hdr "#pragma once\n")
    list(APPEND hdr "#include \"ResourceManager.h\"")

    file(GLOB ${resourcedepname} "../data/${ARGV0}/*")
    list(SORT ${resourcedepname})
    foreach (fn ${${resourcedepname}})
        string(REGEX MATCH "^.*/(([^/.]*)\\.([^/.]*))\$" filename ${fn})
        set(filename ${CMAKE_MATCH_1})
        set(basename ${CMAKE_MATCH_2})
        set(extension ${CMAKE_MATCH_3})
        string(TOUPPER "${basename}" ucname)
        string(REGEX REPLACE "[^A-Z0-9_]" "_" key ${ucname})
        if (${extension} MATCHES "[Bb][Mm][Pp]|[Tt][Gg][Aa]")
            list(APPEND hdr "static const char *${key} = \"${key}\"\\;")
            list(APPEND imgs "\t{\"${key}\", \"data/${ARGV0}/${filename}\"},")
        endif()
    endforeach()
    list(APPEND hdr "\nstatic const ResourceName ${RESOURCE_NAME}_IMAGES[] = {")
    list(APPEND hdr ${imgs})
    list(APPEND hdr "\t{\"\", \"\"}\n}\\;")
    string(JOIN "\n" HDR ${hdr})
    file(WRITE "${OUTPUT_DIR}/${RESOURCE_HEADER}" "${HDR}")
endfunction()

find_module_resources(${RESOURCE_NAME} ${OUTPUT_DIR})