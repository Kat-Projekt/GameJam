#include <engine.hpp>
#include "../Weapon.cpp"
class Knife : public Weapon
{
private:
	/* data */
public:
	Knife ( )
	{
		Informations = { "Weapon", 1.0, "Knife" };
	}
	void Start ( ) override
	{
		Weapon::Start ( );
		Set_Range ( 165.0f );
		Set_Throw_Speed ( 700.0f );
	}
};