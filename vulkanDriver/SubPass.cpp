#ifndef  SUBPASS_CPP
#define SUBPASS_CPP
#include "SubPass.h"
namespace mainNameSpace::VulkanDriver {
	SubPass::SubPass()
	{
	}

	SubPass::~SubPass()
	{
	}

	//layout ͼ���ͼ������Դ�Ĳ���
	void SubPass::addColorAttachmentReference(uint32_t attachmentIndex, VkImageLayout layout) {
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = attachmentIndex;
		colorAttachmentRef.layout = layout;//����һ����VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		mColorAttachmentReferences.push_back(colorAttachmentRef);
	}

	void SubPass::addInputAttachmentReference(uint32_t attachmentIndex, VkImageLayout layout) {
		VkAttachmentReference InputAttachment{};
		InputAttachment.attachment = attachmentIndex;
		InputAttachment.layout = layout;//
		mInputAttachmentReferences.push_back(InputAttachment);
	}

	void SubPass::setDepthStencilAttachmentReference(uint32_t attachmentIndex) {
		mDepthStencilAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		mDepthStencilAttachmentReference.attachment = attachmentIndex;

	}

	//�ֶ���NULL��һ��ָ��VkAttachmentReference�ṹ�����ָ�룬�ýṹ�����˸�SubPass��Resolve Attachment���䲼��,�ڿ���MSAAʱ���õ���
	void SubPass::setResolveAttachmentReference(uint32_t attachmentIndex) {
		mResolvedAttachmentReference.attachment = attachmentIndex;
		mResolvedAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}

	void SubPass::buildSubPassDescription() {
		mSubPassDescription = {};
		mSubPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		if (!mColorAttachmentReferences.empty()) {
			mSubPassDescription.colorAttachmentCount = static_cast<uint32_t>(mColorAttachmentReferences.size());
			mSubPassDescription.pColorAttachments = mColorAttachmentReferences.data();
		}
		else
			mSubPassDescription.colorAttachmentCount = 0;

		if (!mInputAttachmentReferences.empty()) {
			mSubPassDescription.inputAttachmentCount = static_cast<uint32_t>(mInputAttachmentReferences.size());
			mSubPassDescription.pInputAttachments = mInputAttachmentReferences.data();
		}
		else
			mSubPassDescription.inputAttachmentCount = 0;

		mSubPassDescription.pResolveAttachments = mResolvedAttachmentReference.layout == VK_IMAGE_LAYOUT_UNDEFINED ? nullptr : &mResolvedAttachmentReference;

		mSubPassDescription.pDepthStencilAttachment = mDepthStencilAttachmentReference.layout == VK_IMAGE_LAYOUT_UNDEFINED ? nullptr : &mDepthStencilAttachmentReference;

		//mSubPassDescription.flags
		//mSubPassDescription.pPreserveAttachments
		//mSubPassDescription.preserveAttachmentCount
	}

}

#endif
