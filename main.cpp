#include "engine.hpp"
#include "components/file_refes.h"

int load ( )
{
	int result = 0;

	Factory::Register_Directory ( "." );

	result += Manager::Defaults_Load ( );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD DEFAULTS" ); }

	// textures
	result += Manager::Make < Texture > ( "logo", "Logo.png" );
	result += Manager::Make < Texture > ( "noise", "noise.png", 1 );
	result += Manager::Make < Texture > ( "coco", "coconut.jpg" );
	result += Manager::Make < Texture > ( RUNNERS_SHEET, RUNNERS_SHEET_PATH ); // runners sprite sheet
	result += Manager::Make < Texture > ( WEAPONS_SHEET, WEAPONS_SHEET_PATH ); // weapons sprite sheet
	result += Manager::Make < Texture > ( OBSTACLES_SHEET, OBSTACLES_SHEET_PATH ); // obstacles sprite sheet
	result += Manager::Make < Texture > ( TILES_SHEET, TILES_SHEET_PATH ); // tiles sprite sheet
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD TEXTURES" ); }
	else { DEBUG ( 4, "SUCCED TO LOAD TEXTURES" ); } 
	// shaders
	result += Manager::Make < Shader > ( NES_CRT_SHADER, NES_CRT_SHADER_VS_PATH, NES_CRT_SHADER_FS_PATH );
	result += Manager::Make < Shader > ( TILEMAP_SHADER, TILEMAP_SHADER_VS_PATH, TILEMAP_SHADER_FS_PATH );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD SHADERS" ); }
	// fonts
	result += Manager::Make < Font > ( AOVEL_SANS_ROUNDED_FONT, AOVEL_SANS_ROUNDED_FONT_PATH, 90 );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD FONT" ); }

	Manager::Objekt_Load ( "Main Camera" );
	Manager::Camera_Bind ( "Camera", "Main Camera" );

	return result;
}

void __FreamBufferResize (GLFWwindow* window, int width, int heigth ) {
	Manager::Objekt_Get ( "FrameBuffer" )
		->Get_Component < Framebuffer > ( )
		->Set ( width, heigth );
}

int main ( )
{
	ReKat::Graphik::Start ( "Font Test", 800, 600, false, false, true );
	ReKat::Graphik::_current_window->input._FreamBufferResize = __FreamBufferResize;
	ReKat::phisiks::Start ( 60 );
	ReKat::phisiks::Set_Active ( "Main menu" );

	load ( );

	DEBUG ( 3, "Resources Loaded" );

	Manager::Objekt_Load ( "Main menu" )->Add_Component ( "Scenario" );

	Manager::Objekt_Load ( "FrameBuffer", {0,0,0}, {1333,1000,100} )->Add_Component < Framebuffer > ( )
		->Set ( 800,600 )
	//	.Set ( NES_CRT_SHADER, "", true )
		.Set ( Manager::Objekt_Get ( "Main menu" ) );


	Manager::Set_Active_Scene ( "FrameBuffer" );	

	Manager::Get < Shader > ( NES_CRT_SHADER )->setInt ( "screenTexture", 0 );
	Manager::Get < Shader > ( NES_CRT_SHADER )->setInt ( "noiseTexture", 1 );
	Manager::Get < Shader > ( NES_CRT_SHADER )->setFloat ( "time", 0 );

	Timer::Update ( );

	while ( ReKat::Graphik::Is_End ( ) )
	{
		ReKat::Graphik::Clear_Screen ( 0.0f );
		Manager::Update ( );

		Manager::Get < Texture > ( "noise" )->Use ( );
		Manager::Get < Shader > ( NES_CRT_SHADER )->setFloat ( "time", Timer::Get_Time ( ) );
		ReKat::phisiks::Update ( );
		ReKat::Graphik::Update ( );
		// Manager::Objekt_Get ( "Main menu" )->Print_Tree ( );
	}

	Manager::Free ( );
	Manager::Free_Objekts ( );
	
}