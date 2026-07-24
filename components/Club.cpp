#include <engine.hpp>
#include "Weapon.cpp"

class Club : public Weapon
{
private:
	/* data */
public:
	Club ( )
	{
		Informations = { "Club", 1.0, "This is the Clubs information" };
	}

	virtual void Swing ( ) { }

	virtual int Throw ( ) { return 0; }
};