#define DIAGNOSTIC
#include "engine.hpp"

int main ( )
{
	Factory::Register_Directory ( "." );

	ReKat::Graphik::Start ( "Font Test", 800, 600 );
	// ReKat::phisiks::Start ( 60 );

	Manager::Defaults_Load ( );
	DEBUG ( 3, "Started" );

	Manager::Make < Texture > ( "logo", "Logo.png" );
	Manager::Make < Font > ( "Aovel", "AovelSansRounded.ttf", 90 );

	// Manager::Objekt_Load ( "End Credits" )->Add_Component ( "EndScreen" );

	auto player = Manager::Objekt_Load ( "Player" );
	player->Add_Component ( "Player" );
	player->Add_Component < Sprite > ( )->Set ( "logo" );

	DEBUG ( 4, "STARTING" );

	Manager::Set_Active_Scene ( "Player" );

	while ( ReKat::Graphik::Is_End ( ) )
	{
		ReKat::Graphik::Clear_Screen ( 1.0f );
		Manager::Update ( );
		// ReKat::phisiks::Update ( );
		Timer::Update ( );
		ReKat::Graphik::Update ( );
	}

	Manager::Free ( );
	Manager::Free_Objekt ( "Player" );
}