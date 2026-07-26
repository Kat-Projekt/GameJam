#include <engine.hpp>
#include "Runner_Manager.cpp"
#include "Club.cpp"
#include "weapons/Axe.cpp"
#include "weapons/Butcher_Knife.cpp"
#include "weapons/Crowbar.cpp"
#include "weapons/Knife.cpp"
#include "weapons/Metal_Pipe.cpp"
#include "weapons/Shovel.cpp"

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
			//->Register ( "AI", 67, vec4{1,1,0,1}, vec3{300,300,0}, "Enemy" )
			->Register_weapon < Axe > ( "Axe", 11, {0,-200,0}, {100,220,50} )
			->Register_weapon < Butcher_Knife > ( "Butcher_Knife", 2, {100,-200,0}, {100,180,50} )
			->Register_weapon < Crowbar > ( "Crowbar", 6, {200,-200,0}, {100,200,50} )
			->Register_weapon < Knife > ( "Knife", 0, {300,-200,0}, {100,165,50} )
			->Register_weapon < Metal_Pipe > ( "Metal_Pipe", 7, {400,-200,0}, {100,240,50} )
			->Register_weapon < Shovel > ( "Shovel", 15, {500,-200,0}, {100,280,50} );

		Manager::Objekt_Load ( "Chat", vec3{660,-475,0}, vec3{50,50,10} )->Add_Component ( "Chat" );

		Manager::Objekt_Load ( "Ambient" )
			->Add_Child ( "Runners")
			.Add_Child ( "Chat" )
			.Add_Child ( "Weapons" )
			.Add_Child ( "Background" );

		obj->Add_Child ( "Ambient" );
		
	}
};