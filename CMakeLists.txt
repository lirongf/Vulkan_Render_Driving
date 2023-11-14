#��͵�Cmake�汾
cmake_minimum_required(VERSION 3.12)  

#��Ŀ����
project(Vulkan_Render_Driving)  

#ʹ��C++��׼17
set(CMAKE_CXX_STANDARD 17)   

#.��ʾ��Ŀ¼  ��仰��˼����Ѱ��Ŀ¼�����е�cpp �����DIRSRCS������DIRSRCS���Լ�����ı���
aux_source_directory(. DIRSRCS)  

#����������� ÿ�������Լ���include�ļ���,���䵽����������У������·�������÷�
#${CMAKE_CURRENT_SOURCE_DIR}��ָ��CmakeList�ļ�·��,�ҵ����������Include�ͺ�
include_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/glmbuildOut/include
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/glfwbuildOut/include
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../../vulkanSDK/Include
)

#ÿ���������ⶼ�������Լ���dll��lib�⣬�ҵ����·����lib��dll�⣬���߳���ȥ�ĸ��ļ����������ĸ���
link_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../../vulkanSDK/Lib
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/glfwbuildOut/lib
)

#�����Ǳ������ļ��е�CmakeList
add_subdirectory(vulkanDriver)
add_subdirectory(Window)
add_subdirectory(vulkanDriverFactory)

#���ɿ�ִ���ļ�Vulkan_Render_Driving,${DIRSRCS}��ʾ��DIRSRCSչ�����ļ������ɿ�ִ���ļ�
add_executable(Vulkan_Render_Driving ${DIRSRCS}  "main.cpp")  

#�ҵ�Lib�������lib���ļ����������ӵ�Vulkan_Render_Driving��������
target_link_libraries(Vulkan_Render_Driving glfw3.lib vulkan-1.lib windowLib vulkanLib vulkanFactoryLib)