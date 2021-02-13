#pragma once
#include "ShaderCompiler/ShaderCompiler.h"

#include "RenderGraph/RenderGraph.h"
#include "RenderGraphWebGPU/RenderGraphWebGPU.h"

#include "ECS/ECS.h"
#include "Boids.h"

#define TARGET_NUM 120000
int BoidCount = 0;
int LeaderCount = 0;

namespace render_system
{
	using namespace sakura;
	using namespace sakura::graphics;

	inline sakura::vector<std::byte> vertex_shader_spirv;
	inline sakura::vector<std::byte> pixel_shader_spirv;
	const sakura::string vertex_shader_hlsl = 
		u8R"(struct VertexIn
		{
			float3 aPos : SV_Position;
			float3 aCol : COLOR;
		};
		struct VertexOut
		{
			float4 position : SV_Position;
			float3 vCol : COLOR;
		};
		[[vk::binding(0, 1)]] cbuffer ub
		{
			float4x4 view_proj;
		};
		struct Wrds
		{
			float4x4 world;
			float4x4 world1;
			float4x4 world2;
			float4x4 world3;
		};
		[[vk::binding(0, 0)]] ConstantBuffer<Wrds> worlds[];
		VertexOut main(VertexIn vin, uint iID : SV_InstanceID)
		{
			VertexOut vout;
			float4 posW = mul(float4(vin.aPos, 1.0f), worlds[0].world);
			vout.position = mul(posW, view_proj);
			vout.vCol = vin.aCol;
			return vout;
		})";
	const sakura::string pixel_shader_hlsl =
		u8R"(struct VertexOut
		{
			float4 position : SV_Position;
			float3 vCol : COLOR;
		};
		float4 main(VertexOut pin) : SV_TARGET
		{
			return float4(pin.vCol.xyz, 1.f);
		})";


	const sakura::string vertex_shader_wgsl =
		u8R"(
		[[location(0)]] var<in> positionIn: vec3<f32>;
		[[location(1)]] var<in> colorIn: vec3<f32>; 
		[[builtin(instance_idx)]] var<in> InstanceIdx: u32;
		
		[[location(0)]] var<out> v_color: vec4<f32>; 
		[[location(1)]] var<out> v_normal: vec4<f32>; 
		[[location(2)]] var<out> v_position: vec4<f32>; 

		[[builtin(position)]] var<out> Position: vec4<f32>;
		
		[[block]] struct PassCB 
		{
			[[offset(0)]] view_proj: mat4x4<f32>;
			[[offset(64)]] lightdir: vec4<f32>;
			[[offset(80)]] lightcolor : vec4<f32>;
			[[offset(96)]] eyepos : vec4<f32>;
			[[offset(112)]] ambient : vec4<f32>;
		};
		[[block]] struct PositionsBuffer 
		{
			[[offset(0)]] world: [[stride(64)]] array<mat4x4<f32>>;
		}; 
		[[block]] struct IndicesBuffer 
		{
			[[offset(0)]] id: [[stride(4)]] array<u32>;
		}; 
		[[set(0), binding(0)]] var<storage_buffer> worlds: [[access(read)]] PositionsBuffer;
		[[set(0), binding(1)]] var<storage_buffer> ids : [[access(read)]] IndicesBuffer;
		[[set(1), binding(0)]] var<uniform> passCB: PassCB;
		
		fn rand(p0 : u32, p1 : u32) -> f32 {
			var p : vec2<f32> = vec2<f32>(f32(p0), f32(p1));
			var K1 : vec2<f32> = vec2<f32>(23.14069263277926, 2.665144142690225);
			return fract(cos(dot(p,K1)) * 12345.6789);
		}

		fn temperature(p0 : u32) -> vec4<f32> {
			var p : f32 = f32(p0)/8.0;
			return vec4<f32>((0.2+0.8*p),0.2,(-0.2*p+0.4),1.0);
		}
		
		[[stage(vertex)]]
		fn main() -> void {
			var posIn : vec4<f32> = vec4<f32>(positionIn.x, positionIn.y, positionIn.z, 1.0);
			var world : mat4x4<f32> = worlds.world[InstanceIdx];
			var worldPos : vec4<f32> = posIn * world;
			v_position = vec4<f32>(worldPos.x, worldPos.y, worldPos.z, 1.0);
			Position = worldPos * passCB.view_proj;
			var i : u32 = ids.id[InstanceIdx];
			v_color = vec4<f32>(rand(i, i + 3), rand(i, i + 2), rand(i, i + 1), 1.0);
			const normal : vec4<f32> = vec4<f32>(0.0,1.0,0.0,0.0);
			v_normal = normal * world;
			return;
		})";
	//v_color = temperature(i);

	const sakura::string pixel_shader_wgsl =
		u8R"(
		[[location(0)]] var<in> v_color: vec4<f32>;
		[[location(1)]] var<in> v_normal: vec4<f32>;
		[[location(2)]] var<in> v_position: vec4<f32>;
		[[location(0)]] var<out> outColor: vec4<f32>;
		[[builtin(front_facing)]] var<in> is_front : bool;

		[[block]] struct PassCB 
		{
			[[offset(0)]] view_proj: mat4x4<f32>;
			[[offset(64)]] lightdir: vec4<f32>;
			[[offset(80)]] lightcolor : vec4<f32>;
			[[offset(96)]] eyepos : vec4<f32>;
			[[offset(112)]] ambient : vec4<f32>;
		};
		[[set(1), binding(0)]] var<uniform> passCB: PassCB;

		[[stage(fragment)]]
		fn main() -> void {
			var normal : vec4<f32> = v_normal;
			if(!is_front) { normal = -normal; }
			var V : vec4<f32> = normalize(v_position);
			var H : vec4<f32> = normalize(passCB.lightdir+V);
			var diffuse : vec4<f32> = max(dot(normal, passCB.lightdir), 0.0)*passCB.lightcolor*v_color;
			var specular : vec4<f32> = pow(max(dot(H, normal), 0.0), 2.0)*passCB.lightcolor;
			var finalColor : vec4<f32> = diffuse + specular + passCB.ambient*v_color*passCB.ambient.w;
			outColor = vec4<f32>(finalColor.x, finalColor.y, finalColor.z, 1.0);
			return;
		})";

	static sakura::Window main_window;
	static RenderGraph render_graph;
	static IRenderDevice* render_device = nullptr;
	static SwapChainHandle swap_chain = render_graph.SwapChain("DefaultSwapChain");
	static GpuShaderHandle vertex_shader = render_graph.GpuShaderUnsafe("VertexShader");
	static GpuShaderHandle pixel_shader = render_graph.GpuShaderUnsafe("PixelShader");
	static RenderPipelineHandle render_pipeline = render_graph.RenderPipeline("TrianglePipeline");

	static GpuBufferHandle uniform_buffer = render_graph.GpuBuffer("UniformBuffer");
	static GpuBufferHandle uniform_buffer_per_object = render_graph.GpuBuffer("UniformBufferPerObject");
	static GpuBufferHandle id_buffer_per_object = render_graph.GpuBuffer("IdBufferPerObject");
	static GpuBufferHandle uniform_buffer_per_target = render_graph.GpuBuffer("UniformBufferPerTarget");

	static GpuBufferHandle vertex_buffer = render_graph.GpuBuffer("VertexBuffer");
	static GpuBufferHandle index_buffer = render_graph.GpuBuffer("IndexBuffer");

	static GpuBufferHandle vertex_buffer_sphere = render_graph.GpuBuffer("VertexBufferSphere");
	static GpuBufferHandle index_buffer_sphere = render_graph.GpuBuffer("IndexBufferSphere");

	static GpuTextureHandle boids_depth_stencil = render_graph.GpuTexture("BoidsDepthStencil");

	struct PassCB
	{
		sakura::float4x4 view_proj;
		float lightdir[4] = {-1.f, -1.f, 0.f, 0.f};
		float lightcolor[4] = { 80.f / 255.f, 88.f / 255.f, 115.f / 255.f, 255.f / 255.f };
		float eyepos[4] = {0.f, 0.f, 0.f, 1.f};
		float ambient[4] = { 211.f / 255.f, 199.f / 255.f, 255.f / 255.f, 150.f / 255.f };
	};
	static PassCB passCB = {};
	static std::vector<sakura::float4x4> worlds(TARGET_NUM);
	static std::vector<uint32_t> ids(TARGET_NUM);
	static std::vector<sakura::float4x4> target_worlds(10);

	class RenderPassSimple : public RenderPass
	{
	public:
		RenderPassSimple(const RenderPassHandle handle, sakura::uint8 cycleCount = 3)
			:RenderPass(handle, cycleCount) {}
		const RenderCommandBuffer& execute(
			RenderCommandBuffer& command_buffer,
			const RenderGraph& rg, IRenderDevice& device) noexcept override
		{
			command_buffer.enqueue<RenderCommandBeginRenderPass>(
				render_pipeline, attachment, ds);
			{
				command_buffer.enqueue<RenderCommandUpdateBinding>(Binding(0, 0, uniform_buffer_per_target, 0, sizeof(sakura::float4x4) * 10, 0));
				command_buffer.enqueue<RenderCommandUpdateBinding>(Binding(0, 1, id_buffer_per_object, 0, sizeof(uint32_t) * TARGET_NUM, 0));
				command_buffer.enqueue<RenderCommandUpdateBinding>(Binding(1, 0, uniform_buffer, 0, sizeof(PassCB), 0));
				command_buffer.enqueue<RenderCommandSetVB>(0, rg.query<GpuBufferHandle>("VertexBufferSphere"));
				command_buffer.enqueue<RenderCommandSetIB>(rg.query<GpuBufferHandle>("IndexBufferSphere"), EIndexFormat::UINT16);
				{
					command_buffer.enqueue<RenderCommandDraw>(60, LeaderCount);
				}
			}
			
			{
				command_buffer.enqueue<RenderCommandUpdateBinding>(
					Binding(0, 0, uniform_buffer_per_object, 0, sizeof(sakura::float4x4) * TARGET_NUM, 0));
				command_buffer.enqueue<RenderCommandUpdateBinding>(Binding(1, 0, uniform_buffer, 0, sizeof(PassCB), 0));
				command_buffer.enqueue<RenderCommandSetVB>(0, rg.query<GpuBufferHandle>("VertexBuffer"));
				// Mention that you can also do like this.
				// command_buffer.enqueue<RenderCommandSetVB>(1, rg.query<GpuBufferHandle>("VertexBuffer1"));
				command_buffer.enqueue<RenderCommandSetIB>(rg.query<GpuBufferHandle>("IndexBuffer"), EIndexFormat::UINT16);

				{
					ZoneScopedN("RenderPassExecute");
					{
						command_buffer.enqueue<RenderCommandDraw>(3, BoidCount);
					}
				}
			}
			command_buffer.enqueue<RenderCommandEndRenderPass>();
			return command_buffer;
		}
		bool construct(RenderGraph::Builder& builder, IRenderDevice& device) noexcept override
		{
			builder.pipeline(render_pipeline);
			attachment = Attachment({
				Attachment::Slot(swap_chain, sakura::double4(), ELoadOp::Clear, EStoreOp::Store)
			});
			builder.write(attachment);
			ds.clear_depth = 0.f;
			ds.ds_attachment = boids_depth_stencil;
			//builder.write(ds);
			return builder.apply();
		}
		Attachment attachment;
		DepthStencil ds;
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
		{
			deviceConfig.name = "DawnDevice";
			render_device = new webgpu::RenderDevice(render_graph, deviceConfig);
			assert(render_device != nullptr && "ERROR: Failed to create Dawn device!");
		}

		// Create Swap Chains.
		render_device->create_swap_chain(swap_chain, SwapChainDescriptor(EPresentMode::Mailbox, main_window, 3/*buffer_count*/, 1/*sample_count*/));
		auto chain_ptr = render_device->get<ISwapChain>(swap_chain);
		// Init RenderPipeline Desc
		RenderPipelineDescriptor pipelineDesc = RenderPipelineDescriptor(
			ShaderLayout({
				// Create Actual ShaderResources on Device.
				render_device->create_shader(vertex_shader, ShaderDescriptor("VertexShader",
					"main", EShaderFrequency::VertexShader, vertex_shader_wgsl.c_str(), EShaderLanguage::WGSL)),
				render_device->create_shader(pixel_shader, ShaderDescriptor("PiexelShader",
					"main", EShaderFrequency::PixelShader, pixel_shader_wgsl.c_str(), EShaderLanguage::WGSL))
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
						BindingLayout::Slot(0, BindingLayout::ReadonlyStorageBuffer, EShaderFrequency::VertexShader),
						BindingLayout::Slot(1, BindingLayout::ReadonlyStorageBuffer, EShaderFrequency::VertexShader),
					}),
					BindingLayout::Set(
					{
						BindingLayout::Slot(0, BindingLayout::UniformBuffer, EShaderFrequency::VertexShader | EShaderFrequency::PixelShader),
					})
				}),
			AttachmentLayout(
				{ AttachmentLayout::Slot(render_device->get<ISwapChain>(swap_chain)->render_format(), ELoadOp::Clear, EStoreOp::Store) }
			),
			DepthStencilDescriptor(ETextureFormat::D32_FLOAT, true, ECompareFunction::Greater),
			ECullMode::None, EPrimitiveTopology::TriangleList, EPolygonMode::FILL, 1, 0xFFFFFFFF
		);

		// Create Render pipeline.
		render_device->create_render_pipeline(render_pipeline, pipelineDesc);
		// create the buffers (x, y, r, g, b)
		float const vertData[] = {
			 -4.f, 0.f, -4.f,    0.f / 256.f, 49.f / 256.f, 79.f / 256.f, // BL
			 0.f, 0.f, 10.f,    137.f / 256.f, 190.f / 256.f, 178.f / 256.f, // top
			 4.f, 0.f, -4.f,     0.f / 256.f, 49.f / 256.f, 79.f / 256.f, // BR
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
		// Create Depth-Stencil
		TextureDescriptor texDesc = {};
		texDesc.size = { chain_ptr->extent().width, chain_ptr->extent().height, 1 };
		texDesc.dimension = ETextureDimension::Texture2D;
		texDesc.format = ETextureFormat::D32_FLOAT;
		texDesc.mip_levels = 1;
		texDesc.sharing_mode = ESharingMode::Exclusive;
		texDesc.sample_count = 1;
		texDesc.array_layers = 1;
		texDesc.usages = ETextureUsage::Attachment;
		render_device->create_texture(boids_depth_stencil, texDesc);
		// Create Buffers.
		render_device->create_buffer(uniform_buffer,
			BufferDescriptor(EBufferUsage::UniformBuffer | EBufferUsage::CopyDst, sizeof(passCB), &passCB));
		render_device->create_buffer(uniform_buffer_per_target,
			BufferDescriptor(EBufferUsage::StorageBuffer | EBufferUsage::CopyDst, sizeof(sakura::float4x4) * target_worlds.size(), target_worlds.data()));
		render_device->create_buffer(uniform_buffer_per_object,
			BufferDescriptor(EBufferUsage::StorageBuffer | EBufferUsage::CopyDst, sizeof(sakura::float4x4) * worlds.size(), worlds.data()));
		render_device->create_buffer(id_buffer_per_object,
			BufferDescriptor(EBufferUsage::StorageBuffer | EBufferUsage::CopyDst, sizeof(uint32_t)* ids.size(), ids.data()));

		render_device->create_buffer(vertex_buffer,
			BufferDescriptor(EBufferUsage::VertexBuffer | EBufferUsage::CopyDst, sizeof(vertData), vertData));
		render_device->create_buffer(index_buffer,
			BufferDescriptor(EBufferUsage::IndexBuffer | EBufferUsage::CopyDst, sizeof(indxData), &indxData));


		render_device->create_buffer(vertex_buffer_sphere,
			BufferDescriptor(EBufferUsage::VertexBuffer | EBufferUsage::CopyDst, sizeof(sphere_vertices), sphere_vertices));
		render_device->create_buffer(index_buffer_sphere,
			BufferDescriptor(EBufferUsage::IndexBuffer | EBufferUsage::CopyDst, sizeof(sphere_indices), &sphere_indices));

		sakura::info("All Tests Passed!");
	}

	namespace task_system = sakura::task_system;
	namespace math = sakura::math;
	using Vector3f = sakura::Vector3f;
	using Quaternion = sakura::Quaternion;
	using Rotator = sakura::Rotator;
	using float4x4 = sakura::float4x4;
	using IModule = sakura::IModule;
	enum EDebugMode
	{
		Debug_NeighborCount,
		Debug_FollowingLeader,
		Debug_UniqueColor,
	} DebugMode = Debug_FollowingLeader;
	
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
			auto pass = ppl.create_pass(filter, paramList, PassLocation(Collect));
			task_system::ecs::schedule_init(ppl, pass,
				[&world](const task_system::ecs::pass& task_pass)
				{
					world.resize(task_pass.calc_size());
				},
				[&world](const task_system::ecs::pass& task_pass, const ecs::task& tk)
				{
					auto o = operation{ paramList, task_pass, tk };
					auto index = o.get_index();
					const float4x4* l2ws = o.get_parameter<const LocalToWorld>();

					Vector3f pos = Vector3f::vector_zero();
					for (auto i = 0u; i < o.get_count(); i++)
					{
						auto l2w = sakura::math::transpose(l2ws[i]);
						int j = (i + index);
						world[j] = l2w;
					}
				}, maxSlice);
			return pass;
		};
		std::shared_ptr<task_system::ecs::pass> passes[3];
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
		auto CollectId = [&](auto&& f)
		{

			filters filter;
			filter.archetypeFilter = {
				{complist<LocalToWorld, Boid>}, //all
				{}, //any
				{} //none
			};
			static constexpr auto paramList = boost::hana::make_tuple(
				// read.
				param<const BoidDebugData>
			);
			auto pass = ppl.create_pass(filter, paramList, PassLocation(Collect));
			task_system::ecs::schedule_init(ppl, pass,
				[](const task_system::ecs::pass& task_pass)
				{
					ids.resize(task_pass.calc_size());
				},
				[f](const task_system::ecs::pass& task_pass, const ecs::task& tk)
				{
					auto o = operation{ paramList, task_pass, tk };
					auto index = o.get_index();
					auto fls = o.get_parameter<const BoidDebugData>();

					Vector3f pos = Vector3f::vector_zero();
					for (auto i = 0u; i < o.get_count(); i++)
					{
						int j = (i + index);
						f(j, fls[i]);
						//ids[j] = fls[i].following.id;
						//ids[j] = fls[i].neighborCount;
					}
				}, 500);
			return pass;
		};
		{
			if (DebugMode == Debug_FollowingLeader)
				passes[2] = CollectId([](uint32_t j, const BoidDebugData& fls) {ids[j] = fls.following.id; });
			else if (DebugMode == Debug_NeighborCount)
				passes[2] = CollectId([](uint32_t j, const BoidDebugData& fls) {ids[j] = fls.neighborCount; });
			else if (DebugMode == Debug_UniqueColor)
				passes[2] = CollectId([](uint32_t j, const BoidDebugData& fls) {ids[j] = j; });
		}
		passes[0]->event.wait();
		passes[1]->event.wait();
		passes[2]->event.wait();
	}

	void Present()
	{
		ZoneScopedN("Present");

		render_device->present(swap_chain);
	}

	void PrepareCommandBuffer(RenderCommandBuffer& buffer)
	{
		ZoneScopedN("PrepareCommandBuffer");

		RenderPass* pass_ptr = render_graph.render_pass(pass);

		pass_ptr->construct(render_graph.builder(pass), *render_device);
		buffer.reset();
		pass_ptr->execute(buffer, render_graph, *render_device);
	}

	Vector3f CameraPos;
	float Yaw, Pitch, Roll;

	sakura::Vector3f RotateVector(sakura::Quaternion quat, sakura::Vector3f V)
	{
		// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
		// V' = V + 2w(Q x V) + (2Q x (Q x V))
		// refactor:
		// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
		// T = 2(Q x V);
		// V' = V + w*(T) + (Q x T)
		auto d = quat.data_view();
		const sakura::Vector3f Q(d[0], d[1], d[2]);
		const sakura::Vector3f T = 2.f * sakura::Vector3f::cross_product(Q, V);
		const sakura::Vector3f Result = V + (d[3] * T) + sakura::Vector3f::cross_product(Q, T);
		return Result;
	}

	sakura::Vector3f GetForwardVector()
	{
		constexpr sakura::Vector3f Forward = sakura::Vector3f(0.f, 0.f, 1.f);
		auto quat = math::quaternion_from_euler(Pitch, Yaw, Roll);
		return RotateVector(quat, Forward);
	}

	sakura::Vector3f GetRightVector()
	{
		constexpr sakura::Vector3f Forward = sakura::Vector3f(1.f, 0.f, 0.f);
		auto quat = math::quaternion_from_euler(Pitch, Yaw, Roll);
		return RotateVector(quat, Forward);
	}

	void Render(const RenderCommandBuffer& buffer)
	{
		{
			ZoneScopedN("Upload");
			sakura::float4x4 view = sakura::math::look_at_matrix(CameraPos, CameraPos + GetForwardVector() * 500.f);
			sakura::float4x4 proj = sakura::math::perspective_fov(0.25f * 3.1415926f * 2, 1080.f / 1920.f, 1.0f, 1000.0f);
			proj.M[2][3] *= -1;

			auto& view_proj = passCB.view_proj;
			view_proj = sakura::math::multiply(view, proj);
			view_proj = sakura::math::transpose(view_proj);

			render_device->update_buffer(uniform_buffer, 0, &passCB, sizeof(passCB));
			render_device->update_buffer(
				uniform_buffer_per_object, 0, worlds.data(), sizeof(float4x4) * worlds.size());
			render_device->update_buffer(
				id_buffer_per_object, 0, ids.data(), sizeof(uint32_t) * ids.size());
			render_device->update_buffer(uniform_buffer_per_target,
				0, target_worlds.data(), sizeof(float4x4) * target_worlds.size());
		}

		{
			ZoneScopedN("Render");
			RenderPass* pass_ptr = render_graph.render_pass(pass);
			render_device->execute(buffer, pass_ptr->handle());
		}
	}
}	