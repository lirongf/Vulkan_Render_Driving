#遍历循环此文件的cpp文件 存入WINDOW变量
file(GLOB_RECURSE WINDOW ./ *.cpp)
#WINDOW值变成一个Lib
add_library(windowLib ${WINDOW})