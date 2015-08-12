#include "q3.h"

class CameraRayCastCallBack : public q3QueryCallback
{
public:
	friend class PhysicsCamera;

	CameraRayCastCallBack();

	q3RaycastData mData;
	
	q3Body *impactBody;
	q3Vec3 impactPoint;
	r32 t_final;
	
	bool ReportShape(q3Box * b);
};