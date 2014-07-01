#ifndef BATCONTEXT_H
#define BATCONTEXT_H

struct BatContext
{
	int frame_nums;
	int cur_frame_num;
	int rotateX;
	int rotateY;
	double currentX;
	double currentZ;
	bool wireframe;
	bool surface;
	bool symbols;
	bool camber[3];
	int scale;
};


#endif