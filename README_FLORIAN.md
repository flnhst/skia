# Skia

## Building under Windows:

To configure:

```
bin\gn gen out\Static --args="is_component_build=false skia_use_system_expat=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_zlib=false skia_use_system_harfbuzz=false skia_use_system_libwebp=false skia_use_system_icu=false skia_enable_graphite=true skia_use_vulkan=true skia_enable_tools=true skia_enable_vulkan_debug_layers=false"
```

To build:

```
cd out\Static

ninja
```

List of arguments:

```
bin\gn args out\Static --list
```
