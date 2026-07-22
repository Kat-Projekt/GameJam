#define DIAGNOSTIC
#include "engine.hpp"

int main ( )
{
	Factory::Register_Directory ( "." );

	ReKat::Graphik::Start ( "Font Test", 800, 600 );
	Manager::Defaults_Load ( );
	DEBUG ( 3, "Started" );

	Manager::Make < Texture > ( "logo", "Logo.png" );
	Manager::Make < Font > ( "Aovel", "AovelSansRounded.ttf", 90 );

	Manager::Objekt_Load ( "End Credits" )->Add_Component ( "EndScreen" );

	DEBUG ( 4, "STARTING" );

	Manager::Set_Active_Scene ( "End Credits" );

	while ( ReKat::Graphik::Is_End ( ) )
	{
		ReKat::Graphik::Clear_Screen ( 1.0f );
		Manager::Update ( );
		ReKat::Graphik::Update ( );
	}

	Manager::Free ( );
	Manager::Free_Objekt ( "End Credits" );
}