#pragma once
#include "../_INCLUDES_.h"

class Provider
{
	private:
		static Provider* Instance;
		Provider();
	public:
		static Provider* GetInstance();
};