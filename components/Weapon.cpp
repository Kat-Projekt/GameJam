#define DIAGNOSTIC
#pragma once
#include <engine.hpp>

class Weapon : public Behaviour
{
private:
	std::string weapon_name;
public:
	Weapon ( )
	{
		Informations = { "Weapon", 1.0, "This is the weapons information" };
		weapon_name  = "Empty";
	}

	// if another runner has it, it cannot be picked up
	bool IsPickable ( )
	{
		DEBUG ( 5, "father? ", obj->Get_Father ( )->Has_Component ( "Runner" ) );
		return !obj->Get_Father ( )->Has_Component ( "Runner" );
	}

	void Start ( )
	{
		Manager::Make < Animation < float > > ( "weapon_swing" + weapon_name, &(obj->Get_Transform ( ).Expose_2D_Rot ( )), PlayMode::ONCE );
		auto swing = Manager::Get < Animation < float > > ( "weapon_swing" + weapon_name );

		swing	->Add_Frame ( -M_PI_4, M_PI_4, 1 );

		obj	->Add_Component < Animator > ( )
				->New_Node ( "swing" )
				->Add_Animation ( "swing", swing );
	}

	Weapon* Pick ( )
	{
		obj->Set_Pos ( {0,0,0} );
		obj->Set_Rot_Pivot ( {1,0,0} );
		obj->Set_2D_Rot ( M_PI/3 );

		obj->Get_Component < Box_Collider > ( )->Set_Active ( false );

		return this;
	}

	virtual void Swing ( ) {
		obj->Get_Component < Box_Collider > ( )->Set_Active ( true );
		obj->Add_Component < Animator > ( )->Change_Animation ( "swing" );
	}

	// returns the number of left usages
	// when it returns 0 it is detached
	virtual int Throw ( ) { return 0; }
};