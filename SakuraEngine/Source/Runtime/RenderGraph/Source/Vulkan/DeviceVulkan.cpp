#include "RenderGraphVulkan/DeviceVulkan.h"
#include "RenderGraphVulkan/SwapChainVulkan.h"


using namespace sakura::graphics;
using namespace sakura::graphics::vk;

bool bEnableValidationLayers = true;
bool checkValidationLayerSupport(sakura::span<const char*> layers);
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
bool checkDeviceExtensionSupport(VkPhysicalDevice device, std::set<const char*> requiredExtensions);

sakura::graphics::vk::RenderDevice::RenderDevice(const DeviceConfiguration& config)
	:name(config.name)
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "SakuraEngine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// cn: 创建VkInstance.
	// en: Create VkInstance.
	// jp: VkInstanceを作る.
	{
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		sakura::span<const char*> exts_sp = basic_exts;
		createInfo.enabledExtensionCount = exts_sp.size();
		createInfo.ppEnabledExtensionNames = exts_sp.data();
		if (bEnableValidationLayers && !checkValidationLayerSupport(validationLayers))
		{
			sakura::error("validation layers requested, but not available!");
		}

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (bEnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			debugCreateInfo = {};
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = debugCallback;

			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			sakura::error("Vulkan: failed to create instance!");
		}
	}

	// cn: 拾取PhysicalInstance.
	// en: Pick physical device.
	// jp: 物理デバイスを選択する.
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			sakura::error("failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices) 
		{
			VulkanDeviceSet dev = findQueueFamilies(device, config.window_handle);
			// cn: 筛选掉特性不足的设备.
			// en: Filter out devices with insufficient features.
			// jp: 機能が不十分なデバイスを除外する
			if (checkDeviceExtensionSupport(device, std::set<const char*>(basic_device_exts.begin(), basic_device_exts.end())))
			{
				dev.device = device;

				vkGetPhysicalDeviceProperties(device, &dev.properties);
				sakura::info("EnumAllDeviceProperties: DeviceName is {}, VendorId is {}.",
					dev.properties.deviceName, dev.properties.vendorID);
				vkGetPhysicalDeviceFeatures(device, &dev.features);

				device_sets.emplace_back(dev);
			}
		}

		// TODO:
		// cn: 选择最合适的一个设备作为主设备.
		// en: Choose the most suitable device as the master_device.
		// jp: マスターデバイスとして最適なデバイスを選択する.
		{
			master_device_index = 0;
		}

		if (device_sets.empty())
			sakura::error("failed to find a suitable GPU!");
	}

	// cn: 创建逻辑设备.
	// en: Create Logical Device.
	// jp: ロジカル-デバイスを作る.
	{
		bool findGfxQueue = false;
		bool findCmptQueue = false;
		bool findTransferQueue = false;
		bool findPresentQueue = false;
		for (auto& phy_dev : device_sets)
		{
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::vector<float> queuePriority; queuePriority.resize(100, 1.f);
			// Create All Queues.
			for(auto f = 0u; f < phy_dev.queue_families.size(); f++)
			{
				if ( (phy_dev.queue_families[f].queueFlags & VK_QUEUE_GRAPHICS_BIT) || 
					(phy_dev.queue_families[f].queueFlags & VK_QUEUE_COMPUTE_BIT) ||
					(phy_dev.queue_families[f].queueFlags & VK_QUEUE_TRANSFER_BIT) )
				{
					VkDeviceQueueCreateInfo queueCreateInfo{};
					queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					queueCreateInfo.queueFamilyIndex = f;
					queueCreateInfo.queueCount = phy_dev.queue_families[f].queueCount;
					queueCreateInfo.pQueuePriorities = queuePriority.data();
					queueCreateInfos.push_back(queueCreateInfo);
				}
			}

			VkPhysicalDeviceFeatures deviceFeatures{};

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.queueCreateInfoCount = queueCreateInfos.size();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = 0;

			if (bEnableValidationLayers) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();
			}
			else {
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateDevice(phy_dev.device, &createInfo, nullptr, &phy_dev.logical_device) != VK_SUCCESS)
			{
				sakura::error("[DeviceVulkan]: failed to create logical device!");
			}
			else
			{
				for (auto gfxs = 0u; gfxs < phy_dev.graphics_queues.size(); gfxs++)
				{
					VkQueue q = VK_NULL_HANDLE;
					vkGetDeviceQueue(phy_dev.logical_device, phy_dev.graphics_queues[gfxs].family_index, gfxs, &q);
					phy_dev.graphics_queues[gfxs].queue = q;
					findGfxQueue = true;

					// cn: 尝试检查此queue对present的支持.
					// en: Try to check the present support for this queue.
					// jp: このキューのプレゼントへのサポートを確認してみます.
					if (!findPresentQueue)
					{
						for (const auto pf : phy_dev.present_families)
						{
							if (pf == phy_dev.graphics_queues[gfxs].family_index)
							{
								findPresentQueue = true;
								phy_dev.master_queue = phy_dev.graphics_queues[gfxs];
							}
						}
					}
				}
				for (auto gfxs = 0u; gfxs < phy_dev.compute_queues.size(); gfxs++)
				{
					VkQueue q = VK_NULL_HANDLE;
					vkGetDeviceQueue(phy_dev.logical_device, phy_dev.compute_queues[gfxs].family_index, gfxs, &q);
					phy_dev.compute_queues[gfxs].queue = q;
					findCmptQueue = true;
				}
				for (auto gfxs = 0u; gfxs < phy_dev.transfer_queues.size(); gfxs++)
				{
					VkQueue q = VK_NULL_HANDLE;
					vkGetDeviceQueue(phy_dev.logical_device, phy_dev.transfer_queues[gfxs].family_index, gfxs, &q);
					phy_dev.transfer_queues[gfxs].queue = q;
					findTransferQueue = true;
				}

				if (!findPresentQueue)
				{
					sakura::error("Failed to find a proper master_queue! Maybe your graphics queues have no support for present, and currently it's not supported by engine!");
					phy_dev.master_queue = phy_dev.graphics_queues[0];
				}
			}
		}
	}
}

sakura::graphics::vk::RenderDevice::~RenderDevice()
{

}

EBackend RenderDevice::backend() const
{
	return EBackend::Vulkan;
}

bool sakura::graphics::vk::RenderDevice::valid(const RenderShaderHandle shader) const
{
	return false;
}

void sakura::graphics::vk::RenderDevice::destroy_resource(const RenderShaderHandle to_destroy)
{
	return;
}

sakura::string_view sakura::graphics::vk::RenderDevice::get_name() const
{
	return name;
}

bool sakura::graphics::vk::RenderDevice::execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl, const size_t frame)
{
	return false;
}

bool sakura::graphics::vk::RenderDevice::execute(const RenderGraph& graph_to_execute)
{
	return false;
}

bool sakura::graphics::vk::RenderDevice::present(const SwapChainHandle handle)
{
	return false;
}

void sakura::graphics::vk::RenderDevice::terminate()
{
	for (auto& phy_dev : device_sets)
	{
		vkDestroyDevice(phy_dev.logical_device, nullptr);
	}
	if (bEnableValidationLayers) 
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroyInstance(instance, nullptr);
	return;
}

sakura::graphics::RenderShaderHandle sakura::graphics::vk::RenderDevice::create_shader(const RenderShaderHandle handle, const ShaderDesc& config)
{

	return handle;
}

sakura::graphics::RenderBufferHandle sakura::graphics::vk::RenderDevice::create_buffer(const RenderBufferHandle handle, const BufferDesc& config)
{

	return handle;
}

sakura::graphics::RenderAttachmentHandle sakura::graphics::vk::RenderDevice::create_render_attachment(const RenderAttachmentHandle handle, const Attachment& config)
{

	return handle;
}

sakura::graphics::FenceHandle sakura::graphics::vk::RenderDevice::create_fence(const FenceHandle handle, const FenceDesc& desc)
{

	return handle;
}

sakura::graphics::SwapChainHandle sakura::graphics::vk::RenderDevice::create_swap_chain(const SwapChainHandle handle, const SwapChainDesc& desc)
{
	return _create_object_impl<SwapChain, SwapChainHandle>(handle, *this, desc);
}

sakura::graphics::RenderPipelineHandle sakura::graphics::vk::RenderDevice::create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDesc& desc)
{

	return handle;
}

sakura::graphics::RenderBufferHandle sakura::graphics::vk::RenderDevice::update_buffer(const RenderBufferHandle handle, size_t offset, void* data, size_t size)
{

	return handle;
}

sakura::graphics::IGPUBuffer* sakura::graphics::vk::RenderDevice::get(const RenderBufferHandle handle) const
{

	return nullptr;
}

sakura::graphics::IGPUShader* sakura::graphics::vk::RenderDevice::get(const RenderShaderHandle handle) const
{

	return nullptr;
}

sakura::graphics::IRenderPipeline* sakura::graphics::vk::RenderDevice::get(const RenderPipelineHandle handle) const
{

	return nullptr;
}

sakura::graphics::ISwapChain* sakura::graphics::vk::RenderDevice::get(const SwapChainHandle handle) const
{

	return nullptr;
}

sakura::graphics::IFence* sakura::graphics::vk::RenderDevice::get(const FenceHandle handle) const
{

	return nullptr;
}

sakura::graphics::IGPUBuffer* sakura::graphics::vk::RenderDevice::optional(const RenderBufferHandle handle) const
{

	return nullptr;
}

sakura::graphics::IGPUShader* sakura::graphics::vk::RenderDevice::optional(const RenderShaderHandle handle) const
{

	return nullptr;
}

sakura::graphics::IRenderPipeline* sakura::graphics::vk::RenderDevice::optional(const RenderPipelineHandle handle) const
{

	return nullptr;
}

sakura::graphics::ISwapChain* sakura::graphics::vk::RenderDevice::optional(const SwapChainHandle handle) const
{

	return nullptr;
}

sakura::graphics::IFence* sakura::graphics::vk::RenderDevice::optional(const FenceHandle handle) const
{

	return nullptr;
}



// vk-specified:

// cn: 检查设备扩展的支持.
// en: Check device extensions support.
// jp: デバイス-エクステンションのチェック.
bool checkDeviceExtensionSupport(VkPhysicalDevice device, std::set<const char*> requiredExtensions)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

VulkanDeviceSet RenderDevice::findQueueFamilies(VkPhysicalDevice device, sakura::Window wind)
{
	VulkanDeviceSet pd;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	pd.queue_families.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, pd.queue_families.data());

	VkSurfaceKHR surface = create_surface(wind);
	uint32 i = 0u;
	bool findPresentQueue = false;
	for (const auto& queueFamily : pd.queue_families)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			pd.graphics_families.emplace_back(i);
			pd.graphics_queues.resize(queueFamily.queueCount, VulkanQueue{ i, VK_NULL_HANDLE });

		}
		else if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			pd.compute_families.emplace_back(i);
			pd.compute_queues.resize(queueFamily.queueCount, VulkanQueue{ i, VK_NULL_HANDLE });
		}
		else if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			pd.transfer_families.emplace_back(i);
			pd.transfer_queues.resize(queueFamily.queueCount, VulkanQueue{ i, VK_NULL_HANDLE });
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport)
		{
			findPresentQueue = true;
			pd.present_families.emplace_back(i);
		}

		i++;
	}
	if (!findPresentQueue)
	{
		sakura::error("Failed to find a present queue!");
	}
	vkDestroySurfaceKHR(instance, surface, nullptr);
	return pd;
}

VkSurfaceKHR sakura::graphics::vk::RenderDevice::create_surface(Window window) const
{
	VkSurfaceKHR surface;
#ifdef SAKURA_TARGET_PLATFORM_WIN
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = static_cast<HWND>(window.handle());
	createInfo.hinstance = GetModuleHandle(nullptr);
	if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
	{
		sakura::error("failed to create window surface!");
	}
#else
	static_assert(0, "Implement This!");
#endif
	return surface;
}



VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

bool checkValidationLayerSupport(sakura::span<const char*> layers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : layers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageType && VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
	{
		sakura::info("[VkGeneralMessage]: {}", pCallbackData->pMessage);
	}
	else
	{
		sakura::error("[VkValidation/Performance]: {}", pCallbackData->pMessage);
	}
	return VK_FALSE;
}