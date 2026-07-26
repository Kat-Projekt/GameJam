#include <engine.hpp>
#include "../Weapon.cpp"
class Axe : public Weapon
{
private:
	/* data */
public:
	Axe ( )
	{
		Informations = { "Weapon", 1.0, "Axe" };
	}
	void Start ( ) override
	{
		Weapon::Start ( );
		Set_Range ( 220.0f );
		Set_Throw_Speed ( 580.0f );
	}
};