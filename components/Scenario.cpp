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

		Manager::Objekt_Load ( "Weapons" );
		Manager::Objekt_Load ( "Runners" )
			->Add_Component < Runner_Manager > ( )
			->Register ( "player", 0, 0, vec3{0,0,0}, "Player" )
			->Register_weapon ( "club", 1, {0,-200,0}, {50,50,50}, "Club" );

		Manager::Objekt_Load ( "Donators" );
		Manager::Objekt_Load ( "UI", vec3{660,-475,0}, vec3{50,50,10} )->Add_Component ( "Chat" );

		Manager::Objekt_Load ( "Ambient" )
			->Add_Child ( "Background")
			.Add_Child ( "UI" )
			.Add_Child ( "Donators" )
			.Add_Child ( "Runners" )
			.Add_Child ( "Weapons" );

		obj->Add_Child ( "Ambient" );
		
	}
};