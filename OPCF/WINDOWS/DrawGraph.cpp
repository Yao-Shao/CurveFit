#include "DrawGraph.h"



DrawGraph::DrawGraph()
	:width(500),
	height(500)
{
}


DrawGraph::~DrawGraph()
{
}

void DrawGraph::setType(int t)
{
	fitType = t;
}
