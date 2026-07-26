#include <engine.hpp>
#include "Weapon.cpp"
class Club : public Weapon
{
private:
	/* data */
public:
	Club ( )
	{
		Informations = { "Weapon", 1.0, "A blunt melee weapon (Club)" };
		weapon_name  = "Club";
	}
	void Start ( ) override
	{
		Weapon::Start ( );
		Set_Range ( 400.0f );
		Set_Throw_Speed ( 800.0f );
	}
};