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

	void Start ( );
};