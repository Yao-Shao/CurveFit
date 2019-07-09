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
	void opcf_createFunction(Param_opcf p);
	void set_function(std::shared_ptr<Function> func);

private:
	std::shared_ptr<Function> sp_Function;
};

