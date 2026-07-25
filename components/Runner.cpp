#define DIAGNOSTIC

#include <engine.hpp>
#include "Weapon.cpp"
#include "file_refes.h"
#include "Chat.cpp"

class Hands : public Weapon
{
public:
	void Swing ( ) override
	{
		DEBUG ( 3, "Punch" );
	}
	int Throw ( ) override
	{
		DEBUG ( 3, "Cannot throw fists" );
		return 0;
	}
};

class Runner : public Behaviour
{
private:
	float move_speed = 400.0f;
	float _time = 30.0f;
	std::string displaid_time = "";

	int _head = 0;
	int _legs = 0;

	Text* _remaining_time = nullptr;
	Rigidbody* _rigid = nullptr;

	Weapon* _weapon = nullptr;
	Objekt* _candidate_for_pick_up = nullptr;
	Hands _hands;

	vec3 _target_direction = {0,0,0};
	vec3 _attack_point = {0,0,0};

	int _head_locked_by_setting_attack_direction = 0;

	float _next_attack_window = 0;
	float _attack_cooldown = 0.5f;

	void RotateHead ( float _angle )
	{
		obj->Get_Child ( "testa" )->Set_2D_Rot ( _angle ); // this is for head
		obj->Get_Child ( "tempo_rimasto" )->Set_2D_Rot ( _angle ); // this is for head
		obj->Get_Child ( "player_number" )->Set_2D_Rot ( _angle ); // this is for head
		if ( _weapon != nullptr )
		{
			_weapon->obj->Set_2D_Rot ( _angle );
		}
	}
public:
	Runner ( )
	{
		Informations = { "Runner", 1.0, "The ai for the enemy runners" };
	}

	void Start ( ) override
	{
		_rigid = obj->Add_Component < Rigidbody > ( );
		obj->Add_Component < Box_Collider > ( )->Set_Size ( obj->Get_Size ( ) );
		
		auto _testa = std::make_shared < Objekt > ( "testa", vec3{0,0,0}, vec3{200,200,200} );
		auto _gambe = std::make_shared < Objekt > ( "gambe", vec3{0,0,0}, vec3{200,200,200} );
		auto _tempo = std::make_shared < Objekt > ( "tempo_rimasto", vec3{0,0,0}, vec3{40,40,40}, vec3{-0.25,-0.2+0.75,0} );
		auto _numbe = std::make_shared < Objekt > ( "player_number", vec3{0,0,0}, vec3{20,20,20}, vec3{-0.5,0.6+1.5,0} );

		auto _gambe_sprite = _gambe->Add_Component < Sprite > ( )
			->Set ( RUNNERS_SHEET, "", "", {15,2}, _legs )
			.Set ( true );

		auto _testa_sprite = _testa->Add_Component < Sprite > ( )
			->Set ( RUNNERS_SHEET, "", "", {15,2}, _head )
			.Set ( true );

		_remaining_time = _tempo->Add_Component < Text > ( )
			->Set ( AOVEL_SANS_ROUNDED_FONT, "", "" )
			->Set ( "", Text::ALIGNMENT::CENTER, Text::ALIGNMENT::CENTER )
			->Set ( vec4{1.0f,0.5f,0.5f,1.0f} );
		
		_numbe->Add_Component < Text > ( )
			->Set ( AOVEL_SANS_ROUNDED_FONT, "", "" )
			->Set ( std::to_string ( _head ) + std::to_string ( _legs )  , Text::ALIGNMENT::CENTER, Text::ALIGNMENT::CENTER )
			->Set ( vec4{1.0f,0.5f,0.5f,1.0f} );

		obj->Add_Child ( _testa );
		obj->Add_Child ( _gambe );
		obj->Add_Child ( _tempo );
		obj->Add_Child ( _numbe );

		// creating animations
		std::string n = obj->Get_Name ( );
		Manager::Make < Animation < int > > ( "idle_legs" + n, &(_gambe_sprite._frame), PlayMode::LOOP );
		Manager::Make < Animation < int > > ( "idle_head" + n, &(_testa_sprite._frame), PlayMode::LOOP );

		Manager::Make < Animation < int > > ( "walking_legs" + n, &(_gambe_sprite._frame), PlayMode::LOOP );
		Manager::Make < Animation < int > > ( "walking_head" + n, &(_testa_sprite._frame), PlayMode::LOOP );

		Manager::Make < Animation < int > > ( "swing_animation" + n, &(_testa_sprite._frame), PlayMode::ONCE );
		Manager::Make < Animation < int > > ( "fists_animation" + n, &(_testa_sprite._frame), PlayMode::ONCE );
		Manager::Make < Animation < int > > ( "throw_animation" + n, &(_testa_sprite._frame), PlayMode::ONCE );

		/*
		Manager::Make < Animation < float > > ( "weapon_swing" + weapon_name, &(obj->Get_Transform ( ).Expose_2D_Rot ( )), PlayMode::ONCE );
		auto swing = Manager::Get < Animation < float > > ( "weapon_swing" + weapon_name );

		swing	->Add_Frame ( -M_PI_4, M_PI_4, 1 );

		obj	->Add_Component < Animator > ( )
				->New_Node ( "swing" )
				->Add_Animation ( "swing", swing );
		*/
	}

	void Update ( ) override
	{
		_time -= Timer::Get_Delta ( );

		if ( _time <= 0.0f )
		{
			Killed ( obj );
		}

		int value = static_cast<int>(_time*100);

		std::string new_time = std::to_string(value);

		// Pad with leading zeros
		while ( new_time.length() < 4 )
		{ new_time = "0" + new_time; }

		// Insert ':'
		new_time.insert(2, ":");

		if ( new_time != displaid_time )
		{
			displaid_time = new_time;
			_remaining_time->Set ( displaid_time );
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
		_rigid->velocity = _target_direction * move_speed;

		// orientation
		if ( !_head_locked_by_setting_attack_direction )
		{
			RotateHead ( angle ( _target_direction ) ); // this is for head
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

		RotateHead ( angle ( _attack_point ) ); // this is for head
	}

	void Swing ( )
	{
		if ( _next_attack_window > Timer::Get_Time ( ) )
		{
			DEBUG ( 4, "Already attacke wait" );
			return;
		}

		_next_attack_window = Timer::Get_Time ( ) + _attack_cooldown;
		DEBUG ( 3, "Swing" );
		if ( _weapon )
		{ _weapon->Swing ( ); }
		else
		{ _hands.Swing( ); }
	}
	int Throw ( )
	{
		if ( _next_attack_window > Timer::Get_Time ( ) )
		{
			DEBUG ( 4, "Already attacke wait" );
			return 1;
		}

		_next_attack_window = Timer::Get_Time ( ) + _attack_cooldown;
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

		return 0;
	}

	void PickWeapon ( )
	{
		if ( Throw ( ) != 0 )
		{ return; }

		if ( _weapon != nullptr )
		{ return; }
		
		DEBUG ( 3, "PickWeapon" );
		if ( 
			_candidate_for_pick_up != nullptr &&
			_candidate_for_pick_up->Get_Component < Weapon > ( )->IsPickable ( )
		) {
			DEBUG ( 3, "Picked Weapon: ",  _candidate_for_pick_up->Get_Name ( ) );
			obj->Add_Child ( _candidate_for_pick_up->Get_Name ( ) );
			_weapon = _candidate_for_pick_up->Get_Component < Weapon > ( )->Pick ( );
			_candidate_for_pick_up = nullptr;
		}
	}
	void Reward ( float value )
	{
		DEBUG ( 5, "Player: ", obj->Get_Name ( ), " Prize: ", value );
		_time += value;
	}
	void Killed ( Objekt* _obj )
	{
		if ( _obj == obj && _time > 0 )
		{ return; }

		DEBUG ( 3, "Runner: ", obj->Get_Name ( ), " Killed by: ", _obj->Get_Name ( ) );
		_obj->Get_Component < Runner > ( )->Reward ( _time / 3.0f );
		// deactivate this
		obj->Set_Active ( false );

		Manager::Objekt_Get ( "Chat" )->Get_Component < Chat > ( )->PlayerDeath ( obj->Get_Name ( ) );
	}

	void Collision_Trigger_Enter ( Objekt* _obj ) override
	{
		Weapon * _weapon_componet = _obj->Get_Component < Weapon > ( );
		if ( _weapon_componet )
		{
			if ( _obj != obj && _weapon_componet->IsPickable ( ) )
			{
				_candidate_for_pick_up = _obj;
				DEBUG ( 4, "Candidate Enter: ", _candidate_for_pick_up->Get_Name ( ) );
			} else {
				Killed ( _obj->Get_Father ( ).get ( ) );
			}
		}
	}
	void Collision_Trigger_Exit ( Objekt* obj ) override
	{
		if (
			obj->Has_Component <Weapon> ( ) &&
			_candidate_for_pick_up == obj
		) {
			DEBUG ( 4, "Candidate Exit: ", _candidate_for_pick_up->Get_Name ( ) );
			_candidate_for_pick_up = nullptr;
		}
	}

	void Set ( int head, int legs )
	{
		_head = head;
		_legs = legs;
	}

	bool Has_Weapon ( ) const { return _weapon != nullptr; }
};