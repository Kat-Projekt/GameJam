#pragma once
#include <engine.hpp>
#include "Runner_Manager.cpp"

class Enemy : public Behaviour
{
public:
	enum class State {
		Idle,           // Fermo in pausa
		MovingRandom,   // In movimento in una direzione casuale per un tempo t
		ChasingEnemy,    // Inseguimento del nemico (priorità)
		PickingWeapon
	};

private:
	//pointer to runner
	Runner* runner = nullptr;
	Runner_Manager* runner_manager = nullptr;
	/* data */
	float detection_radius = 800.0f;  // Raggio rilevamento nemici
	float weapon_detection_radius = 800.0f;  // Raggio rilevamento armi
	float chasing_weapon_detection_radius = 250.0f;  // Raggio rilevamento armi durante inseguimento

	// Limiti di tempo per il movimento casuale
	const float min_move_time = 1.5f;       // Tempo minimo di camminata
	const float max_move_time = 4.0f;       // Tempo massimo di camminata

	// Limiti di tempo per la pausa
	const float min_pause_time = 1.0f;      // Pausa minima
	const float max_pause_time = 5.0f;      // Pausa massima

	// Timer cooldown
	const float attack_cooldown = 1.0f;      //timer cooldown attack

	// --- STATO ATTUALE ---
	State current_state = State::Idle;
	
	Objekt* enemy_target = nullptr;   // Puntatore al nemico più vicino
	glm::vec3 wander_dir = {0,0,0};   // Vettore direzione casuale (normalizzato)

	float enemy_seek_timer = 0.0f;
	float enemy_seek_timeout = 0.0f;
	const float enemy_seek_max_timer = 5.0f;
	const float enemy_seek_max_timeout = 2.0f;
	const float enemy_seek_pause = 1.0f;

	Objekt* weapon_target = nullptr;

	float weapon_seek_timer = 0.0f;
	float weapon_seek_timeout = 0.0f;
	const float weapon_seek_max_timer = 5.0f;
	const float weapon_seek_max_timeout = 2.0f;
	const float weapon_seek_pause = 0.5f;
	
	float state_timer = 0.0f;         // Timer generico (usato per pause o tempo di camminata)
	float attack_cooldown_timer = 0.0f; //Timer cooldown attacco
	//setup: atttach sprite, rigid body, collider
	//update: target position, target, 

	float Get_Random_Float(float min, float max)
	{
		float scale = (float)rand() / RAND_MAX;
		return min + scale * (max - min);
	}

	float Get_Random_Move()
	{
		return Get_Random_Float(min_move_time, max_move_time);
	}

	float Get_Random_Pause()
	{
		return Get_Random_Float(min_pause_time, max_pause_time);
	}

	void Pick_Random_Direction()
	{
		glm::vec3 dir = { 0.0f, 0.0f, 0.0f };
		float len = 0.0f;

		do 
		{
			float rx = Get_Random_Float(-1.0f, 1.0f);
			float ry = Get_Random_Float(-1.0f, 1.0f);

			dir = { rx, ry, 0.0f };
			len = glm::length(dir);

		} while (len < 0.0001f);

		wander_dir = glm::normalize(dir);
	}

	void Scan_For_Weapon()
	{
		weapon_target = nullptr;
		if (weapon_seek_timeout > 0.0f) return;
		if (!runner || runner->Has_Weapon()) return;
		if (!runner_manager) return;

		float radius = (current_state == State::ChasingEnemy)
			? chasing_weapon_detection_radius
			: weapon_detection_radius;
		float min_distance = radius;
		glm::vec3 my_pos = obj->Get_Pos();

		for (Objekt* w_obj : runner_manager->Get_Weapons())
		{
			if (!w_obj || !w_obj->Get_Active()) continue;

			Weapon* w = w_obj->Get_Component<Weapon>();
			if (!w || !w->IsPickable()) continue;

			float dist = glm::distance(my_pos, w_obj->Get_Pos());
			if (dist < min_distance)
			{
				min_distance = dist;
				weapon_target = w_obj;
			}
		}
	}

	void Scan_For_Targets()
    {
		if (enemy_seek_timeout > 0.0f) 
		{	
			return;
		}

        Objekt* closest_enemy = nullptr;
        float min_distance = detection_radius;

        glm::vec3 my_pos = obj->Get_Pos();

        //checks every runner in the scene to find the closest inside the detection radius
		if (!runner_manager) return;

		for (Objekt* other_obj : runner_manager->Get_Runners())
		{
			if (!other_obj || other_obj == obj || !other_obj->Get_Active()) continue;

			glm::vec3 other_pos = other_obj->Get_Pos();
			float dist = glm::distance(my_pos, other_pos);

			if (dist < min_distance)
			{
				min_distance = dist;
				closest_enemy = other_obj;
			}
		}

		//decides to chase an enemy if one is found, else if the current status is chasing it sets it to idle
        if (closest_enemy)
        {
			if (current_state != State::ChasingEnemy)
			{
				enemy_seek_timer = enemy_seek_max_timer;
			}
            enemy_target = closest_enemy;
            current_state = State::ChasingEnemy; 
        }
        else
        {
            enemy_target = nullptr;
            
            
            if (current_state == State::ChasingEnemy)
            {
                current_state = State::Idle;
                state_timer = Get_Random_Pause();
            }
        }
    }

	void Update_Idle ( float dt) 
	{
		runner->Stay();

		state_timer -= dt;
		if (state_timer <= 0.0f)
		{
			Pick_Random_Direction();
			state_timer = Get_Random_Move();
			current_state = State::MovingRandom;
		}
	}

	void Update_MovingRandom( float dt) 
	{
    		runner->SetDirection(wander_dir);

		state_timer -= dt;
		if (state_timer <= 0.0f)
		{
			state_timer = Get_Random_Pause();
			current_state = State::Idle;
		}
	}

	void Update_ChasingEnemy( float dt )
	{
		if (!enemy_target || !runner) return;


		if ( attack_cooldown_timer > 0.0f )
		{ attack_cooldown_timer -= dt; }


		vec3 to_target = enemy_target->Get_Pos() - obj->Get_Pos();
		float dist = glm::length(to_target);
		DEBUG ( 3, "Distanza attuale dal target: ", dist, " range attuale: ", runner->Get_Attack_Range ( ) );
		
		float attack_range = runner->Get_Attack_Range ( );

		if (dist <= attack_range)
		{
			runner->Stay();
			runner->SetAttackDirection(to_target);

			if (attack_cooldown_timer <= 0.0f)
			{
				DEBUG ( 3, "TENTATIVO SWING NEMICO" );
				runner->Swing();
				attack_cooldown_timer = attack_cooldown;
			}
			return;
		}

		enemy_seek_timer -= dt;
		if (enemy_seek_timer <= 0.0f)
		{
			enemy_target = nullptr;
			enemy_seek_timeout = enemy_seek_max_timeout;
			current_state = State::Idle;
			state_timer = enemy_seek_pause;
			return;
		}

		runner->SetTarget(enemy_target->Get_Pos());
	}

	void Update_PickingWeapon( float dt )
	{
		if (!runner) return;

		runner->PickWeapon();

		if (runner->Has_Weapon() || weapon_target == nullptr)
		{
			weapon_target = nullptr;
			current_state = State::Idle;
			state_timer = Get_Random_Pause();
			return;
		}

		runner->SetTarget(weapon_target->Get_Pos());

		weapon_seek_timer -= dt;
		if (weapon_seek_timer <= 0.0f)
		{
			weapon_target = nullptr;
			weapon_seek_timeout = weapon_seek_max_timeout;
			current_state = State::Idle;
			state_timer = weapon_seek_pause;
		}
	}

public:
	Enemy ( )
	{
		Informations = { "Enemy", 1.0, "The Count down / life manager" };
	}

	void Start() override
	{
		runner = obj->Get_Component<Runner>();
		state_timer = Get_Random_Pause();
		auto runners_obj = Manager::Objekt_Get("Runners");
		if (runners_obj)
		{ runner_manager = runners_obj->Get_Component<Runner_Manager>(); }
	}

	void Update ( ) override
	{
		
		float dt = Timer::Get_Delta ( );

		if ( enemy_seek_timeout > 0.0f )
		{
			enemy_seek_timeout -= dt;
		}
		if ( weapon_seek_timeout > 0.0f )
		{
			weapon_seek_timeout -= dt;
		}

		Scan_For_Weapon ( );

		if ( weapon_target != nullptr && current_state != State::PickingWeapon )
		{
			current_state = State::PickingWeapon;
			weapon_seek_timer = weapon_seek_max_timer;
		}

		if ( current_state != State::PickingWeapon )
		{
			Scan_For_Targets ( );
		}

		switch (current_state)
		{
			case State::Idle:
			Update_Idle(dt);
			break;

			case State::MovingRandom:
			Update_MovingRandom(dt);
			break;

			case State::ChasingEnemy:
			Update_ChasingEnemy(dt);
			break;

			case State::PickingWeapon:
			Update_PickingWeapon(dt);
			break;
		}
		DEBUG ( 3, "stato attuale nemico: ", (int)current_state );
	}
};