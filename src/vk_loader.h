#pragma once

#include <vk_types.h>
#include <unordered_map>
#include <filesystem>
#include <vk_descriptors.h>

struct GLTFMaterial
{
	MaterialInstance data;
};

struct GeoSurface
{
	uint32_t startIndex;
	uint32_t count;
	std::shared_ptr<GLTFMaterial> material;
};

struct MeshAsset
{
	std::string name;

	std::vector<GeoSurface> surfaces;
	GPUMeshBuffers meshBuffers;
};

struct MeshNode : public Node
{
	std::shared_ptr<MeshAsset> mesh;

	virtual void draw(const glm::mat4& topMatrix, DrawContext& ctx) override;
};

//forward declaration
class VulkanEngine;

namespace fastgltf
{
	class Asset;
	struct Image;
}

struct LoadedGLTF : public IRenderable
{
	// storage for all the data on a given glTF file
	std::unordered_map<std::string, std::shared_ptr<MeshAsset>> meshes;
	std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
	std::unordered_map<std::string, AllocatedImage> images;
	std::unordered_map<std::string, std::shared_ptr<GLTFMaterial>> materials;

	// nodes that dont have a parent, for iterating through the file in tree order
	std::vector<std::shared_ptr<Node>> topNodes;

	std::vector<VkSampler> samplers;

	DescriptorAllocatorGrowable descriptorPool;

	AllocatedBuffer materialDataBuffer;

	VulkanEngine* creator;

	~LoadedGLTF() { clear_all(); };

	virtual void draw(const glm::mat4& topMatrix, DrawContext& ctx);

	static std::optional<std::vector<std::shared_ptr<MeshAsset>>> load_gltf_meshes(VulkanEngine* engine, std::filesystem::path filePath);	// TODO: remove

	static std::optional<std::shared_ptr<LoadedGLTF>> load_gltf(VulkanEngine* engine, std::string_view filePath);

	static std::optional<AllocatedImage> load_image(VulkanEngine* engine, fastgltf::Asset& asset, fastgltf::Image& image);

private:

	void clear_all();
};