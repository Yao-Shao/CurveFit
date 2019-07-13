
#include "../COMMON/etlbase.h"
#include "../ViewModel/ViewModel.h"
#define ERRORBOUND 1e-6

bool ViewModel_Test() throw() {

	try {
		std::shared_ptr<Model> model(std::make_shared<Model>());
		std::shared_ptr<ViewModel> vmdl(std::make_shared<ViewModel>());
		vmdl->SetModel(model);

		//case 1 y = 2x+3
		Param_opcf p;
		Type t = LINEAR_FUNCTION;
		Points sampleP;
		Point p0(1, 5);
		sampleP.push_back(p0);
		Point p1(-1, 1);
		sampleP.push_back(p1);
		p.set_type(LINEAR_FUNCTION);
		p.set_point(sampleP);
		vmdl->get_fitCommand()->SetParameter(p);
		vmdl->get_fitCommand()->Exec();
		if (abs((vmdl->getFunction())->get_y(10)) - 23 > ERRORBOUND)
			throw int(1);
		//case 2 y = x^2 + 4*x + 5
		Param_opcf param1;
		t = QUADRATIC_FUNCTION;
		sampleP.clear();
		Point pq0(0, 5);
		sampleP.push_back(pq0);
		Point pq1(1, 10);
		sampleP.push_back(pq1);
		Point pq2(-1, 2);
		sampleP.push_back(pq2);
		p.set_type(QUADRATIC_FUNCTION);
		p.set_point(sampleP);
		vmdl->get_fitCommand()->SetParameter(param1);
		vmdl->get_fitCommand()->Exec();
		if (abs((vmdl->getFunction())->get_y(10)) - 145 > ERRORBOUND)
			throw int(2);
		//case 3 y = x^3 + x^2 + x + 5
		Param_opcf param2;
		t = CUBIC_FUNCTION;
		sampleP.clear();
		Point pc0(0, 5);
		sampleP.push_back(pc0);
		Point pc1(1, 8);
		sampleP.push_back(pc1);
		Point pc2(-1, 4);
		sampleP.push_back(pc2);
		Point pc3(2, 19);
		sampleP.push_back(pc3);
		p.set_type(CUBIC_FUNCTION);
		p.set_point(sampleP);
		vmdl->get_fitCommand()->SetParameter(param2);
		vmdl->get_fitCommand()->Exec();
		if (abs((vmdl->getFunction())->get_y(10)) - 145 > ERRORBOUND)
			throw int(3);


	}
	catch (int& v) {
		printf("case %d error\n", v);
		return false;
	}
	catch (...) {
		printf("exception\n");
		return false;
	}
	return true;
}