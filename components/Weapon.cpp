#include <engine.hpp>

class Weapon : public Behaviour
{
private:
	/* data */
public:
	Weapon ( )
	{
		Informations = { "Weapon", 1.0, "This is the weapons information" };
	}

	virtual int Swing ( ) { return 0; }
	virtual int Throw ( ) { return 0; }
};