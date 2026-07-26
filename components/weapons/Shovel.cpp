#include <engine.hpp>
#include "../Weapon.cpp"
class Shovel : public Weapon
{
private:
	/* data */
public:
	Shovel ( )
	{
		Informations = { "Weapon", 1.0, "Shovel" };
	}
	void Start ( ) override
	{
		Weapon::Start ( );
		Set_Range ( 280.0f );
		Set_Throw_Speed ( 500.0f );
	}
};