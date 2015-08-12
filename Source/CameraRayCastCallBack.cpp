#include "CameraRayCastCallBack.h"

CameraRayCastCallBack::CameraRayCastCallBack()
{
}

bool CameraRayCastCallBack::ReportShape(q3Box * b)
{
	t_final = FLT_MAX;
	if(mData.toi < t_final)
	{
		t_final = mData.toi;
		impactPoint = mData.normal; 
		impactBody = b->body;
	}
	
	mData.toi = t_final;

	return true;
}

