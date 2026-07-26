#include <engine.hpp>
#include "../Weapon.cpp"
class Crowbar : public Weapon
{
private:
	/* data */
public:
	Crowbar ( )
	{
		Informations = { "Weapon", 1.0, "Crowbar" };
	}
	void Start ( ) override
	{
		Weapon::Start ( );
		Set_Range ( 200.0f );
		Set_Throw_Speed ( 600.0f );
	}
};