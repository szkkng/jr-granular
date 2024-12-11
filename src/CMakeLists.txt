add_subdirectory(ui)

target_sources(
    jr-granular
    PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp
)
