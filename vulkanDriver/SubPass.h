#ifndef  SUBPASS_H
#define SUBPASS_H
#include "../base.h"
namespace mainNameSpace::VulkanDriver {
	class SubPass
	{
	public:
		SubPass();
		~SubPass();

		void addColorAttachmentReference(uint32_t attachmentIndex, VkImageLayout layout);
		//�Ƿ����ϴ���Ⱦ�Ľ��ʹ��
		void addInputAttachmentReference(uint32_t attachmentIndex, VkImageLayout layout);

		void setDepthStencilAttachmentReference(uint32_t attachmentIndex);
		//MSAA����
		void setResolveAttachmentReference(uint32_t attachmentIndex);

		void buildSubPassDescription();

		[[nodiscard]] auto getSubPassDescription() const { return mSubPassDescription; }

	private:
		VkSubpassDescription mSubPassDescription{};
		std::vector<VkAttachmentReference> mColorAttachmentReferences{};
		std::vector<VkAttachmentReference> mInputAttachmentReferences{};
		VkAttachmentReference mDepthStencilAttachmentReference{};
		VkAttachmentReference mResolvedAttachmentReference{};
	};
}
#endif // ! SUBPASS_H



