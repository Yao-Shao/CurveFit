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
	void opcf_createFunction(Type t);

private:
	std::shared_ptr<Points> sp_points;
	std::shared_ptr<Function> sp_Function;
};

