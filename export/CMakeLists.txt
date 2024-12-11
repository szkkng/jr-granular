target_include_directories(
    jr-granular
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/rnbo"
        "${CMAKE_CURRENT_SOURCE_DIR}/rnbo/common"
)

target_sources(
    jr-granular
    PRIVATE
        rnbo_granular.cpp
        rnbo/RNBO.cpp
)
