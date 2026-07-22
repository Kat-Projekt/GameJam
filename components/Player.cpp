#include <engine.hpp>

class Player : public Behaviour
{
private:
	/* data */
public:
	Player ( )
	{
		Informations = { "Player", 1.0, "The player controller" };
	}
};