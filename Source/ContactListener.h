#pragma once
#include <q3.h>
class ContactListener :
	public q3ContactListener
{
public:
	ContactListener();

	virtual void BeginContact(const q3ContactConstraint *contact);
	virtual void EndContact(const q3ContactConstraint *contact);

	virtual ~ContactListener();
};

