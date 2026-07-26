#define DIAGNOSTIC

#include <engine.hpp>
#include "Weapon.cpp"
#include "file_refes.h"
#include "Donator_Manager.cpp"

class Runner; // forward declaration

class Punch_Hitbox : public Behaviour
{
public:
	Punch_Hitbox ( )
	{
		Informations = { "Punch_Hitbox", 1.0, "Melee hitbox for bare-handed attacks" };
	}
	void Collision_Trigger_Enter ( Objekt* _obj ) override;
};


class Hands : public Weapon
{
public:
	void Swing ( ) override
	{
		DEBUG ( 3, "Punch" );
	}
	int Throw ( vec3 direction, float strength ) override
	{
		DEBUG ( 3, "Cannot throw fists" );
		return 0;
	}
};

int _Lerp ( int a, int b, float t ) { return a + (b - a) * t; }

class Runner : public Behaviour
{
private:
	float move_speed = 400.0f;
	float _time = 30.0f;
	std::string displaid_time = "";

	vec4 _color = {1,1,1,1};
	int _player_number = 0;

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

	const float _punch_hitbox_size = 200.0f;

	bool _charging_throw = false;
	float _throw_charge = 0.0f;
	const float _max_charge_time = 1.0f;
	const float _min_throw_strength = 0.3f;

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
		DEBUG ( 3, "Runner Box_Collider size: ", obj->Get_Size ( ) );
		
		auto _testa = std::make_shared < Objekt > ( "testa", vec3{0,0,0}, vec3{200,200,200} );
		auto _gambe = std::make_shared < Objekt > ( "gambe", vec3{0,0,0}, vec3{200,200,200} );
		auto _tempo = std::make_shared < Objekt > ( "tempo_rimasto", vec3{0,0,0}, vec3{40,40,40}, vec3{-0.1,-0.2,0} );
		auto _numbe = std::make_shared < Objekt > ( "player_number", vec3{0,0,0}, vec3{20,20,20}, vec3{-0.2,0.6,0} );

		auto _gambe_sprite = _gambe->Add_Component < Sprite > ( )
			->Set ( RUNNERS_SHEET, "", MAIN_CAMERA, {15,2} )
			->Set ( _color );

		auto _testa_sprite = _testa->Add_Component < Sprite > ( )
			->Set ( RUNNERS_SHEET, "", MAIN_CAMERA, {15,2} )
			->Set ( _color );

		_remaining_time = _tempo->Add_Component < Text > ( )
			->Set ( AOVEL_SANS_ROUNDED_FONT, "", MAIN_CAMERA )
			->Set ( "", Text::ALIGNMENT::CENTER, Text::ALIGNMENT::CENTER )
			->Set ( vec4{1.0f,0.5f,0.5f,1.0f} - _color + vec4{0,0,0,1});
		
		_numbe->Add_Component < Text > ( )
			->Set ( AOVEL_SANS_ROUNDED_FONT, "", MAIN_CAMERA )
			->Set ( std::to_string ( _player_number ) , Text::ALIGNMENT::CENTER, Text::ALIGNMENT::CENTER )
			->Set ( vec4{1.0f,0.5f,0.5f,1.0f} - _color + vec4{0,0,0,1} );

		auto _pugno = std::make_shared < Objekt > ( "pugno", vec3{0,0,0}, vec3{150,150,150} );
		auto _pugno_collider = _pugno->Add_Component < Box_Collider > ( );
		_pugno_collider->Set_Size ( vec3{150,150,150} );
		_pugno_collider->Set_Trigger ( true );
		_pugno_collider->Set_Active ( false );
		_pugno->Add_Component < Punch_Hitbox > ( );
		obj->Add_Child ( _pugno );

		obj->Add_Child ( _testa );
		obj->Add_Child ( _gambe );
		obj->Add_Child ( _tempo );
		obj->Add_Child ( _numbe );

		// creating animations
		std::string n = obj->Get_Name ( );
		Manager::Make < Animation < int > > ( "idle_legs" + n, &(_gambe_sprite->_frame), PlayMode::LOOP );
		Manager::Make < Animation < int > > ( "idle_head" + n, &(_testa_sprite->_frame), PlayMode::LOOP );

		Manager::Make < Animation < int > > ( "walking_legs" + n, &(_gambe_sprite->_frame), PlayMode::LOOP );
		Manager::Make < Animation < int > > ( "walking_head" + n, &(_testa_sprite->_frame), PlayMode::LOOP );

		Manager::Make < Animation < int > > ( "swing_animation" + n, &(_testa_sprite->_frame), PlayMode::ONCE );
		Manager::Make < Animation < int > > ( "fists_animation" + n, &(_testa_sprite->_frame), PlayMode::ONCE );
		Manager::Make < Animation < int > > ( "throw_animation" + n, &(_testa_sprite->_frame), PlayMode::ONCE );

		// getting animations
		auto idle_legs		= Manager::Get < Animation < int > > ( "idle_legs" + n );
		auto idle_head		= Manager::Get < Animation < int > > ( "idle_head" + n );

		auto walking_legs	= Manager::Get < Animation < int > > ( "walking_legs" + n );
		auto walking_head	= Manager::Get < Animation < int > > ( "walking_head" + n );

		auto swing_animation	= Manager::Get < Animation < int > > ( "swing_animation" + n );
		auto fists_animation	= Manager::Get < Animation < int > > ( "fists_animation" + n );
		auto throw_animation	= Manager::Get < Animation < int > > ( "throw_animation" + n );

		// configuring animations

		idle_legs->Add_Frame ( 15+0, 15+0, 1, &_Lerp );
		idle_head->Add_Frame ( 4, 4, 1, &_Lerp );

		walking_legs->Add_Frame ( 15+1, 15+4, 0.3, &_Lerp );
		walking_head->Add_Frame ( 5, 8, 0.3, &_Lerp );

		swing_animation->Add_Frame ( 9, 12, 1, &_Lerp );
		fists_animation->Add_Frame ( 0, 3, 1, &_Lerp );
		throw_animation->Add_Frame ( 13, 14, 1, &_Lerp );

		// configuring Animator
		obj->Add_Component < Animator > ( )
			->New_Node ( "idle" )
				->Add_Animation ( "idle", idle_legs )
				->Add_Animation ( "idle", idle_head )
			->New_Node ( "walking" )
				->Add_Animation ( "walking", walking_legs )
				->Add_Animation ( "walking", walking_head )
			->New_Node ( "swing" )
				->Add_Animation ( "swing", swing_animation )
			->New_Node ( "punch" )
				->Add_Animation ( "punch", fists_animation )
			->New_Node ( "throw" )
				->Add_Animation ( "throw", throw_animation );

		obj->Get_Component < Animator > ( )->Change_Animation ( "idle" );

		_hands.Set_Range ( _punch_hitbox_size * 0.5f );
	}

	void Update ( ) override
	{
		_time -= Timer::Get_Delta ( );
		Update_Charging_Throw ( Timer::Get_Delta ( ) );

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

		if ( Timer::Get_Time ( ) >= _next_attack_window )
		{
			auto pugno_collider = obj->Get_Child ( "pugno" )->Get_Component < Box_Collider > ( );
			if ( pugno_collider && pugno_collider->Get_Active ( ) )
			{ pugno_collider->Set_Active ( false ); }
		}
	}
	
	void Stay ( )
	{
		_rigid->velocity = vec3(0);
		obj->Get_Component < Animator > ( )->Change_Animation ( "idle" );
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
		obj->Get_Component < Animator > ( )->Change_Animation ( "walking" );
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
		{ 
			_weapon->Swing ( ); 
		}
		else
		{ 
			_hands.Swing( );
			obj->Get_Child ( "pugno" )->Get_Component < Box_Collider > ( )->Set_Active ( true );
		}
	}

	int Execute_Throw ( float strength )
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
			vec3 direction = _attack_point;
			if ( direction == vec3(0) ) { direction = _target_direction; }
			if ( direction == vec3(0) ) { direction = vec3{1,0,0}; }
			vec3 holder_pos = obj->Get_Pos ( );

			if ( _weapon->Throw ( direction, strength ) <= 0 )
			{
				Manager::Objekt_Get ( "Weapons" )
					->Add_Child ( _weapon->obj->Get_Name ( ) );
				_weapon->obj->Set_Pos ( holder_pos );
				_weapon = nullptr;
			}
		}
		else
		{ _hands.Throw ( vec3{0,0,0}, 0.0f ); }

		return 0;
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
			DEBUG ( 3, "Picked Weapon: ",  _candidate_for_pick_up->Get_Name ( ) );
			obj->Add_Child ( _candidate_for_pick_up->Get_Name ( ) );
			_weapon = _candidate_for_pick_up->Get_Component < Weapon > ( )->Pick ( );
			_candidate_for_pick_up = nullptr;
		}
	}
	void Start_Charging_Throw ( )
	{
		if ( !_weapon ) return;
		_charging_throw = true;
		_throw_charge = 0.0f;
	}
	void Update_Charging_Throw ( float dt )
	{
		if ( !_charging_throw ) return;
		_throw_charge += dt;
		if ( _throw_charge > _max_charge_time )
		{ _throw_charge = _max_charge_time; }
	}
	void Release_Throw ( )
	{
		if ( !_charging_throw ) return;
		float strength = _throw_charge / _max_charge_time;
		if ( strength < _min_throw_strength )
		{ strength = _min_throw_strength; }
		_charging_throw = false;
		_throw_charge = 0.0f;
		Execute_Throw ( strength );
	}
	void Throw_Full_Power ( )
	{
		Execute_Throw ( 1.0f );
	}
	void Reward ( float value )
	{
		DEBUG ( 5, "Player: ", obj->Get_Name ( ), " Prize: ", value );
		_time += value;
	}
	void Killed ( Objekt* killer )
	{
		if ( killer == obj && _time > 0 )
		{ return; }
		Manager::Objekt_Get ( "Chat" )
			->Get_Component < Donator_Manager >( )
			->PlayerDeath ( obj->Get_Name ( ), killer->Get_Name ( ), _time < 0 );

		killer->Get_Component < Runner > ( )->Reward ( _time / 3.0f );

		if ( _weapon != nullptr )
		{
			vec3 death_pos = obj->Get_Pos ( );
			Manager::Objekt_Get ( "Weapons" )
				->Add_Child ( _weapon->obj->Get_Name ( ) );
			_weapon->obj->Set_Pos ( death_pos );
			_weapon->Land ( );
			if ( auto* col = _weapon->obj->Get_Component < Box_Collider > ( ) )
			{ col->Set_Active ( true ); }
			_weapon = nullptr;
		}

		// deactivate this
		obj->Set_Active ( false );

	}

	void Collision_Trigger_Enter ( Objekt* _obj ) override
	{
		Weapon * _weapon_componet = _obj->Get_Component < Weapon > ( );
		if ( _weapon_componet )
		{
			if ( !_weapon_componet->Is_In_Flight ( ) && _obj->Get_Father ( ) && _obj->Get_Father ( ).get ( ) == obj )
			{ return; }

			if ( _weapon_componet->Is_In_Flight ( ) && _weapon_componet->Get_Thrown_By ( ) == obj )
			{ return; } // non ci si ferisce con la propria arma appena lanciata

			if ( _obj != obj && _weapon_componet->IsPickable ( ) )
			{
				_candidate_for_pick_up = _obj;
				DEBUG ( 4, "Candidate Enter: ", _candidate_for_pick_up->Get_Name ( ) );
			} else {
				Objekt* killer = _weapon_componet->Is_In_Flight ( )
					? _weapon_componet->Get_Thrown_By ( )
					: _obj->Get_Father ( ).get ( );
				if ( killer )
				{
					Killed ( killer );
					if ( _weapon_componet->Is_In_Flight ( ) ){
						_weapon_componet->Land ( );
					}
				}
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

	void Set ( vec4 color, int player_number )
	{
		_color = color;
		_player_number = player_number;
	}

	bool Has_Weapon ( ) const { return _weapon != nullptr; }

	float Get_Attack_Range ( ) const
	{
		if ( _weapon ) return _weapon->Get_Range ( );
		return _hands.Get_Range ( );
	}

	void Set_Speed ( float new_speed ) 
	{
		move_speed = new_speed;
	} 

	void Clear_Attack_Direction ( ) 
	{ 
		_attack_point = vec3(0); 
	}
};

void Punch_Hitbox::Collision_Trigger_Enter ( Objekt* _obj )
{
	auto father = obj->Get_Father ( );
	if ( !father ) return;

	Runner* target = _obj->Get_Component < Runner > ( );
	if ( !target ) return;

	target->Killed ( father.get ( ) );
}