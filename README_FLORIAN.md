# Skia

## How it is setup

Skia is in `celaeno-registry` using this repository. There are some patch files there as well, nothing we wrote.

We build dynamically because Skia links against release builds of the VC runtime otherwise there would be conflicts with the iterator debug level.

## Building under Windows:

To configure:

```
bin\gn gen out\Dynamic --args="is_component_build=true skia_use_system_expat=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_zlib=false skia_use_system_harfbuzz=false skia_use_system_libwebp=false skia_use_system_icu=false skia_enable_graphite=true skia_use_vulkan=true skia_enable_tools=true skia_enable_vulkan_debug_layers=false skia_enable_skottie=false skia_enable_skparagraph=true skia_enable_skshaper=true"
```

To build:

```
cd out\Dynamic

ninja
```

List of arguments:

```
bin\gn args out\Dynamic --list
```
