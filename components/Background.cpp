#include <engine.hpp>
#include "file_refes.h"

class Background : public Behaviour
{
private:
	/* data */
public:
	Background ( )
	{
		Informations = { "Background", 1.0, "This is the weapons information" };
	}

	void Start ( )
	{
		auto Tm = std::make_shared < Tilemap > ( );
		Tm->Set ( TILEMAP_PATH, TILES_SHEET, "", "" );

		auto TmC = std::make_shared < Tilemap_Collider > ( );
		TmC->Set ( TILEMAP_COLLISIONS_PATH );

		obj->Add_Component ( Tm );
		obj->Add_Component ( TmC );
	}
};
