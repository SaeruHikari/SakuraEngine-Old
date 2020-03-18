<!--
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:45
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-03-01 20:11:07
 -->
<h1 align="center">Sakura Engine</h1>
A Cross-Platform Mordern Game Engine.

  * Package Management
    * Conan √
  * Construct
    * CMake √
  * UnitTest
    * GTest √
  * PluginManager √
    * [SPA](SPA/)(SakuraPluginAdministrator): Plugin Administrator to manage plugins
      * Based on Python Batcher.
      * Load/Unload plugins in order described with json file, by Dependency Graph.
      * Dispatch and handle requesting from plugins/modules.
  * [Core](SakuraEngine/Core/) √ 
    * Platform Defination √(Win32/Linux)
    * EngineUtils(EnvQuery√, Timer√, T2StringCaster√, Path Operation√, Thread...)
    * Math √(Eigen3)
    * Efficient Streaming
    * Quik String √(pmr::string and caster)
  * MemoryManagement
    * Allocator 🚧
    * STL Container Wrappers √
  * Script
    * Mono?
    * Auto-Binding / Binding Machine
  * Graphics: [CGD](SakuraEngine/Graphicsinterface) 🚧
    * GraphicsUtils
      * GraphicsProfiling(GpuTimer...) 🚧
    * TargetGraphicsinterface 🚧
      * Vulkan 🚧
      * D3D12 🚧
      * Metal
    * CommonGraphicsDevice
      * Async-Compute Command Context
      * RayTracing Layer/Device
      * Sharp Text Renderer
      * 2D Renderer
    * GraphicsDispatcher
      * ResourceManagement
      * Easy Param Binding
      * Multi-Thread Dispatching
      * Data-Driven pipeline
    * RenderPipeline
      * PBR Shading Models
      * F+ Shading
      * Culling
      * Shadowing 
      * SSAO(GTAO/HBAO+)
      * RTAO
      * DDGI
      * Paricle Effect
      * ...
  * EngineDrivers
      * FrameGraph/RenderGraph
      * TaskFlow/JobSystem
  * Scene
      * Node-Based Scene √
      * Serialization
      * Flexible assembling 
  * Profiling
    * PIX
    


