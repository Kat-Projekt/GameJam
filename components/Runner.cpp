#define DIAGNOSTIC
#include <engine.hpp>

class Runner : public Behaviour
{
private:
	float move_speed = 100.0f;
	float time = 0.30f;
	std::string displaid_time = "";

	Text* _text_text = nullptr;
	Rigidbody* _rigid = nullptr;

	vec3 _target_position = {0,0,0};
	vec3 _attack_point = {0,0,0};
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

		int value = static_cast<int>(time);

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
	}

	void Stay ( )
	{
		DEBUG ( 5, "Staing" );
		_rigid->velocity = vec3(0);
	}

	void SetTarget ( vec3 target_position )
	{
		DEBUG ( 5, "Setting target: ", target_position );
		if ( target_position == _target_position )
		{
			return;
		}

		_target_position = target_position;
		auto nn = target_position;

		vec3 _target_direction;
		if ( nn == vec3(0) )
		{ 
			_target_direction = vec3(0);
		} else {
			_target_direction = normalize ( nn );
		}
	
		// move logic
		_rigid->velocity = _target_direction * move_speed;
		DEBUG ( 5, "velocity: ", _target_direction * move_speed );

		// orientation
		obj->Set_2D_Rot ( angle ( _target_direction ) );
	}
	void SetAttackDirection ( vec3 attack_direction )
	{
		if ( attack_direction == vec3(0) )
		{
			_attack_point = vec3{1,0,0};
		} else {
			_attack_point = normalize ( attack_direction );
		}	

		obj->Get_Child ( "testa" )->Set_2D_Rot ( angle ( _attack_point ) );
	}

	void Swing ( );
	void Throw ( );

	void PickWeapon ( );

	void Reward ( int value )
	{

	}
};