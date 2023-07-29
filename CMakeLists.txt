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

set (JUCE_DISPLAY_SPLASH_SCREEN OFF)
set (JUCE_USE_DARK_SPLASH_SCREEN ON)
set (JucePlugin_Build_VST OFF)
set (JucePlugin_Build_VST3 ON)
set (JucePlugin_Build_AU OFF)
set (JucePlugin_Build_AUv3 OFF)
set (JucePlugin_Build_LV2 OFF)
set (JucePlugin_Build_RTAS OFF)
set (JucePlugin_Build_AAX OFF)
set (JucePlugin_Build_Standalone OFF)
set (JucePlugin_Build_Unity OFF)
set (JucePlugin_Enable_ARA OFF)
set (JUCE_USE_WINRT_MIDI OFF)
set (JUCE_ASIO OFF)
set (JUCE_JACK OFF)
set (JUCE_BELA OFF)
set (JUCE_USE_ANDROID_OPENSLES OFF)
set (JUCE_ASIO_DEBUGGING OFF)
set (JUCE_WINRT_MIDI_LOGGING OFF)
set (JUCE_CUSTOM_VST3_SDK ON)
set (JUCE_VST3_CAN_REPLACE_VST2 ON)
set (JUCE_USE_MP3AUDIOFORMAT OFF)
set (JUCE_USE_LAME_AUDIO_FORMAT OFF)

set (VST3_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vst3sdk")
set (VST_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vst2-sdk-for-vst3sdk")
set (DMDA_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libDMDA")

set (JUCE_DEBUG_DEF "DEBUG=1" "_DEBUG=1")
set (JUCE_NDEBUG_DEF "NDEBUG=1")
set (JUCE_LINUX_DEF "LINUX=1")
set (JUCE_WIN_DEF "")
set (JUCE_OSX_DEF "")

set (JUCE_MODULE_DEF 
    "JUCE_DISPLAY_SPLASH_SCREEN=$<IF:$<BOOL:${JUCE_DISPLAY_SPLASH_SCREEN}>,1,0>"
    "JUCE_USE_DARK_SPLASH_SCREEN=$<IF:$<BOOL:${JUCE_USE_DARK_SPLASH_SCREEN}>,1,0>"
    "JUCE_MODULE_AVAILABLE_juce_analytics=1"
    "JUCE_MODULE_AVAILABLE_juce_audio_basics=1"
    "JUCE_MODULE_AVAILABLE_juce_audio_devices=1"
    "JUCE_MODULE_AVAILABLE_juce_audio_formats=1"
    "JUCE_MODULE_AVAILABLE_juce_audio_plugin_client=1"
    "JUCE_MODULE_AVAILABLE_juce_audio_processors=1"
    "JUCE_MODULE_AVAILABLE_juce_audio_utils=1"
    "JUCE_MODULE_AVAILABLE_juce_box2d=1"
    "JUCE_MODULE_AVAILABLE_juce_core=1"
    "JUCE_MODULE_AVAILABLE_juce_cryptography=1"
    "JUCE_MODULE_AVAILABLE_juce_data_structures=1"
    "JUCE_MODULE_AVAILABLE_juce_dsp=1"
    "JUCE_MODULE_AVAILABLE_juce_events=1"
    "JUCE_MODULE_AVAILABLE_juce_graphics=1"
    "JUCE_MODULE_AVAILABLE_juce_gui_basics=1"
    "JUCE_MODULE_AVAILABLE_juce_gui_extra=1"
    "JUCE_MODULE_AVAILABLE_juce_opengl=1"
    "JUCE_MODULE_AVAILABLE_juce_osc=1"
    "JUCE_MODULE_AVAILABLE_juce_product_unlocking=1"
    "JUCE_MODULE_AVAILABLE_juce_video=1"
    "JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1"
    "JUCE_STRICT_REFCOUNTEDPOINTER=1"
    "JUCE_STANDALONE_APPLICATION=0"
    "JucePlugin_Build_VST=$<IF:$<BOOL:${JucePlugin_Build_VST}>,1,0>"
    "JucePlugin_Build_VST3=$<IF:$<BOOL:${JucePlugin_Build_VST3}>,1,0>"
    "JucePlugin_Build_AU=$<IF:$<BOOL:${JucePlugin_Build_AU}>,1,0>"
    "JucePlugin_Build_AUv3=$<IF:$<BOOL:${JucePlugin_Build_AUv3}>,1,0>"
    "JucePlugin_Build_LV2=$<IF:$<BOOL:${JucePlugin_Build_LV2}>,1,0>"
    "JucePlugin_Build_RTAS=$<IF:$<BOOL:${JucePlugin_Build_RTAS}>,1,0>"
    "JucePlugin_Build_AAX=$<IF:$<BOOL:${JucePlugin_Build_AAX}>,1,0>"
    "JucePlugin_Build_Standalone=$<IF:$<BOOL:${JucePlugin_Build_Standalone}>,1,0>"
    "JucePlugin_Build_Unity=$<IF:$<BOOL:${JucePlugin_Build_Unity}>,1,0>"
    "JUCE_USE_WINRT_MIDI=$<IF:$<BOOL:${JUCE_USE_WINRT_MIDI}>,1,0>"
    "JUCE_ASIO=$<IF:$<BOOL:${JUCE_ASIO}>,1,0>"
    "JUCE_JACK=$<IF:$<BOOL:${JUCE_JACK}>,1,0>"
    "JUCE_BELA=$<IF:$<BOOL:${JUCE_BELA}>,1,0>"
    "JUCE_USE_ANDROID_OPENSLES=$<IF:$<BOOL:${JUCE_USE_ANDROID_OPENSLES}>,1,0>"
    "JUCE_ASIO_DEBUGGING=$<IF:$<BOOL:${JUCE_ASIO_DEBUGGING}>,1,0>"
    "JUCE_WINRT_MIDI_LOGGING=$<IF:$<BOOL:${JUCE_WINRT_MIDI_LOGGING}>,1,0>"
    "JUCE_CUSTOM_VST3_SDK=$<IF:$<BOOL:${JUCE_CUSTOM_VST3_SDK}>,1,0>"
    "JUCE_VST3_CAN_REPLACE_VST2=$<IF:$<BOOL:${JUCE_VST3_CAN_REPLACE_VST2}>,1,0>"
    "JUCE_USE_MP3AUDIOFORMAT=$<IF:$<BOOL:${JUCE_USE_MP3AUDIOFORMAT}>,1,0>"
    "JUCE_USE_LAME_AUDIO_FORMAT=$<IF:$<BOOL:${JUCE_USE_LAME_AUDIO_FORMAT}>,1,0>"
    "JucePlugin_Enable_ARA=$<IF:$<BOOL:${JucePlugin_Enable_ARA}>,1,0>"
)

#=============================================================================================================================
#JUCE
#=============================================================================================================================
set (JUCE_INC "JuceLibraryCode" "JUCE/modules")
if (UNIX AND NOT APPLE)
    find_package (PkgConfig REQUIRED)
    pkg_search_module (ALSA REQUIRED alsa)
    pkg_search_module (FREETYPE2 REQUIRED freetype2)
    pkg_search_module (LIBCURL REQUIRED libcurl)
    pkg_search_module (WEBKIT2GTK-4.0 REQUIRED webkit2gtk-4.0)
    pkg_search_module (GTK+-X11-3.0 REQUIRED gtk+-x11-3.0)

    set (JUCE_INC ${JUCE_INC}
        ${ALSA_INCLUDE_DIRS}
        ${FREETYPE2_INCLUDE_DIRS}
        ${LIBCURL_INCLUDE_DIRS}
        ${WEBKIT2GTK-4.0_INCLUDE_DIRS}
        ${GTK+-X11-3.0_INCLUDE_DIRS}
    )

    find_library (RT rt)
    find_library (DL dl)
    find_library (PTHREAD pthread)
    find_library (GL GL)
endif (UNIX AND NOT APPLE)

if (JUCE_ASIO)
    if (DEFINED ASIO_SRC_PATH)
        set (JUCE_ASIO_INC_PATH ${ASIO_SRC_PATH})
    else (DEFINED ASIO_SRC_PATH)
        message (SEND_ERROR "ASIO SDK notfound!")
    endif (DEFINED ASIO_SRC_PATH)
endif (JUCE_ASIO)
if (JucePlugin_Build_VST OR ((JUCE_PLUGINHOST_VST3 OR JucePlugin_Build_VST3) AND JUCE_VST3_CAN_REPLACE_VST2))
    if (DEFINED VST_SRC_PATH)
        set (JUCE_VST_INC_PATH ${VST_SRC_PATH})
    else (DEFINED VST_SRC_PATH)
        message (SEND_ERROR "VST SDK notfound!")
    endif (DEFINED VST_SRC_PATH)
endif (JucePlugin_Build_VST OR ((JUCE_PLUGINHOST_VST3 OR JucePlugin_Build_VST3) AND JUCE_VST3_CAN_REPLACE_VST2))
if (JucePlugin_Build_VST3 AND JUCE_CUSTOM_VST3_SDK)
    if (DEFINED VST3_SRC_PATH)
        set (JUCE_VST3_INC_PATH ${VST3_SRC_PATH})
    else (DEFINED VST3_SRC_PATH)
        message (SEND_ERROR "VST3 SDK notfound!")
    endif (DEFINED VST3_SRC_PATH)
endif (JucePlugin_Build_VST3 AND JUCE_CUSTOM_VST3_SDK)
if (JucePlugin_Build_LV2)
    if (DEFINED LV2_SRC_PATH)
        set (JUCE_LV2_INC_PATH ${LV2_SRC_PATH})
    else (DEFINED LV2_SRC_PATH)
        #LV2 SDK Path
        set (JUCE_LV2_INC_PATH
            "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules/juce_audio_processors/format_types/LV2_SDK"
            "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2"
            "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv"
            "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src"
            "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd"
            "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord"
            "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sratom"
        )
    endif (DEFINED LV2_SRC_PATH)

    #LV2 SDK Src
    set (JUCE_LV2_SRC_FIX
        "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules/juce_audio_processors/juce_audio_processors_lv2_libs.cpp"
    )
endif (JucePlugin_Build_LV2)
if (JucePlugin_Enable_ARA)
    if (DEFINED ARA_SRC_PATH)
        set (JUCE_ARA_INC_PATH ${ARA_SRC_PATH})
    else (DEFINED ARA_SRC_PATH)
        message (SEND_ERROR "ARA SDK notfound!")
    endif (DEFINED ARA_SRC_PATH)

    #ARA SDK Src
    if (JucePlugin_Enable_ARA)
    set (JUCE_ARA_SRC_FIX
        "${ARA_SRC_PATH}/ARA_Library/PlugIn/ARAPlug.cpp"
        "${ARA_SRC_PATH}/ARA_Library/Debug/ARADebug.c"
        "${ARA_SRC_PATH}/ARA_Library/Dispatch/ARAPlugInDispatch.cpp"
        "${ARA_SRC_PATH}/ARA_Library/Utilities/ARAChannelFormat.cpp"
    )
    endif (JucePlugin_Enable_ARA)
endif (JucePlugin_Enable_ARA)

set (DMDA_INC "${DMDA_SRC_PATH}")
set (DMDA_SRC "${DMDA_SRC_PATH}/DMDA.cpp")
set (DMDA_DEF "DMDA_PLUGIN=1")

#=============================================================================================================================
# Target
#=============================================================================================================================

file (GLOB_RECURSE JUCE_SRC "JuceLibraryCode/*.cpp" "JuceLibraryCode/*.h")
file (GLOB_RECURSE TARGET_SRC "src/*.cpp" "src/*.h")
add_library (SynthEngineDemo SHARED ${JUCE_SRC} ${TARGET_SRC} ${DMDA_SRC} ${JUCE_LV2_SRC_FIX} ${JUCE_ARA_SRC_FIX})
set_target_properties (SynthEngineDemo PROPERTIES
  PREFIX "" OUTPUT_NAME "SynthEngineDemo" SUFFIX ".vst3")
set_source_files_properties ("JuceLibraryCode/JuceHeader.h" PROPERTIES HEADER_FILE_ONLY TRUE)
target_include_directories (SynthEngineDemo PUBLIC
    ${JUCE_INC}
    ${JUCE_ASIO_INC_PATH}
    ${JUCE_VST_INC_PATH}
    ${JUCE_VST3_INC_PATH}
    ${JUCE_LV2_INC_PATH}
    ${JUCE_ARA_INC_PATH}
    ${DMDA_INC}
)

target_compile_definitions (SynthEngineDemo PRIVATE
"JucePlugin_Name=\"SynthEngineDemo\""
"JucePlugin_Desc=\"VocalShaper Synth Engine Demo\""
"JucePlugin_Manufacturer=\"VocalSharp Org.\""
"JucePlugin_ManufacturerWebsite=\"https://daw.org.cn\""
"JucePlugin_ManufacturerEmail=\"3142324836@qq.com\""
"JucePlugin_Version=1.0.0"
"JucePlugin_VersionCode=0x10000"
"JucePlugin_VersionString=\"1.0.0\""
"JucePlugin_ManufacturerCode=0x56736474"
"JucePlugin_PluginCode=0x53656764"
"JucePlugin_IsSynth=1"
"JucePlugin_IsMidiEffect=0"
"JucePlugin_WantsMidiInput=1"
"JucePlugin_ProducesMidiOutput=0"
"JucePlugin_EditorRequiresKeyboardFocus=0"
"JucePlugin_VSTUniqueID=JucePlugin_PluginCode"
"JucePlugin_Vst3Category=\"Instrument|Generator|Synth\""
"JucePlugin_CFBundleIdentifier=org.VocalSharp.VocalShaper.SynthEngineDemo"
"JucePlugin_VSTNumMidiInputs=16"
"JucePlugin_VSTNumMidiOutputs=16"
"JucePlugin_MaxNumInputChannels=2"
"JucePlugin_MaxNumOutputChannels=2"
"JucePlugin_PreferredChannelConfigurations={1, 1}, {2, 2}, {1, 2}"
)

if (DEFINED SYNTH_ENGINE_DEMO_OUTPUT_PATH)
    set_target_properties (SynthEngineDemo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SYNTH_ENGINE_DEMO_OUTPUT_PATH})
endif (DEFINED SYNTH_ENGINE_DEMO_OUTPUT_PATH)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions (SynthEngineDemo PUBLIC ${JUCE_DEBUG_DEF})
else (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions (SynthEngineDemo PUBLIC ${JUCE_NDEBUG_DEF})
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

if (WIN32)
    target_compile_definitions (SynthEngineDemo PUBLIC ${JUCE_WIN_DEF})
elseif (UNIX AND NOT APPLE)
    target_compile_definitions (SynthEngineDemo PUBLIC ${JUCE_LINUX_DEF})
elseif (APPLE)
    target_compile_definitions (SynthEngineDemo PUBLIC ${JUCE_OSX_DEF})
endif ()

target_compile_definitions (SynthEngineDemo PUBLIC ${JUCE_MODULE_DEF})
target_compile_definitions (SynthEngineDemo PUBLIC ${DMDA_DEF})

target_compile_definitions (SynthEngineDemo PUBLIC "JUCE_MODAL_LOOPS_PERMITTED=1" "JUCE_CATCH_UNHANDLED_EXCEPTIONS=1")

if (NOT MSVC)
    target_compile_options (SynthEngineDemo PRIVATE
    -fPIC
    -fvisibility=default
    )

    if (WIN32)
        target_link_libraries (SynthEngineDemo PRIVATE
            -m64
            -s
            -lgdi32
            -luser32
            -lkernel32
            -lcomctl32
            -lwinmm
            -luuid
            -lwsock32
            -lwininet
            -lversion
            -lole32
            -lws2_32
            -loleaut32
            -limm32
            -lcomdlg32
            -lshlwapi
            -lrpcrt4
            -lopengl32
        )
    elseif (UNIX AND NOT APPLE)
        target_link_libraries (SynthEngineDemo PRIVATE
            -shared
            -fvisibility=default
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
    
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -pthread -fPIC")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}" )
else (NOT MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        target_compile_options (SynthEngineDemo PUBLIC
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
        target_link_options (SynthEngineDemo PUBLIC
	        /MANIFEST#生成清单:是
	        /NXCOMPAT#数据执行保护(DEP):是
	        /DYNAMICBASE#随机基址:是
	        /LARGEADDRESSAWARE#启用大地址:是
	        /SUBSYSTEM:WINDOWS#子系统:窗口
	        /MANIFESTUAC:NO#启用用户账户控制:否
	        /NOLOGO#取消显示启动版权标志:是
        )
    else (CMAKE_BUILD_TYPE STREQUAL Debug)
        target_compile_options (SynthEngineDemo PUBLIC
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
        target_link_options (SynthEngineDemo PUBLIC
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
