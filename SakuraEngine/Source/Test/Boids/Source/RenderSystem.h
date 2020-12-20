#pragma once
#include "ShaderCompiler/ShaderCompiler.h"

#include "RenderGraph/RenderGraph.h"
#include "RenderGraphWebGPU/RenderGraphWebGPU.h"
#include "RenderGraphVulkan/RenderGraphVulkan.h"

#include "ECS/ECS.h"
#include "Boids.h"

#define TARGET_NUM 5000

namespace render_system
{
	using namespace sakura;
	using namespace sakura::graphics;

	sakura::vector<std::byte> vertexShaderSpirv;
	sakura::vector<std::byte> pixelShaderSpirv;
	sakura::string vertexShaderHLSL = 
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
			float4x4 viewProj;\
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
			vout.position = mul(posW, viewProj);\
			vout.vCol = vin.aCol;\
			return vout;\
		}";
	sakura::string pixelShaderHLSL =
		u8"struct VertexOut\
		{\
			float4 position : SV_Position;\
			float3 vCol : COLOR;\
		};\
		float4 main(VertexOut pin) : SV_TARGET\
		{\
			return float4(pin.vCol.xyz, 1.f);\
		}";

	sakura::Window mainWindow;
	RenderGraph render_graph;
	RenderDeviceGroupProxy deviceGroup(render_graph);
	SwapChainHandle swapChain = render_graph.SwapChain("DefaultSwapChain");
	RenderShaderHandle vertexShader = render_graph.RenderShaderUnsafe("VertexShader");
	RenderShaderHandle pixelShader = render_graph.RenderShaderUnsafe("PixelShader");
	RenderPipelineHandle renderPipeline = render_graph.RenderPipeline("TrianglePipeline");

	RenderBufferHandle uniformBuffer = render_graph.RenderBuffer("UniformBuffer");
	RenderBufferHandle uniformBufferPerObject = render_graph.RenderBuffer("UniformBufferPerObject");
	RenderBufferHandle uniformBufferPerTarget = render_graph.RenderBuffer("UniformBufferPerTarget");	
	RenderBufferHandle uniformBufferPerObject1 = render_graph.RenderBuffer("UniformBufferPerObject");
	RenderBufferHandle uniformBufferPerTarget1 = render_graph.RenderBuffer("UniformBufferPerTarget");

	RenderBufferHandle vertexBuffer = render_graph.RenderBuffer("VertexBuffer");
	RenderBufferHandle indexBuffer = render_graph.RenderBuffer("IndexBuffer");	

	RenderBufferHandle vertexBufferSphere = render_graph.RenderBuffer("VertexBufferSphere");
	RenderBufferHandle indexBufferSphere = render_graph.RenderBuffer("IndexBufferSphere");

	sakura::float4x4 viewProj;
	std::vector<sakura::float4x4> worlds(TARGET_NUM * 4);
	std::vector<sakura::float4x4> targetWorlds(10 * 4);

	class RenderPassSimple : public RenderPass
	{
	public:
		RenderPassSimple(const RenderPassHandle handle, sakura::uint8 cycleCount = 3)
			:RenderPass(handle, cycleCount) {}
		const RenderCommandBuffer& execute(
			RenderCommandBuffer& command_buffer,
			const RenderGraph& rg, IRenderDevice& device) noexcept override
		{
			command_buffer.enqueue<RenderCommandBeginRenderPass>(renderPipeline, attachment);
			Binding binding0 = Binding({
				Binding::Set({
					Binding::Slot(uniformBufferPerObject, 0, sizeof(sakura::float4x4) * 4, 0)
				}),
				Binding::Set({
					Binding::Slot(uniformBuffer, 0, sizeof(sakura::float4x4), 0)
				})
			});
			//command_buffer.enqueue<RenderCommandSetScissorRect>(
			//	0, device.backend() == EBackend::WebGPU ? mainWindow.extent().width / 2 + 10 : 0,
			//	mainWindow.extent().width / 2, mainWindow.extent().height
			//);
			command_buffer.enqueue<RenderCommandUpdateBinding>(binding0);
			command_buffer.enqueue<RenderCommandDraw>(
				RenderCommandDraw::VB(rg.blackboard<RenderBufferHandle>("VertexBufferSphere")),
				RenderCommandDraw::IB(rg.blackboard<RenderBufferHandle>("IndexBufferSphere"),
					60, EIndexFormat::UINT16)
			);
			auto tn = targetWorlds.size() / 4;
			for (auto i = 1u; i < tn; i++)
			{
				Binding binding = Binding({
					Binding::Set({
						Binding::Slot(uniformBufferPerTarget, 0,
							sizeof(sakura::float4x4) * 4, sizeof(sakura::float4x4) * i * 4)
					})
				}, { sizeof(sakura::float4x4) * i * 4 });
				command_buffer.enqueue<RenderCommandDrawInstancedWithArgs>(binding, 60);
			}
			Binding binding00 = Binding({
				Binding::Set({
					Binding::Slot(uniformBufferPerObject, 0,
						sizeof(sakura::float4x4) * 4, 0)
				}) 
			}, { 0 }/*dynamic_offsets*/);
			command_buffer.enqueue<RenderCommandUpdateBinding>(binding00);
			command_buffer.enqueue<RenderCommandDraw>(
				RenderCommandDraw::VB(rg.blackboard<RenderBufferHandle>("VertexBuffer")),
				RenderCommandDraw::IB(rg.blackboard<RenderBufferHandle>("IndexBuffer"),
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
							Binding::Slot(uniformBufferPerObject, 0,
								sizeof(sakura::float4x4) * 4, 
								sizeof(sakura::float4x4) * (i + bn * n / N) * 4)
						})
					}, { sizeof(sakura::float4x4) * (i + bn * n / N) * 4 });
					command_buffer.enqueue<RenderCommandDrawInstancedWithArgs>(binding, 3);
				}
			}
			command_buffer.enqueue<RenderCommandEndRenderPass>();
			return command_buffer;
		}
		bool construct(RenderGraph::Builder& rg) noexcept override
		{
			attachment = Attachment({
				Attachment::Slot(swapChain, sakura::double4(), ELoadOp::Clear, EStoreOp::Store)
			});

			return true;
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
		vertexShaderSpirv = 
			sakura::development::compile_hlsl(vertexShaderHLSL, vars);

		vars.entry = u8"main";
		vars.name = u8"BasePS";
		vars.freq = sakura::graphics::EShaderFrequency::PixelShader;
		pixelShaderSpirv = 
			sakura::development::compile_hlsl(pixelShaderHLSL, vars);

		sakura::Core::initialize(sakura::Core::Parameters());
		sakura::info("game thread id: {}", std::hash<std::thread::id>()(sakura::Core::get_main_thread_id()));
		
		sakura::Window::desc windDesc;
		windDesc.height = 1080;
		windDesc.width = 1920;
		windDesc.name = "Sakura Engine";
		mainWindow = sakura::Window::create(windDesc);

		// Create Devices
		DeviceConfiguration deviceConfig;
		deviceConfig.name = "DawnDevice";
		render_graph.emplace_device(new webgpu::RenderDevice(deviceConfig));
		IRenderDevice* dawnDevice = render_graph.get_device("DawnDevice");
		assert(dawnDevice != nullptr && "ERROR: Failed to create Dawn device!");
		deviceGroup.emplace(dawnDevice);

		deviceConfig.name = "VulkanDevice";
		//render_graph.emplace_device(new vk::RenderDevice(deviceConfig));
		//IRenderDevice* vulaknDevice = render_graph.get_device("VulkanDevice");
		//assert(vulaknDevice != nullptr && "ERROR: Failed to create Vulkan device!");
		//deviceGroup.emplace(vulaknDevice);

		// Create Swap Chains.
		deviceGroup.create_swap_chain(swapChain, SwapChainDesc(EPresentMode::Mailbox, mainWindow, 3));
		// Init RenderPipeline Desc
		RenderPipelineDesc pipelineDesc = RenderPipelineDesc(
			ShaderLayout({
				// Create Actual ShaderResources on Device.
				deviceGroup.create_shader(vertexShader, ShaderDesc("VertexShader", "main", EShaderFrequency::VertexShader, vertexShaderSpirv)),
				deviceGroup.create_shader(pixelShader, ShaderDesc("PiexelShader", "main", EShaderFrequency::PixelShader, pixelShaderSpirv))
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
						BindingLayout::Slot(0, BindingLayout::UniformBuffer, EShaderFrequency::VertexShader),
					}),
					BindingLayout::Set(
					{
						BindingLayout::Slot(0, BindingLayout::UniformBuffer, EShaderFrequency::VertexShader),
					}),
				}),
			AttachmentLayout(
#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
		{ AttachmentLayout::Slot(ETextureFormat::R8G8B8A8_UNORM) }
#else
		{ AttachmentLayout::Slot(ETextureFormat::B8G8R8A8_UNORM) }
#endif
		), ECullMode::Back, EPrimitiveTopology::TriangleList, EPolygonMode::FILL, 1, 0xFFFFFFFF
		);

		// Create Render pipeline.
		deviceGroup.create_render_pipeline(renderPipeline, pipelineDesc);
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
		deviceGroup.create_buffer(uniformBuffer,
			BufferDesc(EBufferUsage::UniformBuffer, sizeof(sakura::float4x4), &viewProj));
		deviceGroup.create_buffer(uniformBufferPerTarget,
				BufferDesc(EBufferUsage::UniformBuffer, sizeof(sakura::float4x4) * targetWorlds.size(), targetWorlds.data()));
		deviceGroup.create_buffer(uniformBufferPerObject,
			BufferDesc(EBufferUsage::UniformBuffer, sizeof(sakura::float4x4) * worlds.size(), worlds.data()));

		deviceGroup.create_buffer(vertexBuffer,
			BufferDesc(EBufferUsage::VertexBuffer, sizeof(vertData), vertData));
		deviceGroup.create_buffer(indexBuffer,
			BufferDesc(EBufferUsage::IndexBuffer, sizeof(indxData), &indxData));


		deviceGroup.create_buffer(vertexBufferSphere,
			BufferDesc(EBufferUsage::VertexBuffer, sizeof(sphere_vertices), sphere_vertices));
		deviceGroup.create_buffer(indexBufferSphere,
			BufferDesc(EBufferUsage::IndexBuffer, sizeof(sphere_indices), &sphere_indices));

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
		using namespace ecs;
		ZoneScopedN("CollectAndUpload");
		auto Collect = [&ppl](filters& filter, std::vector<sakura::float4x4>& world, int maxSlice = 500)
		{

			static constexpr auto paramList = boost::hana::make_tuple(
				// read.
				param<const LocalToWorld>
			);
			auto pass = ppl.create_pass(filter, paramList);
			world.resize(pass->entityCount * 4);
			task_system::ecs::schedule(ppl, pass,
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
			passes[0] = Collect(filter, targetWorlds);
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
		deviceGroup.present(swapChain);
	}

	void PrepareCommandBuffer(RenderCommandBuffer& buffer)
	{
		//task_system::schedule(
		//	[ev2, &buffer]() {
				ZoneScopedN("PrepareCommandBuffer");

		//		defer(ev2.signal());

				RenderPass* pass_ptr = render_graph.render_pass(pass);

				pass_ptr->construct(render_graph.builder(pass));
				buffer.reset();
				pass_ptr->execute(buffer, render_graph, deviceGroup);
		//	});
	}

	void RenderAndPresent(const RenderCommandBuffer& buffer)
	{
		//task_system::schedule(
		//	[ev, evc, &buffer]() {
				{
					ZoneScopedN("Upload");
					sakura::float4x4 offset = math::make_transform(sakura::Vector3f(0, 0, -1.f) * 500);
					sakura::float4x4 view = sakura::math::look_at_matrix(
						sakura::Vector3f(0, 0, -1.f) * 1, Vector3f::vector_zero());
					sakura::float4x4 proj =
						sakura::math::perspective_fov(0.25f * 3.1415926f * 2, 1080.f / 1920.f, 1.0f, 1000.0f);

					viewProj = sakura::math::multiply(offset, view);
					viewProj = sakura::math::multiply(viewProj, proj);
					viewProj = sakura::math::transpose(viewProj);

					deviceGroup.update_buffer(uniformBuffer, 0, &viewProj, sizeof(viewProj));
					deviceGroup.update_buffer(
						uniformBufferPerObject, 0, worlds.data(), sizeof(float4x4) * worlds.size());
					deviceGroup.update_buffer(
						uniformBufferPerTarget, 0, targetWorlds.data(), sizeof(float4x4) * targetWorlds.size());
				}

				//evc.wait();
				//defer(ev.signal());

				{
					ZoneScopedN("RenderAndPresent");
					RenderPass* pass_ptr = render_graph.render_pass(pass);
					//task_system::blocking_call([&]()
					//	{
					deviceGroup.execute(buffer, pass_ptr->handle());
					//	});
				}
		//	}
		//);
	}
}	