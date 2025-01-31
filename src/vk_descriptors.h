#pragma once

#include <vk_types.h>

class DescriptorLayoutBuilder
{
public:
	void add_binding(uint32_t binding, VkDescriptorType type);
	void clear();
	VkDescriptorSetLayout build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0);

private:
	std::vector<VkDescriptorSetLayoutBinding> _bindings;
};

class DescriptorAllocator
{
public:
	struct PoolSizeRatio
	{
		VkDescriptorType type;
		float ratio;
	};

	void init_pool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
	void clear_descriptors(VkDevice device);
	void destroy_pool(VkDevice device);

	VkDescriptorSet allocate(VkDevice device, VkDescriptorSetLayout layout);

private:
	VkDescriptorPool _pool;
};