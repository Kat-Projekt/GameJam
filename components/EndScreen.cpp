#define DIAGNOSTIC

#include <engine.hpp>

class EndScreen : public Behaviour
{
public:
	EndScreen ( )
	{
		Informations = { "EndScreen", 1.0, "Prints the credits" };
	}

	void Start ( )
	{
		// kat logo
		std::string credits = "-- Credits --";
		std::string coda_sinistra = "Director: \n\nProgramming: \nEngine Programming: \nGameplay Programming: \n\nGame Design: \nLevel Design: \n\nStory Director: \nWriter: \n\nArt Director: \nArtist: \nArtist: \nArtist: \n\nMusic:";
		std::string coda_destra = "EC\n\nEC\nEC\nLG\n\nEC\nLG\n\nCR\nAS\n\nFB\nFB\nAS\nBR\n\nLG";
		std::string engine = "This projekt was made possible by";

		float new_line = 18;

		auto bg = Manager::Objekt_Load ( "EndScreen_Background", { 0,-50*(new_line-1),0 }, {200,100*new_line,100} );
		bg->Add_Component < Sprite > ( )->Set ( vec4{0,0,0,1} );

		Manager::Make < Animation < vec3 > > ( "scrool", bg->Expose_Pos ( ), PlayMode::ONCE );
		auto scrool = Manager::Get < Animation < vec3 > > ( "scrool" );
		scrool	->Add_Frame ( vec3{0,-50*(new_line-1),0 }, vec3{1000,-1000,0}, 10 )
			->Add_Frame ( vec3{0,-100,0}, vec3{0,-100,0}, 3 );

		obj->Add_Component < Animator > ( )
			->New_Node ( "splash" )
			->Change_Animation ( "splash" )
			->Add_Animation ( "splash", scrool );

		obj->Add_Child ( bg );
		obj->Set_Pos ( {0,0,0} );

		std::shared_ptr < Text > coda_sinistra_component = std::make_shared < Text > ( );
		std::shared_ptr < Text > coda_destra_component = std::make_shared < Text > ( );
		std::shared_ptr < Text > coda_testa_component = std::make_shared < Text > ( );
		std::shared_ptr < Text > coda_kat_component = std::make_shared < Text > ( );

		coda_testa_component->Set ( "Aovel", "" )
			->Set ( credits, Text::ALIGNMENT::CENTER, Text::ALIGNMENT::BOTTOM );

		coda_sinistra_component->Set ( "Aovel", "" )
			->Set ( coda_destra, Text::ALIGNMENT::RIGHT, Text::ALIGNMENT::TOP )
			->Set ( vec4 { 1,0,0,1 } );

		coda_destra_component->Set ( "Aovel", "" )
			->Set ( coda_sinistra, Text::ALIGNMENT::LEFT, Text::ALIGNMENT::TOP );

		coda_kat_component->Set ( "Aovel", "" )
			->Set ( engine, Text::ALIGNMENT::LEFT, Text::ALIGNMENT::TOP );

		obj->Add_Component ( coda_destra_component );
		obj->Add_Component ( coda_sinistra_component );
		obj->Add_Component ( coda_testa_component );
	}
};
