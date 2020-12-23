/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 * 
 * 
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-08-20 15:56:21
 * @LastEditTime: 2020-08-21 20:03:41
 */
#pragma once
#include "RenderGraphCore.h"
#include "RenderGraph/RenderCommandBuffer.h"
#include "System/Window.h"

namespace sakura::graphics
{
	struct RenderPass;
	struct ISwapChain;
}

namespace sakura::graphics
{
	struct DeviceConfiguration
	{
		sakura::string name;
		sakura::Window window_handle;
	};

	// actual render device class interface
	struct RenderGraphAPI IRenderDevice
	{
		IRenderDevice() = default;
		virtual ~IRenderDevice() = default;

		// cn: 判断一个资源句柄是否再此设备中有对应有效资源.
		// en: Determine whether a resource handle has a corresponding valid resource in this device.
		// jp: リソースハンドルに、デバイスに対応する有効なリソースがあるかどうかを確認.
		virtual bool valid(const RenderResourceHandle handle) const = 0;

		// cn: 销毁句柄在此设备上对应的资源. 成功销毁会释放句柄内索引, 并使对应有效代数+1.
		// en: Destroy the resource corresponding to the handle on this device.
		//     Successful destruction will release index in the handle and make the corresponding generation +1.
		// jp: このデバイスのハンドルに対応するリソースを破棄します。
		//     破棄に成功すると、ハンドルのインデックスが解放され、対応する世代が1つ増えます。
		virtual void destroy_resource(const RenderResourceHandle to_destroy) = 0;

		// cn: 获得设备的名称.
		// en: Get the name of the device.
		// jp: デバイスの名前を取得します.
		virtual sakura::string_view get_name() const = 0;

		// cn: 获得设备的后端.
		// en: Get the backend of the device.
		// jp: デバイスのバックエンドを取得します.
		virtual EBackend backend() const = 0;

		// cn: 从设备请求一条用于资源拷贝的queue. 这样的queue一般是DMA总线的界面, 在桌面平台有多条专门的实例.
		//     对于一些没有专用拷贝队列的平台(例如移动平台)来说, 这个call会直接返回主队列.
		// en: Request a queue for resource copying from the device.
		//     Such a queue is generally the interface of the DMA bus, multiple specialized instances on the desktop/console.
		//     For platforms that have no dedicated copy queues(like mobile), this call directly returns the main queue.
		// jp: デバイスからのリソースコピーのキューを選択します.
		//     このようなキューは通常、DMAバスのインターフェイスであり、デスクトップ/コンソール上の複数の特殊なインスタンスです.
		//     専用のコピーキューがないプラットフォーム（モバイルなど）の場合、この呼び出しはメインキューを直接返します.
		virtual QueueIndex request_copy_queue() const = 0;
		
		// cn: 执行command buffer, 不建立任何fence, 不做任何等待.
		// en: Execute command buffer, establish no fence, do not do any waiting.
		// jp: コマンドバッファを実行し、フェンスを確立せず、待機を行わない
		virtual bool execute_let_fly(const QueueIndex queue, const RenderCommandBuffer& command_buffer) = 0;

		// cn: 执行command buffer, 不建立任何fence, 不做任何等待.
		// en: Execute command buffer, establish no fence, do not do any waiting.
		// jp: コマンドバッファを実行し、フェンスを確立せず、待機を行わない
		virtual bool execute_let_fly(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) = 0;

		// [performance warning]
		// cn: 执行command buffer, 直接阻滞等待到该次执行在GPU完成.
		// en: Execute the command buffer, block and wait until the execution is completed on the GPU.
		// jp: コマンドバッファを実行し、直接ブロックして、GPUで実行が完了するまで待ちます.
		// [performance warning]
		virtual bool execute_block(const QueueIndex queue, const RenderCommandBuffer& command_buffer) = 0;

		// [performance warning]
		// cn: 执行command buffer, 直接阻滞等待到该次执行在GPU完成.
		// en: Execute the command buffer, block and wait until the execution is completed on the GPU.
		// jp: コマンドバッファを実行し、直接ブロックして、GPUで実行が完了するまで待ちます.
		// [performance warning]
		virtual bool execute_block(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) = 0;

		// cn: 执行command buffer, 返回一个可以用来等待该任务完成的fence.
		// en: Execute the command buffer and return a fence, which is used to wait for the task to complete.
		// jp: コマンドバッファを実行し、タスクが完了するのを待つために使用できるフェンスをリターン.
		virtual FenceHandle execute(const QueueIndex queue, const RenderCommandBuffer& command_buffer) = 0;
		
		// cn: 执行command buffer, 返回一个可以用来等待该任务完成的fence.
		// en: Execute the command buffer and return a fence, which is used to wait for the task to complete.
		// jp: コマンドバッファを実行し、タスクが完了するのを待つために使用できるフェンスをリターン.
		virtual FenceHandle execute(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) = 0;

		// [performance warning]
		// cn: 阻滞, 等待该queue中所有任务的完成.
		// en: Block, wait for the completion of all tasks in the queue.
		// jp: ブロックし、キュー内のすべてのタスクを待ちます。
		// [performance warning]
		virtual void wait_idle() = 0;

		
		virtual void write_texture(GpuTextureHandle texture, void const* data, size_t data_size,
			const TextureSlice& slice, const TextureDataLayout& layout, extent3d write_size, QueueIndex queue_index = -1) = 0;
		
		// execute a command buffer
		virtual bool execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl) = 0;
		virtual bool execute(const RenderGraph& graph_to_execute) = 0;
		virtual bool present(const SwapChainHandle handle) { return true; }
		
		virtual GpuShaderHandle create_shader(const GpuShaderHandle handle, const ShaderDesc& desc) = 0;
		virtual GpuBufferHandle create_buffer(const GpuBufferHandle handle, const BufferDesc& desc) = 0;
		virtual GpuTextureHandle create_texture(const GpuTextureHandle handle, const TextureDesc& desc) = 0;

		virtual FenceHandle create_fence(const FenceHandle handle, const FenceDesc& desc) = 0;
		virtual SwapChainHandle create_swap_chain(const SwapChainHandle handle, const SwapChainDesc& desc) = 0;
		virtual RenderPipelineHandle create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDesc& desc) = 0;

		virtual GpuBufferHandle update_buffer(const GpuBufferHandle handle, size_t offset, void* data, size_t size) { return handle; };
		
		[[nodiscard]] virtual IGpuMemoryResource* get_unsafe(const RenderResourceHandle handle) const = 0;
		[[nodiscard]] virtual IGpuMemoryResource* optional_unsafe(const RenderResourceHandle handle) const = 0;
		[[nodiscard]] virtual IGpuObject* get_unsafe(const RenderObjectHandle handle) const = 0;
		[[nodiscard]] virtual IGpuObject* optional_unsafe(const RenderObjectHandle handle) const = 0;
		
		template<typename Type, typename Handle>
		[[nodiscard]] Type* get(const Handle handle) const noexcept;

		template<typename Type, typename Handle>
		[[nodiscard]] Type* optional(const Handle handle) const noexcept;

		virtual void terminate() = 0;
	};

	
	template <typename Type, typename Handle>
	Type* IRenderDevice::get(const Handle handle) const noexcept
	{
		if constexpr (std::is_base_of_v<IGpuObject, Type>)
		{
			static_assert(std::is_base_of_v<RenderObjectHandle, Handle>, "[RGDevice::get]: Handle must be derived from RenderObjectHandle!");
			static_assert(std::is_base_of_v<typename Handle::ObjectType, Type>, "[RGDevice::get]: Handle must match to it's ObjectType!");
			return static_cast<Type*>(get_unsafe(handle));
		}
		else if constexpr (std::is_base_of_v<IGpuMemoryResource, Type>)
		{
			static_assert(std::is_base_of_v<RenderResourceHandle, Handle>, "[RGDevice::get]: Handle must be derived from RenderResourceHandle!");
			static_assert(std::is_base_of_v<typename Handle::ResourceType, Type>, "[RGDevice::get]: Handle must match to it's ResourceType!");
			return static_cast<Type*>(get_unsafe(handle));
		}
		else
		{
			static_assert(std::is_base_of_v<IGpuObject, Type> || std::is_base_of_v<IGpuMemoryResource, Type>, 
				"Type & Handle not matching!");
			return nullptr;
		}
	}

	template <typename Type, typename Handle>
	Type* IRenderDevice::optional(const Handle handle) const noexcept
	{
		if constexpr (std::is_base_of_v<IGpuObject, Type>)
		{
			static_assert(std::is_base_of_v<RenderObjectHandle, Handle>, "[RGDevice::get]: Handle must be derived from RenderObjectHandle!");
			static_assert(std::is_base_of_v<typename Handle::ObjectType, Type>, "[RGDevice::get]: Handle must match to it's ObjectType!");
			return static_cast<Type*>(optional_unsafe(handle));
		}
		else if constexpr (std::is_base_of_v<IGpuMemoryResource, Type>)
		{
			static_assert(std::is_base_of_v<RenderResourceHandle, Handle>, "[RGDevice::get]: Handle must be derived from RenderResourceHandle!");
			static_assert(std::is_base_of_v<typename Handle::ResourceType, Type>, "[RGDevice::get]: Handle must match to it's ResourceType!");
			return static_cast<Type*>(optional_unsafe(handle));
		}
		else
		{
			static_assert(std::is_base_of_v<IGpuObject, Type> || std::is_base_of_v<IGpuMemoryResource, Type>
				, "Type & Handle not matching!");
			return nullptr;
		}
	}

	class RenderDeviceGroupProxy final : public IRenderDevice
	{
	public:
		FORCEINLINE RenderDeviceGroupProxy(const RenderGraph& inRenderGraph)
			:renderGraph(inRenderGraph)
		{
			
		}
		void emplace(IRenderDevice* in_device)
		{
			devices.push_back(in_device);
			name.append(" ").append(sakura::string(in_device->get_name())).append(" &");
		}
		template<typename... Args>
		void emplace(Args... args)
		{
			std::initializer_list<int32_t> {(emplace(args), 0)...};
			std::cout << std::endl;
		}
		FORCEINLINE sakura::size_t count() const
		{
			return devices.size();
		}
		FORCEINLINE bool valid(const RenderResourceHandle shader) const override
		{
			bool result = true;
			for (auto i = 0; i < count(); i++)
			{
				result &= devices[i]->valid(shader);
			}
			return result;
		}
		bool present(const SwapChainHandle handle) override
		{
			bool result = true;
			for (auto i = 0; i < count(); i++)
			{
				result = result & devices[i]->present(handle);
			}
			return result;
		}
		FORCEINLINE bool execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl) override
		{
			bool result = true;
			for (auto i = 0; i < count(); i++)
			{
				result = result & devices[i]->execute(cmdBuffer, hdl);
			}
			return result;
		}
		FORCEINLINE bool execute(const RenderGraph& graph_to_execute) override
		{
			bool result = true;
			for (auto i = 0; i < count(); i++)
			{
				result = result & devices[i]->execute(graph_to_execute);
			}
			return result;
		}
		FORCEINLINE void destroy_resource(const RenderResourceHandle to_destroy) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->destroy_resource(to_destroy);
			}
		}
		FORCEINLINE GpuShaderHandle create_shader(const GpuShaderHandle handle, const ShaderDesc& config) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->create_shader(handle, config);
			}
			return handle;
		}
		FenceHandle create_fence(const FenceHandle handle, const FenceDesc& desc) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->create_fence(handle, desc);
			}
			return handle;
		}
		FORCEINLINE GpuBufferHandle create_buffer(const GpuBufferHandle handle, const BufferDesc& config) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->create_buffer(handle, config);
			}
			return handle;
		}
		FORCEINLINE GpuTextureHandle create_texture(const GpuTextureHandle handle, const TextureDesc& desc) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->create_texture(handle, desc);
			}
			return handle;
		}
		SwapChainHandle create_swap_chain(
			const SwapChainHandle handle, const SwapChainDesc& desc) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->create_swap_chain(handle, desc);
			}
			return handle;
		}

		RenderPipelineHandle create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDesc& desc) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->create_render_pipeline(handle, desc);
			}
			return handle;
		}
		FORCEINLINE void terminate() override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->terminate();
			}
		}
		FORCEINLINE sakura::string_view get_name() const override
		{
			return sakura::string_view(name.c_str(), name.size() - 1);
		}

		GpuBufferHandle update_buffer(const GpuBufferHandle handle, size_t offset, void* data, size_t size) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->update_buffer(handle, offset, data, size);
			}
			return handle;
		}

		EBackend backend() const override
		{
			return EBackend::Count;
		}
		
		IRenderDevice* operator[](int index) const
		{
			return devices[index];
		}

		virtual IGpuMemoryResource* get_unsafe(const RenderResourceHandle handle) const override
		{
			return nullptr;
		}

		virtual IGpuObject* get_unsafe(const RenderObjectHandle handle) const override
		{
			return nullptr;
		}

		virtual IGpuMemoryResource* optional_unsafe(const RenderResourceHandle handle) const override
		{
			return nullptr;
		}

		virtual IGpuObject* optional_unsafe(const RenderObjectHandle handle) const override
		{
			return nullptr;
		}

		QueueIndex request_copy_queue() const override
		{
			return QueueIndex(-1);
		}
		bool execute_let_fly(const QueueIndex queue, const RenderCommandBuffer& command_buffer) override
		{
			return false;
		}
		bool execute_block(const QueueIndex queue, const RenderCommandBuffer& command_buffer) override
		{
			return false;
		}
		FenceHandle execute(const QueueIndex queue, const RenderCommandBuffer& command_buffer) override
		{
			return GenerationalId::UNINITIALIZED;
		}
		void wait_idle() override
		{
			return;
		}

		bool execute_let_fly(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->execute_let_fly(queue, command_buffer);
			}
			return true;
		}
		bool execute_block(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->execute_block(queue, command_buffer);
			}
			return true;
		}
		FenceHandle execute(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) override
		{
			return GenerationalId::UNINITIALIZED;
		}

		void write_texture(GpuTextureHandle texture, void const* data, size_t data_size, const TextureSlice& slice,
			const TextureDataLayout& layout, extent3d write_size, QueueIndex queue_index) override
		{
			for (auto i = 0; i < count(); i++)
			{
				devices[i]->write_texture(texture, data, data_size, slice, layout, write_size, queue_index);
			}
		}
	private:
		sakura::string name = "DeviceGroup:";
		sakura::vector<IRenderDevice*> devices;
		const RenderGraph& renderGraph;
	};
}