#include <engine.hpp>

class Runner : public Behaviour
{
public:
	enum class State {
        Idle,           // Fermo in pausa
        MovingRandom,   // In movimento in una direzione casuale per un tempo t
        ChasingEnemy    // Inseguimento del nemico (priorità)
    };

	Runner ( )
	{
		Informations = { "Runner", 1.0, "The ai for the enemy runners" };
	}
private:
	/* data */
	// --- PARAMETRI DI CONFIGURAZIONE ---
    float move_speed = 100.0f;        // Velocità di movimento
    float detection_radius = 200.0f;  // Raggio rilevamento nemici

    // Limiti di tempo per il movimento casuale
    float min_move_time = 1.5f;       // Tempo minimo di camminata
    float max_move_time = 4.0f;       // Tempo massimo di camminata

    // Limiti di tempo per la pausa
    float min_pause_time = 1.0f;      // Pausa minima
    float max_pause_time = 3.0f;      // Pausa massima

    // --- STATO ATTUALE ---
    State current_state = State::Idle;
    
    Objekt* enemy_target = nullptr;   // Puntatore al nemico più vicino
    glm::vec3 wander_dir = {0,0,0};   // Vettore direzione casuale (normalizzato)
    
    float state_timer = 0.0f;         // Timer generico (usato per pause o tempo di camminata)
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

	void Scan_For_Targets()
    {
        Objekt* closest_enemy = nullptr;
        float min_distance = detection_radius;

        glm::vec3 my_pos = obj->Get_Pos();

        //checks every runner in the scene to find the closest inside the detection radius
        auto active_scene = Manager::Objekt_Get("Main scene");
		if (!active_scene) return;

		auto runners = active_scene->Get_Components_Recursive<Runner>();

		for (Runner* current_candidate_runner : runners)
		{
			if (!current_candidate_runner) continue;

			Objekt* other_obj = current_candidate_runner->obj; // L'oggetto a cui è attaccato il Runner

			// Ignoriamo se stessi o corridori non attivi
			if (!other_obj || other_obj == obj || !other_obj->Get_Active()) continue;

			glm::vec3 other_pos = other_obj->Get_Pos();
			float dist = glm::distance(my_pos, other_pos);

			if (dist < min_distance)
			{
				min_distance = dist;
				closest_enemy = other_obj;
			}
		}

		//checks wether player is the closest target inside the radius
		auto player_ptr = Manager::Objekt_Get("Player");
        if (player_ptr)
        {
            Objekt* player = player_ptr.get();
            if (player && player->Get_Active())
            {
                float dist = glm::distance(my_pos, player->Get_Pos());
                if (dist < min_distance)
                {
                    min_distance = dist;
                    closest_enemy = player;
                }
            }
        }

		//decides to chase an enemy if one is found, else if the current status is chasing it sets it to idle
        if (closest_enemy)
        {
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

	void Update_Idle(int dt) 
	{
		state_timer -= dt;
		if (state_timer <= 0.0f)
		{
			Pick_Random_Direction();
			state_timer = Get_Random_Move();
			current_state = State::MovingRandom;
		}
	}

	void Update_MovingRandom(int dt) 
	{
		glm::vec3 displacement = wander_dir * move_speed * dt;
    	obj->Inc_Pos(displacement);

		state_timer -= dt;
		if (state_timer <= 0.0f)
		{
			state_timer = Get_Random_Pause();
			current_state = State::Idle;
		}
	}
	void Update_ChasingEnemy(float dt)
	{
		// 1. Verifichiamo che il bersaglio sia ancora un puntatore valido
		if (!enemy_target) return;

		glm::vec3 my_pos = obj->Get_Pos();
		glm::vec3 target_pos = enemy_target->Get_Pos();

		// 2. Calcoliamo il vettore direzione verso il nemico
		glm::vec3 chase_dir = target_pos - my_pos;
		float dist = glm::length(chase_dir);

		// 3. Se non siamo esattamente sopra al bersaglio, ci muoviamo verso di lui
		if (dist > 0.0001f)
		{
			chase_dir = glm::normalize(chase_dir);
			glm::vec3 displacement = chase_dir * move_speed * dt;
			obj->Inc_Pos(displacement);
		}
	}

public:


	void Start() override
    {
        state_timer = Get_Random_Pause();

    }

    void Update() override
    {
       Scan_For_Targets()

	   float dt = Graphik::Manager::Get_Delta_Time();

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
    }
    }
	}

	void Start ( )
	{

	}

	void Update ( )
	{
		
	}
};