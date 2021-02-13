# RenderGraph
RenderGraph旨在提供基本的GPU功能/资源接口以及自动化的GPU资源管理。在RenderGraph中，一切GPU资源甚至是对象都被映射到handle上。

## handle
分为ResourceId，ObjectId以及PassId。派生自[GenerationalId](RuntimeCore/GeneraionalId.md)