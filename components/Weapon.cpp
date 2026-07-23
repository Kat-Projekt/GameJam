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

	void Swing ( );
};