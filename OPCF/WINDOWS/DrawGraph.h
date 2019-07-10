#pragma once
#ifndef DRAW_GRAPH_H
#define DRAW_GRAPH_H

#include <QPixMap>

class DrawGraph
{
public:
	DrawGraph();
	~DrawGraph();
	void setType(int t);

private:
	int fitType;
	const int width;
	const int height;
	QPixmap* pix;
};

#endif
