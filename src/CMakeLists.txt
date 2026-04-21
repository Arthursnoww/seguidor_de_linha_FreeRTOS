cmake_minimum_required(VERSION 3.13)

# Caminho para o SDK da Raspberry Pi Pico
set(PICO_SDK_PATH "../../../pico-sdk")
set(PICO_BOARD "pico_w")

# Importar SDK e FreeRTOS
include(../pico_sdk_import.cmake)
include(../FreeRTOS_Kernel_import.cmake)

project(seguidor C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Inicializar o SDK
pico_sdk_init()

# Executável principal com heap_4.c renomeado como heap.c
add_executable(seguidor main.c)

# Link com bibliotecas necessárias
target_link_libraries(seguidor pico_stdlib FreeRTOS-Kernel FreeRTOS-Kernel-Heap4 pico_cyw43_arch_none pico_stdio_usb hardware_pwm hardware_adc)

# Geração dos formatos bin/hex/uf2 etc.
pico_add_extra_outputs(seguidor)

# Incluir diretórios de cabeçalhos
target_include_directories(seguidor PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    /home/arthur/2025.1/rtos/seguidor/inc                
    ${CMAKE_CURRENT_LIST_DIR}/src
    ../FreeRTOS-Kernel/include
    ../FreeRTOS-Kernel/portable/GCC/RP2040
    ${PICO_SDK_PATH}/src/rp2_common/hardware_adc/include
)

# Cópia automática do UF2 após build
add_custom_command(TARGET seguidor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copiando UF2 para a Pico W..."
    COMMAND ${CMAKE_COMMAND} -E sleep 1
    COMMAND ${CMAKE_COMMAND} -E copy seguidor.uf2 /media/arthur/RPI-RP2
    COMMENT "Copiando arquivo UF2 para o Raspberry Pi Pico"
)
