#include "Error.h"

void ErrorCode::Clear()
{
	flag = 0;
	msg.clear();
}