#include "neurolution/kmeans.h"

#include <math.h>

#include "neurolution/neurolution.h"
#include "neurolution/agent.h"

#include "data_structures/vector.h"
#include "data_structures/clist.h"
#include "data_structures/pool.h"

#include "tools/utils.h"
#include "tools/pcg_basic.h"

void kmeans_init(clist* datalist, clist* specieslist, int species_count)
{
	Agent* agent_centroid;
	Specie* specie;
    clist *data = cy_clone(datalist);
	for ( int i = 0; i < species_count; i++ )
	{
		agent_centroid = cy_random_data( data );
		specie = malloc( sizeof( Specie ) );
		specie->centroid = request( &P_AGENT, sizeof( Agent ) );
        memcpy(specie->centroid, agent_centroid, sizeof( Agent ) );
		specie->specimens = NULL;
		specie->id = i;
		cy_insert( &specie->specimens, agent_centroid );
		cy_insert( &specieslist, specie );
		cy_remove( &data, c );
	}

	speciate(data, specieslist);

	int count_point = 0;
	CY_ITER_DATA( specieslist, specie_node, specie, Specie*,
		printf("S°%d: %d\n", specie->id, cy_len(specie->specimens));
		count_point += cy_len(specie->specimens);
	);
	printf("Total specimens speciated: %d\n", count_point);
}

void kmeans_run(clist* datalist, clist* specieslist)
{
    int changes = 0;
    do
    {
        CY_ITER_DATA(specieslist, specie_node, specie, Specie*,
            // NEXT: calculate new centroid
            // specie->centroid.x = 0;
            // specie->centroid.y = 0;
            // CY_ITER_DATA(specie->specimens, agent_node, agent, Agent*,
            //     specie->centroid.x += point->x;
            //     specie->centroid.y += point->y;
            // );
            // double s_count = cy_len(specie->points);
            // specie->centroid.x /= s_count;
            // specie->centroid.y /= s_count;

        );
        

        CY_ITER_DATA( specieslist, specie_node, specie, Specie*,
            cy_clear(&specie->specimens);
        );

        changes = speciate(datalist, specieslist);
    } while (changes != 0);
}

double distance_point( Point *p1, Point *p2 )
{
	return  sqrt( ( p1->x - p2->x ) * ( p1->x - p2->x ) +  ( p1->y - p2->y ) * ( p1->y - p2->y ) );
}

int speciate( clist* datalist, clist* species )
{
	int changes = 0;
	Specie* nearest_specie;
	double nearest_specie_distance = 0;
	double target_specie_distance;
	CY_ITER_DATA( datalist, agent_node, agent, Agent*,
		nearest_specie_distance = 0;

		clist* specie_node = species;			
        Specie* specie;					
        do
        {							
            specie = (Specie*) specie_node->data;

			target_specie_distance = distance( specie->centroid, agent, 1.0, 1.0 );
			if ( target_specie_distance < nearest_specie_distance || nearest_specie_distance == 0 )
			{
				nearest_specie = specie;
				nearest_specie_distance = target_specie_distance;
			}

            next(specie_node);				
        } while (specie_node != species);

		if (agent->specie != nearest_specie->id)
		{
			changes++;
			agent->specie = nearest_specie->id;
		}

		cy_insert(&nearest_specie->specimens, agent);
	);

	return changes;
}