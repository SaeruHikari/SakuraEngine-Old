# GenerationalId
包含代数的[ID结构](https://github.com/SaeruHikari/SakuraEngine/blob/master/SakuraEngine/Source/Runtime/RuntimeCore/Include/Base/GenerationalId.h)，用于中心化的资源管理(例如RenderGraph中的GPU资源)。

结构布局如下：

```cpp
namespace sakura
{
	struct GenerationalId
	{
		using uhalf = ____::uhalf<true>;
		using uhalf_t = uhalf::type;
	protected:
		union
		{
			size_t storage;
			struct view
			{
				uhalf_t generation;
				uhalf_t index;
			}v;
		}u;
	};
}
```
