#include "Model.h"


Model::Model()
{
	sp_Fuction = std::make_shared<Function>();
	sp_points = std::make_shared<Points>();
}

std::shared_ptr<Function> Model::getFunction()
{
	return std::shared_ptr<Function>();
}

void Model::opcf_createFunction(Type t)
{
	if (t == LINEAR_FUNCTION)
	{

	}
	else if (t == QUADRATIC_FUNCTION) {

	}
	else if(t == EXPONENTIAL_FUNCTION){

	}
	else if (t == LN_FUNCTION) {

	}
	else if (t == NORMAL_FUNCTION) {
	
	}
	else {

	}
	//告知其它模块，model里面的Function已经改变
	Fire_OnPropertyChanged("Fuction");
}
