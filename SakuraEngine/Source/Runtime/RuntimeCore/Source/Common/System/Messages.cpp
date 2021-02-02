#include "System/Messages.h"

namespace sakura
{
	OSMessages::OSMessages(const Window window)
		: kb_subscriber_(kb_subject_.get_subscriber()), os_subscriber_(os_subject_.get_subscriber()), window_(window)
	{

	}

	OSMessages::~OSMessages()
	{
		kb_subscriber_.on_completed();
		os_subscriber_.on_completed();
	}
}