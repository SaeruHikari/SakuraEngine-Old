#pragma once
#include "ShaderCompiler/ShaderCompiler.h"

#include "RenderGraph/RenderGraph.h"
#include "RenderGraphWebGPU/RenderGraphWebGPU.h"
#include "RenderGraphVulkan/RenderGraphVulkan.h"

#include "ECS/ECS.h"
#include "Boids.h"

#define TARGET_NUM 20000
const bool useVk = false;

namespace render_system
{
	using namespace sakura;
	using namespace sakura::graphics;

	inline sakura::vector<std::byte> vertex_shader_spirv;
	inline sakura::vector<std::byte> pixel_shader_spirv;
	const sakura::string vertex_shader_hlsl = 
		u8"struct VertexIn\
		{\
			float3 aPos : SV_Position;\
			float3 aCol : COLOR;\
		};\
		struct VertexOut\
		{\
			float4 position : SV_Position;\
			float3 vCol : COLOR;\
		};\
		[[vk::binding(0, 1)]] cbuffer ub\
		{\
			float4x4 view_proj;\
		};\
		[[vk::binding(0, 0)]] cbuffer ub2\
		{\
			float4x4 world;\
			float4x4 world1;\
			float4x4 world2;\
			float4x4 world3;\
		};\
		VertexOut main(VertexIn vin)\
		{\
			VertexOut vout;\
			float4 posW = mul(float4(vin.aPos, 1.0f), world);\
			vout.position = mul(posW, view_proj);\
			vout.vCol = vin.aCol;\
			return vout;\
		}";
	const sakura::string pixel_shader_hlsl =
		u8"struct VertexOut\
		{\
			float4 position : SV_Position;\
			float3 vCol : COLOR;\
		};\
		float4 main(VertexOut pin) : SV_TARGET\
		{\
			return float4(pin.vCol.xyz, 1.f);\
		}";

	inline sakura::Window main_window;
	inline RenderGraph render_graph;
	inline RenderDeviceGroupProxy device_group = RenderDeviceGroupProxy(render_graph);
	inline SwapChainHandle swap_chain = render_graph.SwapChain("DefaultSwapChain");
	inline GpuShaderHandle vertex_shader = render_graph.GpuShaderUnsafe("VertexShader");
	inline GpuShaderHandle pixel_shader = render_graph.GpuShaderUnsafe("PixelShader");
	inline RenderPipelineHandle render_pipeline = render_graph.RenderPipeline("TrianglePipeline");

	inline GpuBufferHandle uniform_buffer = render_graph.GpuBuffer("UniformBuffer");
	inline GpuBufferHandle uniform_buffer_per_object = render_graph.GpuBuffer("UniformBufferPerObject");
	inline GpuBufferHandle uniform_buffer_per_target = render_graph.GpuBuffer("UniformBufferPerTarget");

	inline GpuBufferHandle vertex_buffer = render_graph.GpuBuffer("VertexBuffer");
	inline GpuBufferHandle index_buffer = render_graph.GpuBuffer("IndexBuffer");

	inline GpuBufferHandle vertex_buffer_sphere = render_graph.GpuBuffer("VertexBufferSphere");
	inline GpuBufferHandle index_buffer_sphere = render_graph.GpuBuffer("IndexBufferSphere");

	inline sakura::float4x4 view_proj;
	inline std::vector<sakura::float4x4> worlds(TARGET_NUM * 4);
	inline std::vector<sakura::float4x4> target_worlds(10 * 4);

	class RenderPassSimple : public RenderPass
	{
	public:
		RenderPassSimple(const RenderPassHandle handle, sakura::uint8 cycleCount = 3)
			:RenderPass(handle, cycleCount) {}
		const RenderCommandBuffer& execute(
			RenderCommandBuffer& command_buffer,
			const RenderGraph& rg, IRenderDevice& device) noexcept override
		{
			command_buffer.enqueue<RenderCommandBeginRenderPass>(render_pipeline, attachment);
			Binding binding0 = Binding({
				Binding::Set({
					Binding::Slot(uniform_buffer_per_target, 0, sizeof(sakura::float4x4) * 4, 0)
				}),
				Binding::Set({
					Binding::Slot(uniform_buffer, 0, sizeof(sakura::float4x4), 0)
				})
			});
			//command_buffer.enqueue<RenderCommandSetScissorRect>(
			//	0, device.backend() == EBackend::WebGPU ? main_window.extent().width / 2 + 10 : 0,
			//	main_window.extent().width / 2, main_window.extent().height
			//);
			command_buffer.enqueue<RenderCommandUpdateBinding>(binding0);
			command_buffer.enqueue<RenderCommandDraw>(
				RenderCommandDraw::VB(rg.query<GpuBufferHandle>("VertexBufferSphere")),
				RenderCommandDraw::IB(rg.query<GpuBufferHandle>("IndexBufferSphere"),
					60, EIndexFormat::UINT16)
			);
			auto tn = target_worlds.size() / 4;
			for (auto i = 1u; i < tn; i++)
			{
				Binding binding = Binding({
					Binding::Set({
						Binding::Slot(uniform_buffer_per_target, 0,
							sizeof(sakura::float4x4) * 4, 0)
					}, { sizeof(sakura::float4x4) * i * 4 })
				});
				command_buffer.enqueue<RenderCommandDrawInstancedWithArgs>(binding, 60);
			}

			
			Binding binding00 = Binding({
				Binding::Set({
					Binding::Slot(uniform_buffer_per_object, 0,
						sizeof(sakura::float4x4) * 4, 0)
				}, { 0 }/*dynamic_offsets*/)
			});
			command_buffer.enqueue<RenderCommandUpdateBinding>(binding00);
			command_buffer.enqueue<RenderCommandDraw>(
				RenderCommandDraw::VB(rg.query<GpuBufferHandle>("VertexBuffer")),
				RenderCommandDraw::IB(rg.query<GpuBufferHandle>("IndexBuffer"),
					3, EIndexFormat::UINT16)
			);
			static constexpr size_t N = 10;
			auto bn = worlds.size() / 4;
			for (auto n = 0u; n < N; n++)
			{
				ZoneScopedN("RenderPassExecute");
				for (auto i = 0u; i < bn / N; i++)
				{
					Binding binding = Binding({
						Binding::Set({
							Binding::Slot(uniform_buffer_per_object, 0,
								sizeof(sakura::float4x4) * 4, 
								0)
						}, { sizeof(sakura::float4x4) * (i + bn * n / N) * 4 })
					});
					command_buffer.enqueue<RenderCommandDrawInstancedWithArgs>(binding, 3);
				}
			}
			command_buffer.enqueue<RenderCommandEndRenderPass>();
			return command_buffer;
		}
		bool construct(RenderGraph::Builder& builder) noexcept override
		{
			builder.pipeline(render_pipeline);
			attachment = Attachment({
				Attachment::Slot(swap_chain, sakura::double4(), ELoadOp::Clear, EStoreOp::Store)
			});
			builder.write(attachment);
			return builder.apply();
		}
		Attachment attachment;
	};
	RenderPassHandle pass = render_graph.create_render_pass<RenderPassSimple>();

	void initialize()
	{
		sakura::development::ShaderCompilerVariables vars;
		vars.target_language = sakura::development::EShaderLanguage::Spirv;

		vars.entry = u8"main";
		vars.name = u8"BaseVS";
		vars.freq = sakura::graphics::EShaderFrequency::VertexShader;
		vertex_shader_spirv = 
			sakura::development::compile_hlsl(vertex_shader_hlsl, vars);

		vars.entry = u8"main";
		vars.name = u8"BasePS";
		vars.freq = sakura::graphics::EShaderFrequency::PixelShader;
		pixel_shader_spirv = 
			sakura::development::compile_hlsl(pixel_shader_hlsl, vars);

		sakura::Core::initialize(sakura::Core::Parameters());
		sakura::info("game thread id: {}", std::hash<std::thread::id>()(sakura::Core::get_main_thread_id()));
		
		sakura::Window::desc windDesc;
		windDesc.height = 1080;
		windDesc.width = 1920;
		windDesc.name = "Sakura Engine";
		main_window = sakura::Window::create(windDesc);

		// Create Devices
		DeviceConfiguration deviceConfig;

		if(!useVk)
		{
			deviceConfig.name = "DawnDevice";
			render_graph.emplace_device(new webgpu::RenderDevice(deviceConfig));
			IRenderDevice* dawnDevice = render_graph.get_device("DawnDevice");
			assert(dawnDevice != nullptr && "ERROR: Failed to create Dawn device!");
			device_group.emplace(dawnDevice);
		}
		else
		{
			deviceConfig.name = "VulkanDevice";
			render_graph.emplace_device(new vk::RenderDevice(deviceConfig));
			IRenderDevice* vulaknDevice = render_graph.get_device("VulkanDevice");
			assert(vulaknDevice != nullptr && "ERROR: Failed to create Vulkan device!");
			device_group.emplace(vulaknDevice);
		}

		// Create Swap Chains.
		device_group.create_swap_chain(swap_chain, SwapChainDesc(EPresentMode::Mailbox, main_window, 3));
		// Init RenderPipeline Desc
		RenderPipelineDesc pipelineDesc = RenderPipelineDesc(
			ShaderLayout({
				// Create Actual ShaderResources on Device.
				device_group.create_shader(vertex_shader, ShaderDesc("VertexShader", "main", EShaderFrequency::VertexShader, vertex_shader_spirv)),
				device_group.create_shader(pixel_shader, ShaderDesc("PiexelShader", "main", EShaderFrequency::PixelShader, pixel_shader_spirv))
				}),
			VertexLayout(
				{
					VertexLayout::Element("POSITION", EVertexFormat::FLOAT3, 0),
					VertexLayout::Element("COLOR", EVertexFormat::FLOAT3, 3 * sizeof(float))
				}, VertexLayout::Frequency::PerVertexData, sizeof(float) * 6
			),
			BindingLayout(
				{
					BindingLayout::Set(
					{
						BindingLayout::Slot(
							0, BindingLayout::UniformBuffer, EShaderFrequency::VertexShader),
					}),
					BindingLayout::Set(
					{
						BindingLayout::Slot(
							0, BindingLayout::UniformBuffer, EShaderFrequency::VertexShader),
					}),
				}),
			AttachmentLayout(
#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
				{ AttachmentLayout::Slot(ETextureFormat::R8G8B8A8_UNORM, ELoadOp::Clear, EStoreOp::Store) }
#else
				{ AttachmentLayout::Slot(ETextureFormat::B8G8R8A8_UNORM, ELoadOp::Clear, EStoreOp::Store) }
#endif
			),
			ECullMode::Back, EPrimitiveTopology::TriangleList, EPolygonMode::FILL, 1, 0xFFFFFFFF
		);

		// Create Render pipeline.
		device_group.create_render_pipeline(render_pipeline, pipelineDesc);
		// create the buffers (x, y, r, g, b)
		float const vertData[] = {
			 -4.f, 0.f, -4.f,    0.f / 256.f, 49.f / 256.f, 79.f / 256.f, // BL
			 10.f, 0.f, 0.f,    137.f / 256.f, 190.f / 256.f, 178.f / 256.f, // top
			 -4.f, 0.f, 4.f,     0.f / 256.f, 49.f / 256.f, 79.f / 256.f, // BR
		}; // triangle on XY-Plane.
		uint16_t const indxData[] = { 0, 1, 2, 0 };

		const float X = 0.525731f * 2.f;
		const float Z = 0.850651f * 2.f;
		float const sphere_vertices[] =
		{
			-X, 0.0f, Z,  137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			X, 0.0f, Z,   137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			-X, 0.0f, -Z, 137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			X, 0.0f, -Z,  137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			0.0f, Z, X,   137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			0.0f, Z, -X,  137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			0.0f, -Z, X,  137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			0.0f, -Z, -X, 137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			Z, X, 0.0f,   137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			-Z, X, 0.0f,  137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			Z, -X, 0.0f,  137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
			-Z, -X, 0.0f, 137.f / 256.f, 190.f / 256.f, 178.f / 256.f,
		};
		uint16_t sphere_indices[] =
		{
			1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
			1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
			3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
			10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
		};
		// Create Buffers.
		device_group.create_buffer(uniform_buffer,
			BufferDesc(EBufferUsage::UniformBuffer | EBufferUsage::CopyDst, sizeof(sakura::float4x4), &view_proj));
		device_group.create_buffer(uniform_buffer_per_target,
			BufferDesc(EBufferUsage::UniformBuffer | EBufferUsage::CopyDst, sizeof(sakura::float4x4) * target_worlds.size(), target_worlds.data()));
		device_group.create_buffer(uniform_buffer_per_object,
			BufferDesc(EBufferUsage::UniformBuffer | EBufferUsage::CopyDst, sizeof(sakura::float4x4) * worlds.size(), worlds.data()));

		device_group.create_buffer(vertex_buffer,
			BufferDesc(EBufferUsage::VertexBuffer | EBufferUsage::CopyDst, sizeof(vertData), vertData));
		device_group.create_buffer(index_buffer,
			BufferDesc(EBufferUsage::IndexBuffer | EBufferUsage::CopyDst, sizeof(indxData), &indxData));


		device_group.create_buffer(vertex_buffer_sphere,
			BufferDesc(EBufferUsage::VertexBuffer | EBufferUsage::CopyDst, sizeof(sphere_vertices), sphere_vertices));
		device_group.create_buffer(index_buffer_sphere,
			BufferDesc(EBufferUsage::IndexBuffer | EBufferUsage::CopyDst, sizeof(sphere_indices), &sphere_indices));

		sakura::info("All Tests Passed!");
	}

	namespace task_system = sakura::task_system;
	namespace math = sakura::math;
	using Vector3f = sakura::Vector3f;
	using Quaternion = sakura::Quaternion;
	using Rotator = sakura::Rotator;
	using float4x4 = sakura::float4x4;
	using IModule = sakura::IModule;
	void CollectAndUpload(task_system::ecs::pipeline& ppl, float deltaTime)
	{
		using namespace sakura;
		using namespace sakura::graphics;
		using namespace sakura::ecs;
		ZoneScopedN("CollectAndUpload");
		auto Collect = [&ppl](filters& filter, std::vector<sakura::float4x4>& world, int maxSlice = 500)
		{

			static constexpr auto paramList = boost::hana::make_tuple(
				// read.
				param<const LocalToWorld>
			);
			auto pass = ppl.create_pass(filter, paramList);
			task_system::ecs::schedule_init(ppl, pass,
				[&world](const task_system::ecs::pipeline& pipeline, const task_system::ecs::pass& task_pass)
				{
					world.resize(pipeline.pass_size(task_pass) * 4);
				},
				[&world](const task_system::ecs::pipeline& pipeline, const task_system::ecs::pass& task_pass, const ecs::task& tk)
				{
					ZoneScopedN("CollectJob");
					auto o = operation{ paramList, task_pass, tk };
					auto index = o.get_index();
					const float4x4* l2ws = o.get_parameter<const LocalToWorld>();

					Vector3f pos = Vector3f::vector_zero();
					for (auto i = 0u; i < o.get_count(); i++)
					{
						auto l2w = sakura::math::transpose(l2ws[i]);
						int j = (i + index) * 4;
						world[j] = l2w;
						world[j + 1] = l2w;
						world[j + 2] = l2w;
						world[j + 3] = l2w;
					}
				}, maxSlice);
			return pass;
		};
		std::shared_ptr<task_system::ecs::pass> passes[2];
		{
			filters filter;
			filter.archetypeFilter = {
				{complist<LocalToWorld, BoidTarget>}, //all
				{}, //any
				{} //none
			};
			passes[0] = Collect(filter, target_worlds);
		}
		{
			filters filter;
			filter.archetypeFilter = {
				{complist<LocalToWorld, Boid>}, //all
				{}, //any
				{} //none
			};
			passes[1] = Collect(filter, worlds);
		}
		passes[0]->event.wait();
		passes[1]->event.wait();
		ppl.forget();
	}

	void Present()
	{
		device_group.present(swap_chain);
	}

	void PrepareCommandBuffer(RenderCommandBuffer& buffer)
	{
		ZoneScopedN("PrepareCommandBuffer");

		RenderPass* pass_ptr = render_graph.render_pass(pass);

		pass_ptr->construct(render_graph.builder(pass));
		buffer.reset();
		pass_ptr->execute(buffer, render_graph, device_group);
	}

	void RenderAndPresent(const RenderCommandBuffer& buffer)
	{
		{
			ZoneScopedN("Upload");
			sakura::float4x4 offset = math::make_transform(sakura::Vector3f(0, 0, -1.f) * 500);
			sakura::float4x4 view = sakura::math::look_at_matrix(
				sakura::Vector3f(0, 0, -1.f) * 1, Vector3f::vector_zero());
			sakura::float4x4 proj =
					sakura::math::perspective_fov(0.25f * 3.1415926f * 2, 1080.f / 1920.f, 1.0f, 1000.0f);

			view_proj = sakura::math::multiply(offset, view);
			view_proj = sakura::math::multiply(view_proj, proj);
			view_proj = sakura::math::transpose(view_proj);

			device_group.update_buffer(uniform_buffer, 0, &view_proj, sizeof(view_proj));
			device_group.update_buffer(
			uniform_buffer_per_object, 0, worlds.data(), sizeof(float4x4) * worlds.size());
			device_group.update_buffer(uniform_buffer_per_target,
				0, target_worlds.data(), sizeof(float4x4) * target_worlds.size());
		}

		{
			ZoneScopedN("RenderAndPresent");
			RenderPass* pass_ptr = render_graph.render_pass(pass);
			device_group.execute(buffer, pass_ptr->handle());
			render_system::Present(); // 0
		}
	}
}	