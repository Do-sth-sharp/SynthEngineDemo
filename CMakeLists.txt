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
target_link_libraries (SynthEngineDemo PRIVATE juce)
set_target_properties (SynthEngineDemo PROPERTIES
  PREFIX "" OUTPUT_NAME "SynthEngineDemo" SUFFIX ".vst3")

if (NOT MSVC)
    target_compile_options (SynthEngineDemo PRIVATE
			-pthread
			-fPIC
			-fvisibility=hidden
			-fvisibility-inlines-hidden
    )
	target_link_options (SynthEngineDemo PRIVATE
			-shared
			-fvisibility=hidden
			-fvisibility-inlines-hidden
	)

    if (WIN32)
        target_link_libraries (SynthEngineDemo PRIVATE
            -m64
            -s
			-lcomctl32
			-lwinmm
			-lwsock32
			-lwininet
			-lversion
			-lws2_32
			-loleaut32
			-limm32
			-lshlwapi
			-lrpcrt4
			-lopengl32
			-ldxgi
        )
    elseif (UNIX AND NOT APPLE)
        target_link_libraries (SynthEngineDemo PRIVATE
            ${RT}
            ${DL}
            ${PTHREAD}
            ${GL}
            ${ALSA_LIBRARIES}
            ${FREETYPE2_LIBRARIES}
            ${LIBCURL_LIBRARIES}
        )
    elseif (APPLE)
        target_link_libraries (SynthEngineDemo PRIVATE
            "-framework Accelerate"
            "-framework AudioToolbox"
            "-framework AVFoundation"
            "-framework AVKit"
            "-framework Carbon"
            "-framework Cocoa"
            "-framework CoreAudio"
            "-framework CoreAudioKit"
            "-framework CoreMedia"
            "-framework CoreMIDI"
            "-framework DiscRecording"
            "-framework Foundation"
            "-framework IOKit"
            "-framework OpenGL"
            "-framework QuartzCore"
            "-framework WebKit"
        )
    endif ()
else (NOT MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        target_compile_options (SynthEngineDemo PRIVATE
            /permissive-#符合模式:是
	        /GS#安全检查:启用安全检查
	        /Zc:wchar_t#将WChar_t视为内置类型:是
	        /Gm-#启用最小重新生成:否
	        /sdl#SDL检查:是
	        /fp:precise#浮点模型:精度
	        /Zc:forScope#强制For循环范围中的合规性:是
	        /Gd#调用约定:__cdecl
	        /nologo#取消显示启动版权标志:是
	        /bigobj#较大生成对象:是
        )
        target_link_options (SynthEngineDemo PRIVATE
	        /MANIFEST#生成清单:是
	        /NXCOMPAT#数据执行保护(DEP):是
	        /DYNAMICBASE#随机基址:是
	        /LARGEADDRESSAWARE#启用大地址:是
	        /SUBSYSTEM:WINDOWS#子系统:窗口
	        /MANIFESTUAC:NO#启用用户账户控制:否
	        /NOLOGO#取消显示启动版权标志:是
        )
    else (CMAKE_BUILD_TYPE STREQUAL Debug)
        target_compile_options (SynthEngineDemo PRIVATE
            /permissive-#符合模式:是
	        /GS#安全检查:启用安全检查
	        /GL#全程序优化:是
	        /Gy#启用函数级链接:是
	        /Zc:wchar_t#将WChar_t视为内置类型:是
	        /Gm-#启用最小重新生成:否
	        /sdl#SDL检查:是
	        /fp:precise#浮点模型:精度
	        /Zc:forScope#强制For循环范围中的合规性:是
	        /Gd#调用约定:__cdecl
	        /Oi#启用内部函数:是
	        /nologo#取消显示启动版权标志:是
	        /bigobj#较大生成对象:是
        )
        target_link_options (SynthEngineDemo PRIVATE
	        /MANIFEST#生成清单:是
	        /LTCG:incremental#链接时间代码生成:使用快速链接时间代码生成
	        /NXCOMPAT#数据执行保护(DEP):是
	        /DYNAMICBASE#随机基址:是
	        /LARGEADDRESSAWARE#启用大地址:是
	        /OPT:REF#引用:是
	        /SUBSYSTEM:WINDOWS#子系统:窗口
	        /MANIFESTUAC:NO#启用用户账户控制:否
	        /OPT:ICF#启用COMDAT折叠:是
	        /NOLOGO#取消显示启动版权标志:是
        )
    endif (CMAKE_BUILD_TYPE STREQUAL Debug)
endif (NOT MSVC)

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