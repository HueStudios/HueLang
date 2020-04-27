-- premake5.lua
workspace "HueLang"
   configurations { "Debug", "Release" }

project "HueLang"
   kind "SharedLib"
   language "C"
   targetdir "lib/libhuelang"

   files { "libhuelang/src/**.h", "libhuelang/src/**.c" }
