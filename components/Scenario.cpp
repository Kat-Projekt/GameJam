#include <engine.hpp>
#include "Runner_Manager.cpp"
#include "Club.cpp"

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
		Manager::Objekt_Load ( "Background" )
			->Add_Component ( "Background" );

		Manager::Objekt_Load ( "Weapons" );
		Manager::Objekt_Load ( "Runners" )
			->Add_Component < Runner_Manager > ( )
			->Register ( "player", 420, vec4{0,0,1,1}, vec3{0,0,0}, "Player" )
			->Register ( "AI", 67, vec4{1,1,0,1}, vec3{300,300,0}, "Enemy" )
			->Register_weapon < Club > ( "club", 1, {0,-200,0}, {50,50,50} );

		Manager::Objekt_Load ( "Chat", vec3{660,-475,0}, vec3{50,50,10} )->Add_Component ( "Chat" );

		Manager::Objekt_Load ( "Ambient" )
			->Add_Child ( "Background")
			.Add_Child ( "Chat" )
			.Add_Child ( "Weapons" )
			.Add_Child ( "Runners" );

		obj->Add_Child ( "Ambient" );
		
	}
};