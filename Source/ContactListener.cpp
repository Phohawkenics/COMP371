#include "ContactListener.h"
#include "PhysicalModel.h"
#include <dynamics/q3Contact.h>

ContactListener::ContactListener()
{
}


ContactListener::~ContactListener()
{
}

void ContactListener::BeginContact(const q3ContactConstraint *contact){
	PhysicalModel * dataA = (PhysicalModel *)contact->A->GetUserdata();
	PhysicalModel * dataB = (PhysicalModel *)contact->B->GetUserdata();

	if (dataA) dataA->handleBeginContact(contact);
	if (dataB) dataB->handleBeginContact(contact);
}

void ContactListener::EndContact(const q3ContactConstraint *contact){	
	PhysicalModel * dataA = (PhysicalModel *)contact->A->GetUserdata();
	PhysicalModel * dataB = (PhysicalModel *)contact->B->GetUserdata();

	if (dataA) { dataA->handleEndContact(contact); };
	if (dataB) { dataB->handleEndContact(contact); };
}

