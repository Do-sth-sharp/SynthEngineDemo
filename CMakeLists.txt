cmake_minimum_required (VERSION 3.15)
project (SynthEngineDemo VERSION 1.0.0 LANGUAGES CXX C)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif (NOT DEFINED CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 20)#使用C++20
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_STANDARD 17)#使用C17
set (CMAKE_C_EXTENSIONS OFF)
set (CMAKE_C_STANDARD_REQUIRED ON)

set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")#MSVC下使用/MD和/MDd

# =============================================
# JUCE
# =============================================
set (JucePlugin_Name "SynthEngineDemo")
set (JucePlugin_Desc "VocalShaper Synth Engine Demo")
set (JucePlugin_Manufacturer "VocalSharp Org.")
set (JucePlugin_ManufacturerWebsite "https://daw.org.cn")
set (JucePlugin_ManufacturerEmail "3142324836@qq.com")
set (JucePlugin_Version "1.0.0")
set (JucePlugin_VersionCode "0x010000")
set (JucePlugin_ManufacturerCode "0x56736474")
set (JucePlugin_PluginCode "0x53656764")
set (JucePlugin_IsSynth ON)
set (JucePlugin_IsMidiEffect OFF)
set (JucePlugin_WantsMidiInput ON)
set (JucePlugin_ProducesMidiOutput OFF)
set (JucePlugin_EditorRequiresKeyboardFocus ON)
set (JucePlugin_Vst3Category "Instrument|Generator|Synth")
set (JucePlugin_CFBundleIdentifier "org.VocalSharp.VocalShaper.SynthEngineDemo")
set (JucePlugin_VSTNumMidiInputs "16")
set (JucePlugin_VSTNumMidiOutputs "0")
set (JucePlugin_MaxNumInputChannels "2")
set (JucePlugin_MaxNumOutputChannels "2")
set (JucePlugin_PreferredChannelConfigurations "{0, 1}, {0, 2}, {1, 1}, {1, 2}, {2, 1}, {2, 2}")
set (JucePlugin_Enable_ARA ON)
set (JucePlugin_ARACompatibleArchiveIDs "org.VocalSharp.VocalShaper.SynthEngineDemo")
set (JucePlugin_ARAContentTypes "0x01")
set (JucePlugin_ARATransformationFlags "0x00")
set (JucePlugin_ARAFactoryID "org.VocalSharp.VocalShaper.SynthEngineDemo")
set (JucePlugin_ARADocumentArchiveID "org.VocalSharp.VocalShaper.SynthEngineDemo")

add_subdirectory (juce-vst3-dev-kit)

# =============================================
# Target
# =============================================
file (GLOB_RECURSE TARGET_SRC "src/*.cpp" "src/*.h")
add_library (SynthEngineDemo SHARED ${TARGET_SRC})
if (NOT WIN32)
	target_link_libraries (SynthEngineDemo PRIVATE -Wl,--whole-archive juce -Wl,--no-whole-archive)
else ()
	target_link_libraries (SynthEngineDemo PRIVATE juce)
endif ()
set_target_properties (SynthEngineDemo PROPERTIES
  PREFIX "" OUTPUT_NAME "SynthEngineDemo" SUFFIX ".vst3")

# =============================================
# Install
# =============================================
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set (ENGINE_INSTALL_NAME ${PROJECT_NAME})

set (ENGINE_CMAKE_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${ENGINE_INSTALL_NAME})

install (TARGETS SynthEngineDemo
	EXPORT ${ENGINE_INSTALL_NAME}Targets
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" OPTIONAL
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" OPTIONAL
)

if (MSVC)
	install (FILES $<TARGET_PDB_FILE:SynthEngineDemo> DESTINATION "${CMAKE_INSTALL_BINDIR}" OPTIONAL)
endif (MSVC)

write_basic_package_version_file (
	"${CMAKE_CURRENT_BINARY_DIR}/${ENGINE_INSTALL_NAME}ConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file (
	"${CMAKE_CURRENT_LIST_DIR}/${ENGINE_INSTALL_NAME}Config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${ENGINE_INSTALL_NAME}Config.cmake"
	INSTALL_DESTINATION ${ENGINE_CMAKE_CONFIG_INSTALL_DIR}
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install (FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${ENGINE_INSTALL_NAME}Config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${ENGINE_INSTALL_NAME}ConfigVersion.cmake"
	DESTINATION ${ENGINE_CMAKE_CONFIG_INSTALL_DIR}
)

install (EXPORT ${ENGINE_INSTALL_NAME}Targets
	FILE "${ENGINE_INSTALL_NAME}Targets.cmake"
	NAMESPACE ${ENGINE_INSTALL_NAME}::
	DESTINATION ${ENGINE_CMAKE_CONFIG_INSTALL_DIR}
)