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

		Manager::Objekt_Load ( "Background" );
		Manager::Objekt_Load ( "Runners" )
			->Add_Component < Runner_Manager > ( )
			->Register ( "player", 0, "logo", vec3{0,0,0}, "Player" );
		Manager::Objekt_Load ( "Donators" );
		Manager::Objekt_Load ( "UI" )->Add_Component ( "Chat" );
<<<<<<< HEAD

		Manager::Objekt_Load ( "Ambient" )
			->Add_Child ( "Background")
			.Add_Child ( "UI" )
			.Add_Child ( "Donators" )
			.Add_Child ( "Runners" );

		obj->Add_Child ( "Ambient" );
=======
		
>>>>>>> 7b4f050bac855b2695458dc2384cc97449f58bd1
	}
};