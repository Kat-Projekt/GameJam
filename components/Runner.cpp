#define DIAGNOSTIC

#include <engine.hpp>
#include "Weapon.cpp"

class Hands : public Weapon
{

};

class Runner : public Behaviour
{
private:
	float move_speed = 400.0f;
	float time = 30.0f;
	std::string displaid_time = "";

	Text* _text_text = nullptr;
	Rigidbody* _rigid = nullptr;

	Weapon* _weapon = nullptr;
	Objekt* _candidate_for_pick_up = nullptr;
	Weapon _hands;

	vec3 _target_direction = {0,0,0};
	vec3 _attack_point = {0,0,0};

	int _head_locked_by_setting_attack_direction = 0;
public:
	Runner ( )
	{
		Informations = { "Runner", 1.0, "The ai for the enemy runners" };
	}

	void Start ( ) override
	{
		_rigid = obj->Add_Component < Rigidbody > ( );
		obj->Add_Component < Box_Collider > ( )->Set_Size ( obj->Get_Size ( ) );

		_text_text = obj->Add_Component < Text > ( )
			->Set ( "Aovel", "" )
			->Set ( "", Text::ALIGNMENT::CENTER, Text::ALIGNMENT::CENTER )
			->Set ( vec4{1.0f,0.5f,0.5f,1.0f} );
	}

	void Update ( ) override
	{
		time -= Timer::Get_Delta ( );

		int value = static_cast<int>(time*100);

		std::string new_time = std::to_string(value);

		// Pad with leading zeros
		while ( new_time.length() < 4 )
		{ new_time = "0" + new_time; }

		// Insert ':'
		new_time.insert(2, ":");

		if ( new_time != displaid_time )
		{
			displaid_time = new_time;
			_text_text->Set ( displaid_time );
		}

		// animation
		if ( _head_locked_by_setting_attack_direction > 0 )
		{ _head_locked_by_setting_attack_direction --; } // for unlocking legs
	}
	
	void Stay ( )
	{
		_rigid->velocity = vec3(0);
	}

	void SetDirection ( vec3 target_direction )
	{
		if ( target_direction == vec3(0) )
		{
			Stay( );
			return;
		}
		
		_target_direction = normalize ( target_direction );
		_rigid->velocity = target_direction * move_speed;

		// orientation
		if ( !_head_locked_by_setting_attack_direction )
		{
			obj->Set_2D_Rot ( angle ( _target_direction ) ); // this is for head
		}

		obj->Get_Child ( "gambe" )->Set_2D_Rot ( angle ( _target_direction ) ); // this is for legs
	}

	void SetTarget ( vec3 target_position )
	{
		auto nn = target_position - obj->Get_Pos ( );

		if ( nn == vec3(0) )
		{ 
			_target_direction = vec3(0);
			Stay ( );
		} else {
			// _target_direction = normalize ( nn );
			SetDirection ( nn );
		}
	}
	void SetAttackDirection ( vec3 attack_direction )
	{
		if ( attack_direction == vec3(0) )
		{
			_attack_point = vec3{1,0,0};
			return;
		} else {
			_attack_point = normalize ( attack_direction );
		}

		_head_locked_by_setting_attack_direction = 2;

		obj->Set_2D_Rot ( angle ( _attack_point ) ); // this is for head
	}

	void Swing ( )
	{
		DEBUG ( 3, "Swing" );
		if ( _weapon )
		{ _weapon->Swing ( ); }
		else
		{ _hands.Swing( ); }
	}
	void Throw ( )
	{
		DEBUG ( 3, "Throw" );
		if ( _weapon )
		{
			if ( _weapon->Throw ( ) )
			{
				// detach weapon ( attach it to Weapons )
				Manager::Objekt_Get ( "Weapons" )
					->Add_Child ( _weapon->obj->Get_Name ( ) );
				_weapon = nullptr;
			}
		}
		else
		{ _hands.Throw( ); }
	}

	void PickWeapon ( )
	{
		if ( _weapon != nullptr )
		{ return; }
		
		DEBUG ( 3, "PickWeapon" );
		if ( 
			_candidate_for_pick_up != nullptr &&
			_candidate_for_pick_up->Get_Component < Weapon > ( )->IsPickable ( )
		) {
			obj->Add_Child ( _candidate_for_pick_up->Get_Name ( ) );
			_weapon = _candidate_for_pick_up->Get_Component < Weapon > ( );
		}
	}

	void Reward ( int value )
	{

	}

	void Collision_Trigger_Enter ( Objekt* obj ) override
	{
		if ( obj->Has_Component <Weapon> ( ) )
		{
			_candidate_for_pick_up = obj;
			DEBUG ( 4, "Candidate Enter: ", _candidate_for_pick_up->Get_Name ( ) );
		}
	}

	void Collision_Trigger_Exit ( Objekt* obj ) override
	{
		if (
			obj->Has_Component <Weapon> ( ) &&
			_candidate_for_pick_up == obj
		) {
			_candidate_for_pick_up = nullptr;
			DEBUG ( 4, "Candidate Exit: ", _candidate_for_pick_up->Get_Name ( ) );
		}
	}
};