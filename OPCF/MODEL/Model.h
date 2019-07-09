#pragma once

#include "../COMMON/base.h"
#include <memory>
#include "../COMMON/etlbase.h"

class Model : public Proxy_PropertyNotification<Model>, public Proxy_CommandNotification<Model>
{
public:
	Model();
	~Model() {}
	std::shared_ptr<Function> getFunction();

private:
	Points p_sets;
	std::shared_ptr<Function> sp_Fuction;
};

class model
{
};
