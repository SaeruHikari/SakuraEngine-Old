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

	// cn: 渲染设备的接口.
	// en: actual render device class interface.
	// jp: レンダリングデバイスインターフェイス.
	struct RenderGraphAPI IRenderDevice
	{
		IRenderDevice() = default;
		virtual ~IRenderDevice() = default;

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
		// jp: コマンドバッファを実行し、フェンスを確立せず、アイドルを行わない
		virtual bool execute_let_fly(const QueueIndex queue, const RenderCommandBuffer& command_buffer) = 0;

		// cn: 执行command buffer, 不建立任何fence, 不做任何等待.
		// en: Execute command buffer, establish no fence, do not do any waiting.
		// jp: コマンドバッファを実行し、フェンスを確立せず、アイドルを行わない
		virtual bool execute_let_fly(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) = 0;

		// [性能警告] [performance warning] [パフォーマンス警告]
		// cn: 执行command buffer, 直接阻滞等待到该次执行在GPU完成.
		// en: Execute the command buffer, block and wait until the execution is completed on the GPU.
		// jp: コマンドバッファを実行し、直接ブロックして、GPUで実行が完了するまで待ちます.
		// [性能警告] [performance warning] [パフォーマンス警告]
		virtual bool execute_block(const QueueIndex queue, const RenderCommandBuffer& command_buffer) = 0;

		// [性能警告] [performance warning] [パフォーマンス警告]
		// cn: 执行command buffer, 直接阻滞等待到该次执行在GPU完成.
		// en: Execute the command buffer, block and wait until the execution is completed on the GPU.
		// jp: コマンドバッファを実行し、直接ブロックして、GPUで実行が完了するまで待ちます.
		// [性能警告] [performance warning] [パフォーマンス警告]
		virtual bool execute_block(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) = 0;

		// cn: 执行command buffer, 返回一个可以用来等待该任务完成的fence.
		// en: Execute the command buffer and return a fence, which is used to wait for the task to complete.
		// jp: コマンドバッファを実行し、タスクが完了するのを待つために使用できるフェンスをリターン.
		virtual FenceHandle execute(const QueueIndex queue, const RenderCommandBuffer& command_buffer) = 0;
		
		// cn: 执行command buffer, 返回一个可以用来等待该任务完成的fence.
		// en: Execute the command buffer and return a fence, which is used to wait for the task to complete.
		// jp: コマンドバッファを実行し、タスクが完了するのを待つために使用できるフェンスをリターン.
		virtual FenceHandle execute(const ERenderQueueType queue, const RenderCommandBuffer& command_buffer) = 0;

		// [性能警告] [performance warning] [パフォーマンス警告]
		// cn: 阻滞, 等待该queue中所有任务的完成.
		// en: Block, wait for the completion of all tasks in the queue.
		// jp: ブロックし、キュー内のすべてのタスクを待ちます。
		// [性能警告] [performance warning] [パフォーマンス警告]
		virtual void wait_idle() = 0;

		// [性能警告] [performance warning] [パフォーマンス警告]
		// cn: 阻滞, 将指定的数据上传到texture上.
		// en: Block, upload the specified data to the texture.
		// jp: ブロックし、指定したデータをテクスチャにアップロードします。
		// [性能警告] [performance warning] [パフォーマンス警告]
		virtual void write_texture(GpuTextureHandle texture, void const* data, size_t data_size,
			const TextureSlice& slice, const TextureDataLayout& layout, extent3d write_size, QueueIndex queue_index = -1) = 0;
		
		// execute a command buffer
		virtual bool execute(const RenderCommandBuffer& cmdBuffer, const RenderPassHandle hdl) = 0;
		virtual bool execute(const RenderGraph& graph_to_execute) = 0;
		virtual bool present(const SwapChainHandle handle) { return true; }
		
		// cn: 判断一个资源句柄是否再此设备中有对应有效资源.
		// en: Determine whether a resource handle has a corresponding valid resource in this device.
		// jp: リソースハンドルに、デバイスに対応する有効なリソースがあるかどうかを確認.
		virtual bool valid(const RenderResourceHandle handle) const = 0;

		// cn: 销毁句柄在此设备上对应的资源. 成功销毁会释放句柄内索引, 并使对应有效代数+1.
		// en: Destroy the resource corresponding to the handle on this device.
		//     Successful destruction will release index in the handle and make the corresponding generation +1.
		// jp: このデバイスのハンドルに対応するリソースを破棄します。
		//     破棄に成功すると、ハンドルのインデックスが解放され、対応するジェネレーションが1つ増えます。
		virtual void destroy(const RenderResourceHandle to_destroy) = 0;

		virtual GpuShaderHandle create_shader(const GpuShaderHandle handle, const ShaderDescriptor& desc) = 0;
		virtual GpuBufferHandle create_buffer(const GpuBufferHandle handle, const BufferDescriptor& desc) = 0;
		virtual GpuTextureHandle create_texture(const GpuTextureHandle handle, const TextureDescriptor& desc) = 0;
		virtual GpuSamplerHandle create_sampler(const GpuSamplerHandle handle, const SamplerDescriptor& desc) = 0;

		virtual FenceHandle create_fence(const FenceHandle handle, const FenceDescriptor& desc) = 0;
		virtual SwapChainHandle create_swap_chain(const SwapChainHandle handle, const SwapChainDescriptor& desc) = 0;
		virtual RenderPipelineHandle create_render_pipeline(
			const RenderPipelineHandle handle, const RenderPipelineDescriptor& desc) = 0;
		virtual ComputePipelineHandle create_compute_pipeline(
			const ComputePipelineHandle handle, const ComputePipelineDescriptor& desc) = 0;

		virtual GpuBufferHandle update_buffer(const GpuBufferHandle handle, size_t offset, void* data, size_t size) = 0;

		// TODO
		// cn: resize一个GpuBuffer. 在此操作后该Buffer的索引保持在原地, 代数会+1.
		// en: Resize a GpuBuffer. After this operation, the Buffer index remains in place, and the generation will be +1.
		// jp: GpuBufferのサイズを変更する。この後、バッファインデックスはそのまま残り、ジェネレーションは+1になります。
		//virtual GpuBufferHandle resize(const GpuBufferHandle handle, size_t size, void* data = nullptr, size_t to = 0);
		// TODO
		
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
}