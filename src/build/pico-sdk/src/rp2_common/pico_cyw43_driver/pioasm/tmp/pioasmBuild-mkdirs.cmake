# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/arthur/2025.1/rtos/pico-sdk/tools/pioasm"
  "/home/arthur/2025.1/rtos/hcsr04/src/build/pioasm"
  "/home/arthur/2025.1/rtos/hcsr04/src/build/pioasm-install"
  "/home/arthur/2025.1/rtos/hcsr04/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/arthur/2025.1/rtos/hcsr04/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "/home/arthur/2025.1/rtos/hcsr04/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/arthur/2025.1/rtos/hcsr04/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/arthur/2025.1/rtos/hcsr04/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/arthur/2025.1/rtos/hcsr04/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
