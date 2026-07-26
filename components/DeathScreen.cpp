#include <engine.hpp>
#include <Runner.cpp>

class DeathScreen : public Behaviour
{
private:
	/* data */
public:
	DeathScreen ( )
	{
		Informations = { "DeathScreen", 1, "" };
	}

	void Start ( )
	{
		auto runner = Manager::Objekt_Get ( "player" )->Get_Component < Runner > ( );

		runner
	}
};