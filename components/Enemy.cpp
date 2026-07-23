#include <engine.hpp>

class Enemy : public Behaviour
{
private:
	std::vector < Behaviour* > _runners;
	std::vector < Behaviour* > _weapons;
public:
	Enemy ( )
	{
		Informations = { "Enemy", 1.0, "The Count down / life manager" };
	}

	void Register ( std::string name, vec3 spawn_point, std::string controller );

	void Register_weapon ( std::string weapon_name, vec3 spawn_point );
};