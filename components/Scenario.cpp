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
			->Register ( "player", 0, 0, vec3{0,0,0}, "Player" )
			->Register ( "AI", 0, 0, vec3{300,300,0}, "Enemy" );
		Manager::Objekt_Load ( "Donators" );
		Manager::Objekt_Load ( "UI", vec3{660,-475,0}, vec3{50,50,10} )->Add_Component ( "Chat" );

		Manager::Objekt_Load ( "Ambient" )
			->Add_Child ( "Background")
			.Add_Child ( "UI" )
			.Add_Child ( "Donators" )
			.Add_Child ( "Runners" );

		obj->Add_Child ( "Ambient" );
		
	}
};