#include <engine.hpp>

class Weapon : public Behaviour
{
private:
	/* data */
public:
	Weapon ( )
	{
		Informations = { "Weapon", 1.0, "This is the weapons information" };
	}

	// if another runner has it, it cannot be picked up
	bool IsPickable ( )
	{
		return obj->Get_Father ( )->Has_Component ( "Runner" );
	}

	virtual void Swing ( ) { }

	// returns the number of left usages
	// when it returns 0 it is detached
	virtual int Throw ( ) { return 0; }
};