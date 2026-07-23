#include <engine.hpp>

#include "Chat.cpp"
#include "Beater.cpp"

enum class Donation_event
{
	KILL,
	DOUBLE_KILL,
	TRIPPLE_KILL,
	KILLED_JONH,
	EMPTY_EVENT = 0
};

struct Donator
{
	std::unordered_map < std::string, int > affinities;
	std::string name;

	void Increase_Affinity ( std::string runner, int value )
	{
		if ( affinities.find ( runner ) == affinities.end ( ) )
		{ 
			affinities [ runner ] = value;
		}
		else
		{
			affinities [ runner ] = value * value / affinities [ runner ];
		}
	}
};

struct Donation
{
	Donation_event trigger = Donation_event::EMPTY_EVENT;
	std::shared_ptr < Donator > donator = nullptr;
	std::vector < std::string > mesages;
	int value = 0;

	float CalculateTime ( ) { return 1; }
	std::string GetMessage ( ) { return mesages[0]; }

	Donation (
		std::shared_ptr < Donator > _donator,
		std::vector < std::string > _mesages,
		int _value,
		Donation_event _trigger
	) : trigger(_trigger), donator(_donator), mesages(_mesages), value(_value)
	{ }

	Donation ( ) { }
};

class Donator_Manager : public Behaviour
{
private:
	// name and pointer
	std::unordered_map < std::string, std::shared_ptr < Donator > > donators;
	std::vector < Donation > donations;

	struct Donation_resolution {
		Donation* dono;
		float time;
		std::string player;

		Donation_resolution (
			Donation* _dono,
			const std::string &_player
		) : dono(_dono), time(_dono->CalculateTime ( )), player(_player)
		{ }

		Donation_resolution ( ) { }
	};
	std::vector <  Donation_resolution > resolution_queue;
public:
	Donator_Manager ( )
	{
		Informations = { "Donator_Manager", 1.0, "This is the single donator manager, it describes how a donor behaves" };
	}

	void Update ( )
	{
		for ( auto dono : resolution_queue )
		{
			if ( Timer::Get_Time_d ( ) > dono.time )
			{
				obj->Get_Component < Chat > ( )->Post 
				( dono.dono->donator->name, dono.dono->GetMessage ( ), true );

				dono.dono->donator->Increase_Affinity ( dono.player, dono.dono->value );

				Manager::Objekt_Get ( dono.player )
					->Get_Component < Beater > ( )
					->Reward ( dono.dono->value );
			}
		}
	}

	void Post ( std::string player, Donation_event event )
	{
		for ( auto dono : donations )
		{
			if ( dono.trigger == event )
			{
				resolution_queue.push_back ( { &dono,player } );
			}
		}
	}

	void New_Donator ( std::string name )
	{
		donators [ name ] = std::make_shared < Donator > ( );
	}

	void New_Objective (
		std::string donor,
		std::vector < std::string > messagges,
		int value,
		Donation_event trigger
	) {
		donations.push_back ( { donators[donor], messagges, value, trigger } );
	}

};