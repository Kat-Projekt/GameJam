#include <engine.hpp>

class Runner : public Behaviour
{
private:
	float move_speed = 200.0f;
public:
	Runner ( )
	{
		Informations = { "Runner", 1.0, "The ai for the enemy runners" };
	}

	void Start ( ) override
	{
		
	}

	void Update ( ) override
	{
		
	}

	void SetTarget ( vec3 target_position );
	void SetAttackDirection ( vec3 attack_direction );

	void Swing ( );
	void Throw ( );

	void PickWeapon ( );
};