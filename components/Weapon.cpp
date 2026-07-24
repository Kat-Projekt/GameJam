#define DIAGNOSTIC
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
		DEBUG ( 5, "father? ", obj->Get_Father ( )->Has_Component ( "Runner" ) );
		return !obj->Get_Father ( )->Has_Component ( "Runner" );
	}

	Weapon* Pick ( )
	{
		obj->Set_Pos ( {0,0,0} );
		obj->Set_Rot_Pivot ( {1,0,0} );
		obj->Set_2D_Rot ( M_PI/3 );

		return this;
	}

	virtual void Swing ( ) { }

	// returns the number of left usages
	// when it returns 0 it is detached
	virtual int Throw ( ) { return 0; }
};