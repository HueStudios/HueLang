-- premake5.lua
workspace "HueLang"
   configurations { "Debug", "Release" }

project "HueLang"
   kind "SharedLib"
   language "C"
   targetdir "lib/libhuelang"

   files { "src/libhuelang/**.h", "src/libhuelang/**.c" }
