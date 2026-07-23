#include <engine.hpp>
#include "Runner_Manager.cpp"

class Scenario : public Behaviour
{
private:

public:
	Scenario ( )
	{
		Informations = { "Scenario", 1.0, "The scenenary load" };
	}

	void Start ( )
	{
		Manager::Objekt_Load ( "Ambient" );

		Manager::Objekt_Load ( "Background" );
		Manager::Objekt_Load ( "Runners" );
		Manager::Objekt_Load ( "Donators" );
		Manager::Objekt_Load ( "UI" )->Add_Component ( "Chat" );
		
	}
};