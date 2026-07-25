#define DIAGNOSTIC
#pragma once
#include <engine.hpp>

class Weapon : public Behaviour
{
protected:
	std::string weapon_name;

	bool _in_flight = false;
	Objekt* _thrown_by = nullptr;

	float _throw_speed = 900.0f;
	float _flight_timer = 0.0f;
	const float _max_flight_time = 1.0f;
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
		if ( _in_flight ) { return false; }
		return !obj->Get_Father ( )->Has_Component ( "Runner" );
	}

	Weapon* Set_Throw_Speed ( float speed ) { _throw_speed = speed; return this; }
	float Get_Throw_Speed ( ) const { return _throw_speed; }
	bool Is_In_Flight ( ) const { return _in_flight; }
	Objekt* Get_Thrown_By ( ) const { return _thrown_by; }

	void Land ( )
	{
		_in_flight = false;
		_thrown_by = nullptr;
		if ( auto* rb = obj->Get_Component < Rigidbody > ( ) )
		{ rb->velocity = vec3{0,0,0}; }
		if ( auto* col = obj->Get_Component < Box_Collider > ( ) )
		{ col->Set_Active ( false ); }
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

	void Update ( ) override
	{
		if ( _in_flight )
		{
			_flight_timer -= Timer::Get_Delta ( );
			if ( _flight_timer <= 0.0f )
			{ Land ( ); }
		}
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
	virtual int Throw ( vec3 direction, float strength )
	{
		_thrown_by = obj->Get_Father ( ) ? obj->Get_Father ( ).get ( ) : nullptr;
		_in_flight = true;
		_flight_timer = _max_flight_time;

		auto* rb = obj->Get_Component < Rigidbody > ( );
		if ( !rb ) { rb = obj->Add_Component < Rigidbody > ( ); }

		vec3 dir = ( direction == vec3(0) ) ? vec3{1,0,0} : normalize ( direction );
		rb->velocity = dir * ( _throw_speed * strength );

		if ( auto* col = obj->Get_Component < Box_Collider > ( ) )
		{ col->Set_Active ( true ); }

		return 0;
	}
};