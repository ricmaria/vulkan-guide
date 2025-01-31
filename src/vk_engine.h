// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>
#include <vk_descriptors.h>


class VulkanEngine
{
private:
	static constexpr unsigned int FRAME_OVERLAP = 2;

public:
	static VulkanEngine& get();

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	//run main loop
	void run();	

private:
	class DeletionQueue
	{
	public:
		void push_function(std::function<void()>&& function)
		{
			_deletors.push_back(function);
		}

		void flush()
		{
			// reverse iterate the deletion queue to execute all the functions
			for (auto it = _deletors.rbegin(); it != _deletors.rend(); it++) {
				(*it)(); //call functors
			}

			_deletors.clear();
		}
	private:
		std::deque<std::function<void()>> _deletors;
	};

	struct FrameData
	{
		VkCommandPool _commandPool;
		VkCommandBuffer _mainCommandBuffer;
		VkSemaphore _swapchainSemaphore;
		VkSemaphore _renderSemaphore;
		VkFence _renderFence;
		DeletionQueue _deletionQueue;
	};

	struct AllocatedImage
	{
		VkImage image;
		VkImageView imageView;
		VmaAllocation allocation;
		VkExtent3D imageExtent;
		VkFormat imageFormat;
	};

	struct ComputePushConstants
	{
		glm::vec4 data1;
		glm::vec4 data2;
		glm::vec4 data3;
		glm::vec4 data4;
	};

	struct ComputeEffect
	{
		const char* name;

		VkPipeline pipeline;
		VkPipelineLayout layout;

		ComputePushConstants data;
	};

	FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; };

	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_sync_structures();
	void init_descriptors();
	void init_pipelines();
	void init_background_pipelines();
	void init_triangle_pipeline();
	void init_imgui();

	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();

	void update_imgui();

	void draw_background(VkCommandBuffer cmd);

	void draw_geometry(VkCommandBuffer cmd);

	void draw_imgui(VkCommandBuffer cmd, VkImageView targetImageView);

	void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

	struct SDL_Window* _window{ nullptr };

	bool _isInitialized{ false };
	int _frameNumber{ 0 };
	bool _stopRendering{ false };
	VkExtent2D _windowExtent{ 1700 , 900 };

	FrameData _frames[FRAME_OVERLAP];

	VkInstance _instance;// Vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger;// Vulkan debug output handle
	VkPhysicalDevice _chosenGPU;// GPU chosen as the default device
	VkDevice _device; // Vulkan device for commands
	VkSurfaceKHR _surface;// Vulkan window surface

	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	DeletionQueue _mainDeletionQueue;

	VmaAllocator _allocator;

	//draw resources
	AllocatedImage _drawImage;
	VkExtent2D _drawExtent;

	DescriptorAllocator _globalDescriptorAllocator;

	VkDescriptorSet _drawImageDescriptors;
	VkDescriptorSetLayout _drawImageDescriptorLayout;

	VkPipeline _computePipeline;
	VkPipelineLayout _computePipelineLayout;

	VkPipelineLayout _trianglePipelineLayout;
	VkPipeline _trianglePipeline;

	// immediate submit structures
	VkFence _immFence;
	VkCommandBuffer _immCommandBuffer;
	VkCommandPool _immCommandPool;

	std::vector<ComputeEffect> _backgroundEffects;
	int _currentBackgroundEffect{ 0 };
};
