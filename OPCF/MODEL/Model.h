#pragma once

#include "../COMMON/base.h"
#include <memory>
#include "../COMMON/etlbase.h"
#include "../COMMON/param.h"

class Model : public Proxy_PropertyNotification<Model>, public Proxy_CommandNotification<Model>
{
public:
	Model();
	~Model() {}
	std::shared_ptr<Function> getFunction();
	bool opcf_fit(Param_opcf& p);

private:
	std::shared_ptr<Function> sp_Function;
};

