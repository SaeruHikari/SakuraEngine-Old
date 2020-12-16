#pragma once
#include "ShaderCompiler/ShaderCompiler.h"

#include "RenderGraph/RenderGraph.h"
#include "RenderGraphWebGPU/RenderGraphWebGPU.h"

#include "ECS/ECS.h"

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
		[[vk::binding(0, 0)]] cbuffer ub\
		{\
			float4x4 viewProj;\
		};\
		[[vk::binding(1, 0)]] cbuffer ub2\
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

	RenderBufferHandle vertexBuffer = render_graph.RenderBuffer("VertexBuffer");
	RenderBufferHandle indexBuffer = render_graph.RenderBuffer("IndexBuffer");	

	RenderBufferHandle vertexBufferSphere = render_graph.RenderBuffer("VertexBufferSphere");
	RenderBufferHandle indexBufferSphere = render_graph.RenderBuffer("IndexBufferSphere");

	sakura::float4x4 viewProj;
	std::vector<sakura::float4x4> worlds(42000 * 4);

	class RenderPassSimple : public RenderPass
	{
	public:
		RenderPassSimple(const RenderPassHandle handle,
			sakura::uint8 cycleCount = 3, size_t bufferSize = 4096 * 16 * 8)
			:RenderPass(handle, cycleCount, bufferSize) {}
		bool execute(const RenderGraph& rg, const RenderGraph::Builder& builder, IRenderDevice& device) noexcept override
		{
			command_buffer().enqueue<RenderCommandBeginRenderPass>(renderPipeline, attachment);
			for (auto i = 0u; i < worlds.size() / 4; i++)
			{
				Binding binding = Binding({
					Binding::Set({
						Binding::Slot(uniformBuffer, 0, sizeof(sakura::float4x4), 0),
						Binding::Slot(uniformBufferPerObject, 1,
							sizeof(sakura::float4x4) * 4, sizeof(sakura::float4x4) * i * 4)
					})
				});
				command_buffer().enqueue<RenderCommandUpdateBinding>(binding);
				command_buffer().enqueue<RenderCommandDraw>(
					RenderCommandDraw::VB(rg.blackboard<RenderBufferHandle>("VertexBufferSphere")),
					RenderCommandDraw::IB(rg.blackboard<RenderBufferHandle>("IndexBufferSphere"), 
					60, EIndexFormat::UINT16)
				);
			}
			command_buffer().enqueue<RenderCommandEndRenderPass>();
			return device.execute(*this, handle()) && this->reset();
		}
		bool construct(RenderGraph::Builder& rg) noexcept override
		{
			sakura::float4x4 offset = math::make_transform(sakura::Vector3f(-1.f, -1.f, -1.f) * 400);
			sakura::float4x4 view = sakura::math::look_at_matrix(
				sakura::Vector3f(-1.f, -1.f, -1.f) * 1, Vector3f::vector_zero());
			sakura::float4x4 proj =
				sakura::math::perspective_fov(0.25f * 3.1415926f, 720.f / 1080.f, 1.0f, 1000.0f);

			viewProj = sakura::math::multiply(offset, view);
			viewProj = sakura::math::multiply(viewProj, proj);
			viewProj = sakura::math::transpose(viewProj);

			deviceGroup.update_buffer(uniformBuffer, 0, &viewProj, sizeof(viewProj));

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
		windDesc.height = 720;
		windDesc.width = 1080;
		windDesc.name = "Sakura Engine";
		mainWindow = sakura::Window::create(windDesc);

		// Create Devices
		DeviceConfiguration deviceConfig;
		deviceConfig.name = "DawnDevice";
		render_graph.emplace_device(new webgpu::RenderDevice(deviceConfig));
		IRenderDevice* dawnDevice = render_graph.get_device("DawnDevice");
		assert(dawnDevice != nullptr && "ERROR: Failed to create Dawn device!");
		deviceGroup.emplace(dawnDevice);

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
						BindingLayout::Slot(1, BindingLayout::UniformBuffer, EShaderFrequency::VertexShader)
					})
				}),
			AttachmentLayout(
#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
		{ AttachmentLayout::Slot(ETextureFormat::R8G8B8A8_UNORM) }
#else
		{ AttachmentLayout::Slot(ETextureFormat::B8G8R8A8_UNORM) }
#endif
		), EPrimitiveTopology::TriangleList, 1, 0xFFFFFFFF
		);

		// Create Render pipeline.
		deviceGroup.create_render_pipeline(renderPipeline, pipelineDesc);
		// create the buffers (x, y, r, g, b)
		float const vertData[] = {
			 0.f, -4.f, -4.f,    1.0f, 0.0f, 1.0f, // BL
			 0.f, -4.f, 4.f,    0.0f, 1.0f, 0.0f, // BR
			 0.0f, 10.f, 0.f,    1.0f, 0.0f, 0.0f, // top
		}; // triangle on XY-Plane.
		uint16_t const indxData[] = { 0, 1, 2, 0 };

		const float X = 0.525731f * 7.f;
		const float Z = 0.850651f * 7.f;
		float const sphere_vertices[72] =
		{
			-X, 0.0f, Z,  1.0f, 0.0f, 1.0f,
			X, 0.0f, Z,   0.0f, 1.0f, 0.0f,
			-X, 0.0f, -Z, 1.0f, 0.0f, 0.0f,
			X, 0.0f, -Z,  1.0f, 0.0f, 1.0f,
			0.0f, Z, X,   0.0f, 1.0f, 0.0f,
			0.0f, Z, -X,  1.0f, 0.0f, 0.0f,
			0.0f, -Z, X,  1.0f, 0.0f, 1.0f,
			0.0f, -Z, -X, 0.0f, 1.0f, 0.0f,
			Z, X, 0.0f,   1.0f, 0.0f, 0.0f,
			-Z, X, 0.0f,  1.0f, 0.0f, 1.0f,
			Z, -X, 0.0f,  0.0f, 1.0f, 0.0f,
			-Z, -X, 0.0f, 1.0f, 0.0f, 0.0f,
		};
		uint16_t sphere_indices[60] =
		{
			1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
			1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
			3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
			10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
		};
		// Create Buffers.
		deviceGroup.create_buffer(uniformBuffer,
			BufferDesc(EBufferUsage::UniformBuffer, sizeof(sakura::float4x4), &viewProj));
		deviceGroup.create_buffer(uniformBufferPerObject,
				BufferDesc(EBufferUsage::UniformBuffer, sizeof(sakura::float4x4) * 10500 * 4, worlds.data()));

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
	task_system::Event CollectSystem(task_system::ecs::pipeline& ppl, float deltaTime)
	{
		using namespace sakura;
		using namespace sakura::graphics;
		using namespace ecs;

		filters filter;
		filter.archetypeFilter = {
			{complist<LocalToWorld>}, //all
			{}, //any
			{} //none
		};
		static constexpr auto paramList = boost::hana::make_tuple(
			// read.
			param<const LocalToWorld>
		);
		static size_t cycles = 0;
		cycles += 1;
		if(cycles % 60 == 0)
			mainWindow.set_title(fmt::format(L"SakuraEngine: {:.2f} FPS", 1.0 / deltaTime).c_str());

		worlds.clear();
		return task_system::ecs::schedule<false, true>(ppl, *ppl.create_pass(filter, paramList),
			[](const task_system::ecs::pipeline& pipeline, const ecs::pass& task_pass, const ecs::task& tk)
			{
				auto o = operation{ paramList, task_pass, tk };
				const float4x4* l2ws = o.get_parameter<const LocalToWorld>();

				Vector3f pos = Vector3f::vector_zero();
				for(auto i = 0u; i < o.get_count(); i++)
				{
					worlds.emplace_back(sakura::math::transpose(l2ws[i]));
					worlds.emplace_back(sakura::math::transpose(l2ws[i]));
					worlds.emplace_back(sakura::math::transpose(l2ws[i]));
					worlds.emplace_back(sakura::math::transpose(l2ws[i]));
				}
			});
	}

	task_system::Event RenderAndPresent(const task_system::Event at)
	{
		at.wait();
		task_system::Event ev;
		task_system::schedule(
			[ev]() {
				defer(ev.signal());
				deviceGroup.update_buffer(
					uniformBufferPerObject, 0, worlds.data(), sizeof(float4x4) * worlds.size());

				RenderPass* pass_ptr = render_graph.render_pass(pass);
				pass_ptr->construct(render_graph.builder(pass));
				if (pass_ptr->execute(render_graph, render_graph.builder(pass), deviceGroup))
				{
					deviceGroup.present(swapChain);
				}
			}
		);
		return ev;
	}
}