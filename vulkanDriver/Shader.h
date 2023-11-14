#ifndef SHADER_H
#define SHADER_H

#include "../base.h"
#include "Device.h"
namespace mainNameSpace::VulkanDriver {
	class Shader
	{
	public:
		using Ptr = std::shared_ptr<Shader>;
		static Ptr create(const Device::Ptr& device, std::vector<char>& codeBuffer , VkShaderStageFlagBits shaderStage, const std::string& entryPoint) {
			return std::make_shared<Shader>(device, codeBuffer, shaderStage, entryPoint);
		}
		Shader(const Device::Ptr& device, std::vector<char>& codeBuffer, VkShaderStageFlagBits shaderStage, const std::string& entryPoint);
		
		~Shader();


		[[nodiscard]] auto getShaderStage() const { return mShaderStage; }
		[[nodiscard]] auto& getShaderEntryPoint() const { return mEntryPoint; }
		[[nodiscard]] auto getShaderModule() const { return mShaderModule; }

	private:
		Device::Ptr mDevice{ nullptr };
		VkShaderModule mShaderModule{ VK_NULL_HANDLE };
		std::string mEntryPoint;
		VkShaderStageFlagBits mShaderStage;
	};
}
#endif //!SHADER_H

