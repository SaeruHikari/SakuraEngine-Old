# Core
Core of the Engine with no status(which means it's available for both static and shared lib building).
  * CoreMinimal:
  Pure headers. Defination and keywords of the Engine. General Collected in: [[CoreMinimal.h]](CoreMinimal/CoreMinimal.h)
  * EngineUtils:
  Engine Utils with the implementation of some cross-platform OS operations. With some .cc and .cpp files but no status in it.
  * Math:
  Pure headers. Based on Eigen3.
  * CoreModule:
  An export of Core for SPA module. Export as a static lib as default.
  
