#最低的Cmake版本
cmake_minimum_required(VERSION 3.12)  

#项目名称
project(Vulkan_Render_Driving)  

#使用C++标准17
set(CMAKE_CXX_STANDARD 17)   

#.表示本目录  这句话意思是搜寻本目录下所有的cpp 打包到DIRSRCS变量，DIRSRCS是自己定义的变量
aux_source_directory(. DIRSRCS)  

#引入第三方库 每个库有自己的include文件夹,将配到下面的命令中，是相对路径的配置法
#${CMAKE_CURRENT_SOURCE_DIR}是指此CmakeList文件路径,找到第三方库的Include就好
include_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/glmbuildOut/include
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/glfwbuildOut/include
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../../vulkanSDK/Include
)

#每个第三方库都可能有自己的dll和lib库，找到相对路径的lib和dll库，告诉程序去哪个文件夹下面找哪个库
link_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../../vulkanSDK/Lib
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/glfwbuildOut/lib
)

#下面是遍历子文件夹的CmakeList
add_subdirectory(vulkanDriver)
add_subdirectory(Window)
add_subdirectory(vulkanDriverFactory)

#生成可执行文件Vulkan_Render_Driving,${DIRSRCS}表示用DIRSRCS展开的文件来生成可执行文件
add_executable(Vulkan_Render_Driving ${DIRSRCS}  "main.cpp")  

#找到Lib库里面的lib个文件，并且链接到Vulkan_Render_Driving主程序上
target_link_libraries(Vulkan_Render_Driving glfw3.lib vulkan-1.lib windowLib vulkanLib vulkanFactoryLib)