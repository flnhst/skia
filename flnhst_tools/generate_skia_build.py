import subprocess
import sys

COMMON_ARGUMENTS = [
    "skia_use_jpeg_gainmaps=false",
    "skia_use_libjpeg_turbo_decode=false",
    "skia_use_libjpeg_turbo_encode=false",
    "skia_use_libpng_decode=false",
    "skia_use_libpng_encode=false",
    "skia_use_libwebp_decode=false",
    "skia_use_libwebp_encode=false",
    "skia_use_expat=false",
    "skia_use_system_icu=false"
]

STATIC_ARGUMENTS = COMMON_ARGUMENTS + [
    "is_official_build=true",
    "is_component_build=false",
    "skia_enable_tools=true"
]

"""

-- OPTIONS = 'target_cpu="x64" skia_enable_android_utils=false skia_enable_spirv_validation=false skia_enable_tools=false skia_enable_gpu_debug_layers=false skia_use_jpeg_gainmaps=false skia_use_libheif=false skia_use_lua=false skia_enable_skshaper=true skia_enable_skparagraph=true skia_enable_skunicode=true target_os="win" skia_enable_bentleyottmann=false is_component_build=true skia_enable_skshaper=true skia_enable_skparagraph=true skia_use_fontconfig=false skia_use_freetype=false skia_use_harfbuzz=true skia_use_icu=true skia_use_system_icu=true skia_use_gl=true skia_use_vulkan=true skia_vulkan_memory_allocator_dir="C:/vcpkg/vcpkg/installed/x64-windows-llvm" win_vc="C:\Program Files\Microsoft Visual Studio\2022\Community\VC" win_sdk="C:\Program Files (x86)\Windows Kits\10"'
-- OPTIONS_DBG = 'is_debug=true     extra_cflags_c=["-FS","-Zi","-Ob0","-Od","-RTC1"]     extra_cflags_cc=["-FS","-Zi","-Ob0","-Od","-RTC1"]'
-- OPTIONS_REL = 'is_official_build=true     extra_cflags_c=["-FS","-O2","-Ob2","-DNDEBUG"]     extra_cflags_cc=["-FS","-O2","-Ob2","-DNDEBUG"]'

"""

VCPKG_ARGUMENTS = [
    "skia_use_system_libjpeg_turbo=false",
    "skia_use_system_zlib=false",
    "skia_use_system_libwebp=false",
    "skia_use_system_libpng=false",
    "skia_use_system_expat=false",
    "skia_use_system_harfbuzz=false",
    "skia_use_system_icu=false",
    # Stuff
    "is_component_build=false",
    "skia_enable_tools=true",
    # Below is from vcpkg
    "target_cpu=\"x64\"",
    "skia_enable_android_utils=false",
    "skia_enable_spirv_validation=false",
    #"skia_enable_tools=false",
    "skia_enable_gpu_debug_layers=false",
    "skia_use_jpeg_gainmaps=false",
    "skia_use_libheif=false",
    "skia_use_lua=false",
    "skia_enable_skshaper=true",
    "skia_enable_skparagraph=true",
    "skia_enable_skunicode=true",
    "target_os=\"win\"",
    "skia_enable_bentleyottmann=false",
    "skia_enable_skshaper=true",
    "skia_enable_skparagraph=true",
    "skia_use_fontconfig=false",
    "skia_use_freetype=false",
    "skia_use_harfbuzz=true",
    "skia_use_icu=true",
    #"skia_use_system_icu=true",
    "skia_use_gl=true",
    "skia_use_vulkan=true",
    #"skia_vulkan_memory_allocator_dir=\"C:/vcpkg/vcpkg/installed/x64-windows-llvm\"",
    "win_vc=\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\"",
    "win_sdk=\"C:\\Program Files (x86)\\Windows Kits\\10\""
]

VCPKG_REL_ARGUMENTS = VCPKG_ARGUMENTS + [
    "is_official_build=true",
    "extra_cflags_c=[\"-FS\",\"-O2\",\"-Ob2\",\"-DNDEBUG\"]",
    "extra_cflags_cc=[\"-FS\",\"-O2\",\"-Ob2\",\"-DNDEBUG\"]"
]

VCPKG_DBG_ARGUMENTS = VCPKG_ARGUMENTS + [
    "is_debug=true",
    "extra_cflags_c=[\"-FS\",\"-Zi\",\"-Ob0\",\"-Od\",\"-RTC1\"]",
    "extra_cflags_cc=[\"-FS\",\"-Zi\",\"-Ob0\",\"-Od\",\"-RTC1\"]"
]

AVAILABLE_ARGUMENT_SETS = {
    "static": STATIC_ARGUMENTS,
    "vcpkg_rel": VCPKG_REL_ARGUMENTS,
    "vcpkg_dbg": VCPKG_DBG_ARGUMENTS,
}

def main(argv):
    if len(argv) < 2:
        print("Please specify the argument set to use:")

        for argument_set_name in AVAILABLE_ARGUMENT_SETS:
            print(f"  {argument_set_name}")

        return 1

    if argv[1] not in AVAILABLE_ARGUMENT_SETS:
        print(f"Unknown argument set '{argv[1]}'.")

        return 1

    selected_argument_set_name = argv[1]
    selected_argument_set = AVAILABLE_ARGUMENT_SETS[selected_argument_set_name]

    print("Generating using the following arguments:")

    for argument in selected_argument_set:
        print(f"  Argument: {argument}")

    args = ""

    for argument in selected_argument_set:
        args += f"{argument} "

    print("Generating...")

    result = subprocess.run(["bin\\gn", "gen", f"out\\{selected_argument_set_name}", "--ide=vs2022", f"--args={args}"])

    result.check_returncode()

    print("Generated.")

    return 0

if __name__ == "__main__":
    exit(main(sys.argv))
