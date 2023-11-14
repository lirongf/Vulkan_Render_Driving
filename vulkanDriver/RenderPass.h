#ifndef RENDERPASS_H
#define RENDERPASS_H
#include "../base.h"
#include "Device.h"
#include "SubPass.h"
namespace mainNameSpace::VulkanDriver {

	class RenderPass
	{
		
	public:
		using Ptr = std::shared_ptr<RenderPass>;
		
		static Ptr create(const Device::Ptr& device) { return std::make_shared<RenderPass>(device); }
	public:
		RenderPass(const Device::Ptr& device);
		~RenderPass();
		[[nodiscard]] VkRenderPass getRenderPass() const { return mRenderPass; }
		void buildRenderPass();

		void setSubPass(std::vector<SubPass> &subpasses);

		void setDependency(std::vector<VkSubpassDependency> &subpassDependencies);

		void addAttachment(const VkAttachmentDescription attachmentDes);

		uint32_t getID() { return id; };
	private:
		Device::Ptr mDevice{ nullptr };
		VkRenderPass mRenderPass{ VK_NULL_HANDLE };
		
		std::vector<VkSubpassDependency> mDependencies{};
		std::vector<VkAttachmentDescription> mAttachmentDescriptions{};
		std::vector<SubPass> mSubPasses{};

		uint32_t id{0};
		
		
	};
}
#endif
