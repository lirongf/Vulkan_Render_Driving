#遍历循环此文件的cpp文件 存入VULKAN变量
file(GLOB_RECURSE VULKANFACTORY ./ *.cpp)
#VULKAN值变成一个Lib
add_library(vulkanFactoryLib ${VULKANFACTORY})