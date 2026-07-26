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
	void Load_Obstacle (
		std::string obstacle_name,
		ivec2 sprite_grid,
		int sprite_number,
		vec3 spawn_point,
		vec3 collider_size
	) {
		auto _obstacle = Manager::Objekt_Load ( obstacle_name, spawn_point );
		_obstacle->Set_Size ( collider_size );

		_obstacle->Add_Component < Box_Collider > ( )
			->Set_Size ( collider_size );

		_obstacle->Add_Component < Rigidbody > ( );

		_obstacle->Add_Component < Sprite > ( )
			->Set ( OBSTACLES_SHEET, "", MAIN_CAMERA, sprite_grid, sprite_number );

		Manager::Objekt_Get ( "Obstacles" )->Add_Child ( _obstacle );
	}

	void Start ( )
	{
		Manager::Objekt_Load ( "Background" )
			->Add_Component ( "Background" );

		Manager::Objekt_Load ( "Weapons" );
		Manager::Objekt_Load ( "Obstacles" );
		Manager::Objekt_Load ( "Runners" )
			->Add_Component < Runner_Manager > ( )
			->Register ( "player", 420, vec4{0,0,1,1}, vec3{0,0,0}, "Player" )
			/*->Register ( "AI_1", 67, vec4{0.9,0.2,0.2,1}, vec3{-3917,-2250,0}, "Enemy" )
			->Register ( "AI_2", 12, vec4{0.2,0.8,0.3,1}, vec3{2083,-2250,0}, "Enemy" )
			->Register ( "AI_3", 88, vec4{0.3,0.3,0.9,1}, vec3{-2583,-2083,0}, "Enemy" )
			->Register ( "AI_4", 34, vec4{0.9,0.6,0.1,1}, vec3{3250,-2083,0}, "Enemy" )
			->Register ( "AI_5", 77, vec4{0.6,0.1,0.7,1}, vec3{-1250,-1917,0}, "Enemy" )
			->Register ( "AI_6", 21, vec4{0.1,0.9,0.9,1}, vec3{1417,-1917,0}, "Enemy" )
			->Register ( "AI_7", 45, vec4{0.8,0.8,0.1,1}, vec3{-3417,-1750,0}, "Enemy" )
			->Register ( "AI_8", 60, vec4{0.3,0.6,0.3,1}, vec3{3750,-1750,0}, "Enemy" )
			->Register ( "AI_9", 15, vec4{0.7,0.2,0.4,1}, vec3{-417,-1583,0}, "Enemy" )
			->Register ( "AI_10", 90, vec4{0.4,0.4,0.9,1}, vec3{2417,-1583,0}, "Enemy" )
			->Register ( "AI_11", 27, vec4{0.9,0.3,0.6,1}, vec3{-2917,-1250,0}, "Enemy" )
			->Register ( "AI_12", 63, vec4{0.2,0.7,0.5,1}, vec3{1750,-1250,0}, "Enemy" )
			->Register ( "AI_13", 8, vec4{0.5,0.5,0.5,1}, vec3{-1750,-1083,0}, "Enemy" )
			->Register ( "AI_14", 99, vec4{0.9,0.9,0.4,1}, vec3{2917,-1083,0}, "Enemy" )
			->Register ( "AI_15", 36, vec4{0.3,0.1,0.8,1}, vec3{-3583,-917,0}, "Enemy" )
			->Register ( "AI_16", 71, vec4{0.1,0.6,0.1,1}, vec3{3750,-917,0}, "Enemy" )
			->Register ( "AI_17", 19, vec4{0.6,0.3,0.1,1}, vec3{-917,-750,0}, "Enemy" )
			->Register ( "AI_18", 82, vec4{0.8,0.1,0.9,1}, vec3{1417,-750,0}, "Enemy" )
			->Register ( "AI_19", 55, vec4{0.2,0.9,0.3,1}, vec3{-2250,-583,0}, "Enemy" )
			->Register ( "AI_20", 104, vec4{0.5,0.7,0.9,1}, vec3{3584,-583,0}, "Enemy" )
			->Register ( "AI_21", 42, vec4{0.9,0.4,0.2,1}, vec3{-3250,-417,0}, "Enemy" )
			->Register ( "AI_22", 69, vec4{0.2,0.5,0.8,1}, vec3{2250,-417,0}, "Enemy" )
			->Register ( "AI_23", 30, vec4{0.7,0.7,0.2,1}, vec3{-750,-250,0}, "Enemy" )
			->Register ( "AI_24", 95, vec4{0.4,0.2,0.6,1}, vec3{1583,-250,0}, "Enemy" )
			->Register ( "AI_25", 6, vec4{0.9,0.1,0.4,1}, vec3{-2750,-83,0}, "Enemy" )
			->Register ( "AI_26", 58, vec4{0.1,0.7,0.7,1}, vec3{2750,-83,0}, "Enemy" )
			->Register ( "AI_27", 24, vec4{0.6,0.9,0.1,1}, vec3{-1417,83,0}, "Enemy" )
			->Register ( "AI_28", 87, vec4{0.3,0.4,0.9,1}, vec3{3917,83,0}, "Enemy" )
			->Register ( "AI_29", 13, vec4{0.8,0.5,0.3,1}, vec3{-3750,417,0}, "Enemy" )
			->Register ( "AI_30", 76, vec4{0.5,0.1,0.9,1}, vec3{1917,417,0}, "Enemy" )
			->Register ( "AI_31", 40, vec4{0.2,0.8,0.6,1}, vec3{-1917,583,0}, "Enemy" )
			->Register ( "AI_32", 91, vec4{0.9,0.7,0.1,1}, vec3{3417,583,0}, "Enemy" )
			->Register ( "AI_33", 3, vec4{0.1,0.4,0.9,1}, vec3{-750,750,0}, "Enemy" )
			->Register ( "AI_34", 65, vec4{0.7,0.1,0.2,1}, vec3{1250,750,0}, "Enemy" )
			->Register ( "AI_35", 22, vec4{0.4,0.9,0.5,1}, vec3{-3083,917,0}, "Enemy" )
			->Register ( "AI_36", 84, vec4{0.6,0.2,0.8,1}, vec3{2584,917,0}, "Enemy" )
			->Register ( "AI_37", 50, vec4{0.9,0.5,0.6,1}, vec3{-2417,1250,0}, "Enemy" )
			->Register ( "AI_38", 17, vec4{0.2,0.6,0.9,1}, vec3{2083,1250,0}, "Enemy" )
			->Register ( "AI_39", 73, vec4{0.8,0.3,0.1,1}, vec3{-1083,1417,0}, "Enemy" )
			->Register ( "AI_40", 29, vec4{0.3,0.9,0.8,1}, vec3{3584,1017,0}, "Enemy" )
			->Register ( "AI_41", 61, vec4{0.9,0.2,0.7,1}, vec3{-4083,1583,0}, "Enemy" )
			->Register ( "AI_42", 9, vec4{0.5,0.8,0.2,1}, vec3{-2083,1583,0}, "Enemy" )
			->Register ( "AI_43", 96, vec4{0.1,0.3,0.6,1}, vec3{83,1750,0}, "Enemy" )
			->Register ( "AI_44", 47, vec4{0.7,0.6,0.9,1}, vec3{-3450,1750,0}, "Enemy" )*/
->Register ( "AI_1", 67, vec4{0.9,0.2,0.2,1}, vec3{-3917,-2250,0}, "Enemy" )
->Register ( "AI_2", 12, vec4{0.2,0.8,0.3,1}, vec3{2083,-2250,0}, "Enemy" )
->Register ( "AI_5", 77, vec4{0.6,0.1,0.7,1}, vec3{-1250,-1917,0}, "Enemy" )
->Register ( "AI_8", 60, vec4{0.3,0.6,0.3,1}, vec3{3750,-1750,0}, "Enemy" )
->Register ( "AI_9", 15, vec4{0.7,0.2,0.4,1}, vec3{-417,-1583,0}, "Enemy" )
->Register ( "AI_12", 63, vec4{0.2,0.7,0.5,1}, vec3{1750,-1250,0}, "Enemy" )
->Register ( "AI_13", 8, vec4{0.5,0.5,0.5,1}, vec3{-1750,-1083,0}, "Enemy" )
->Register ( "AI_16", 71, vec4{0.1,0.6,0.1,1}, vec3{3750,-917,0}, "Enemy" )
->Register ( "AI_17", 19, vec4{0.6,0.3,0.1,1}, vec3{-917,-750,0}, "Enemy" )
->Register ( "AI_20", 104, vec4{0.5,0.7,0.9,1}, vec3{3584,-583,0}, "Enemy" )
->Register ( "AI_21", 42, vec4{0.9,0.4,0.2,1}, vec3{-3250,-417,0}, "Enemy" )
->Register ( "AI_24", 95, vec4{0.4,0.2,0.6,1}, vec3{1583,-250,0}, "Enemy" )
->Register ( "AI_25", 6, vec4{0.9,0.1,0.4,1}, vec3{-2750,-83,0}, "Enemy" )
->Register ( "AI_28", 87, vec4{0.3,0.4,0.9,1}, vec3{3917,83,0}, "Enemy" )
->Register ( "AI_29", 13, vec4{0.8,0.5,0.3,1}, vec3{-3750,417,0}, "Enemy" )
->Register ( "AI_32", 91, vec4{0.9,0.7,0.1,1}, vec3{3417,583,0}, "Enemy" )
->Register ( "AI_33", 3, vec4{0.1,0.4,0.9,1}, vec3{-750,750,0}, "Enemy" )
->Register ( "AI_36", 84, vec4{0.6,0.2,0.8,1}, vec3{2584,917,0}, "Enemy" )
->Register ( "AI_37", 50, vec4{0.9,0.5,0.6,1}, vec3{-2417,1250,0}, "Enemy" )
->Register ( "AI_40", 29, vec4{0.3,0.9,0.8,1}, vec3{3584,1017,0}, "Enemy" )
->Register ( "AI_41", 61, vec4{0.9,0.2,0.7,1}, vec3{-4083,1583,0}, "Enemy" )
->Register ( "AI_42", 9, vec4{0.5,0.8,0.2,1}, vec3{-2083,1583,0}, "Enemy" )
->Register ( "AI_43", 96, vec4{0.1,0.3,0.6,1}, vec3{83,1750,0}, "Enemy" )
->Register ( "AI_44", 47, vec4{0.7,0.6,0.9,1}, vec3{-3450,1750,0}, "Enemy" )
			->Register_weapon < Axe > ( "Axe_1", 11, {-3334,-2000,0}, {100,220,50} )
			->Register_weapon < Axe > ( "Axe_2", 11, {1667,-1834,0}, {100,220,50} )
			->Register_weapon < Axe > ( "Axe_3", 11, {-2500,-1667,0}, {100,220,50} )
			->Register_weapon < Butcher_Knife > ( "Butcher_Knife_1", 2, {3333,-1500,0}, {100,180,50} )
			->Register_weapon < Butcher_Knife > ( "Butcher_Knife_2", 2, {-1667,-1167,0}, {100,180,50} )
			->Register_weapon < Butcher_Knife > ( "Butcher_Knife_3", 2, {2500,-1167,0}, {100,180,50} )
			->Register_weapon < Crowbar > ( "Crowbar_1", 6, {0,-1000,0}, {100,200,50} )
			->Register_weapon < Crowbar > ( "Crowbar_2", 6, {-3834,-834,0}, {100,200,50} )
			->Register_weapon < Crowbar > ( "Crowbar_3", 6, {3833,-834,0}, {100,200,50} )
			->Register_weapon < Knife > ( "Knife_1", 0, {-1167,-667,0}, {100,165,50} )
			->Register_weapon < Knife > ( "Knife_2", 0, {1167,-500,0}, {100,165,50} )
			->Register_weapon < Knife > ( "Knife_3", 0, {-2834,-334,0}, {100,165,50} )
			->Register_weapon < Metal_Pipe > ( "Metal_Pipe_1", 7, {2833,-167,0}, {100,240,50} )
			->Register_weapon < Metal_Pipe > ( "Metal_Pipe_2", 7, {-834,0,0}, {100,240,50} )
			->Register_weapon < Metal_Pipe > ( "Metal_Pipe_3", 7, {-4167,166,0}, {100,240,50} )
			->Register_weapon < Shovel > ( "Shovel_1", 15, {2167,500,0}, {100,280,50} )
			->Register_weapon < Shovel > ( "Shovel_2", 15, {-2167,666,0}, {100,280,50} )
			->Register_weapon < Shovel > ( "Shovel_3", 15, {4167,833,0}, {100,280,50} )
			->Register_weapon < Club > ( "Club_1", 1, {-334,1000,0}, {50,50,50} )
			->Register_weapon < Club > ( "Club_2", 1, {1667,1333,0}, {50,50,50} )
			->Register_weapon < Club > ( "Club_3", 1, {-3334,1500,0}, {50,50,50} );
			//->Register_weapon < Axe > ( "Axe", 11, {0,-200,0}, {100,220,50} )
			//->Register_weapon < Butcher_Knife > ( "Butcher_Knife", 2, {100,-200,0}, {100,180,50} )
			//->Register_weapon < Crowbar > ( "Crowbar", 6, {200,-200,0}, {100,200,50} )
			//->Register_weapon < Knife > ( "Knife", 0, {300,-200,0}, {100,165,50} )
			//->Register_weapon < Metal_Pipe > ( "Metal_Pipe", 7, {400,-200,0}, {100,240,50} )
			//->Register_weapon < Shovel > ( "Shovel", 15, {500,-200,0}, {100,280,50} );

		Manager::Objekt_Load ( "Chat", vec3{660,-475,0}, vec3{50,50,10} )->Add_Component ( "Chat" );

		Load_Obstacle ( "Container_1", {1,1}, 0, {600,-200,50}, {1200,3000,100} );

		Manager::Objekt_Load ( "Ambient" )
			->Add_Child ( "Runners")
			.Add_Child ( "Chat" )
			.Add_Child ( "Weapons" )
			.Add_Child ( "Background" )
			.Add_Child("Obstacles");

		obj->Add_Child ( "Ambient" );
		
	}
};