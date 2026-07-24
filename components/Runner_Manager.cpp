#include <engine.hpp>

class Runner_Manager : public Behaviour
{
private:
	std::vector < Behaviour* > _runners;
	std::vector < Behaviour* > _weapons;
public:
	Runner_Manager ( )
	{
		Informations = { "Runner_Manager", 1.0, "The Count down / life manager" };
	}

	void Register (
		std::string name,
		int sprite_testa,
		int sprite_gambe,
		vec3 spawn_point,
		std::string controller
	) {
		auto _gambe = Manager::Objekt_Load ( name, spawn_point );
		auto _testa = std::make_shared < Objekt > ( "testa" );

		_gambe->Add_Component < Sprite > ( )
			->Set ( "logo", "", "", {4,1}, sprite_gambe )
			.Set ( true ); // 4 animation frames
		_gambe->Add_Component ( "Runner" );
		_gambe->Add_Component ( controller );

		_testa->Add_Component < Sprite > ( )
			->Set ( "coco", "", "", {2,2}, sprite_testa )
			.Set ( true ); // only the head

		_gambe->Add_Child ( _testa );
		obj->Add_Child ( _gambe );
	}

	void Register_weapon ( std::string weapon_name, int sprite_number, vec3 spawn_point );
};