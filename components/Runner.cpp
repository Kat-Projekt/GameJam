#define DIAGNOSTIC
#include <engine.hpp>

class Runner : public Behaviour
{
private:
	float move_speed = 400.0f;
	float time = 30.0f;
	std::string displaid_time = "";

	Text* _text_text = nullptr;
	Rigidbody* _rigid = nullptr;

	vec3 _target_direction = {0,0,0};
	vec3 _attack_point = {0,0,0};

	bool _head_locked_by_setting_attack_direction = false;
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
		_head_locked_by_setting_attack_direction = false; // for unlocking legs
	}

	void Stay ( )
	{
		DEBUG ( 5, "Staing" );
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
		DEBUG ( 5, "Setting target: ", target_position );

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

		_head_locked_by_setting_attack_direction = true;

		obj->Set_2D_Rot ( angle ( _attack_point ) ); // this is for head
	}

	void Swing ( );
	void Throw ( );

	void PickWeapon ( );

	void Reward ( int value )
	{

	}
};