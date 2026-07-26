#include <engine.hpp>
#include "../Weapon.cpp"
class Butcher_Knife : public Weapon
{
private:
	/* data */
public:
	Butcher_Knife ( )
	{
		Informations = { "Weapon", 1.0, "Butcher_Knife" };
	}
	void Start ( ) override
	{
		Weapon::Start ( );
		Set_Range ( 180.0f );
		Set_Throw_Speed ( 650.0f );
	}
};