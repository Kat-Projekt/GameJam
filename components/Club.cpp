#include <engine.hpp>
#include "Weapon.cpp"

class Club : public Weapon
{
private:
	/* data */
public:
	Club ( )
	{
		Informations = { "Weapon", 1.0, "This is the weapons information" };
		weapon_name  = "Club";
	}

	virtual void Swing ( ) { }

	virtual int Throw ( ) { return 0; }
};