#include <engine.hpp>
#include "../Weapon.cpp"
class Metal_Pipe : public Weapon
{
private:
	/* data */
public:
	Metal_Pipe ( )
	{
		Informations = { "Weapon", 1.0, "Metal_Pipe" };
	}
	void Start ( ) override
	{
		Weapon::Start ( );
		Set_Range ( 240.0f );
		Set_Throw_Speed ( 540.0f );
	}
};