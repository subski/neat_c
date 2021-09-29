#include "neurolution/kmeans.h"

#include <math.h>

#include "data_structures/vector.h"
#include "data_structures/clist.h"

#include "tools/utils.h"
#include "tools/pcg_basic.h"

vector points;
int point_count = 9;
int specie_count = 3;

clist *speciesList = NULL;
clist *pointList = NULL;


void kmeans_init()
{
    points = new_vector( sizeof( Point ), point_count, 0 );

	VEC( points, Point, 1 ) = (Point) { 50, 30 };
	VEC( points, Point, 2 ) = (Point) { 100, 40 };
	VEC( points, Point, 3 ) = (Point) { 20, 10 };

	VEC( points, Point, 4 ) = (Point) { 450, 250 };
	VEC( points, Point, 5 ) = (Point) { 420, 250 };
	VEC( points, Point, 6 ) = (Point) { 460, 270 };

	VEC( points, Point, 7 ) = (Point) { 850, 450 };
	VEC( points, Point, 8 ) = (Point) { 800, 450 };
	VEC( points, Point, 0 ) = (Point) { 835, 425 };



	for ( int i = 0; i < point_count; i++ )
	{
		// VEC( points, Point, i ) = (Point) { UI_WIDTH*pcg32_doublerand(), UI_HEIGHT*pcg32_doublerand() };
		cy_insert( &pointList, &VEC( points, Point, i ) );
	}

	Point* c;
	Specie* s;
	for ( int i = 0; i < specie_count; i++ )
	{
		c = cy_random_data( pointList );
		s = malloc( sizeof( Specie ) );
		s->centroid = *c;
		s->points = NULL;
		s->id = i;
		memcpy( s->color, (int[3]) { pcg32_boundedrand(255), pcg32_boundedrand(255), 255 }, sizeof(int) * 3 );
		cy_insert( &s->points, c );
		cy_insert( &speciesList, s );
		cy_remove( &pointList, c );
	}

	speciate(pointList, speciesList);

	cy_clear(&pointList);
	for ( int i = 0; i < point_count; i++ )
	{
		cy_insert( &pointList, &VEC( points, Point, i ) );
	}

	int count_point = 0;
	CY_ITER_DATA( speciesList, specie_node, specie, Specie*,
		printf("S%d: %d\n", specie->id, cy_len(specie->points));
		count_point += cy_len(specie->points);
	);
	printf("Total point speciated: %d\n", count_point);
}

void kmeans_run()
{
    int step = 0;
    if (step % 2 == 0)
    {
        step++;
        CY_ITER_DATA(speciesList, specie_node, specie, Specie*,

            specie->centroid.x = 0;
            specie->centroid.y = 0;

            CY_ITER_DATA(specie->points, point_node, point, Point*,
                specie->centroid.x += point->x;
                specie->centroid.y += point->y;
            );

            double s_count = cy_len(specie->points);
            specie->centroid.x /= s_count;
            specie->centroid.y /= s_count;
        );
    }
    else
    {
        step++;
        CY_ITER_DATA( speciesList, specie_node, specie, Specie*,
            printf("S%d centroid: (%lf, %lf)\n", specie->id, specie->centroid.x, specie->centroid.y);
            cy_clear(&specie->points);
        );

        printf("Changes: %d\n", speciate(pointList, speciesList));
    }
}

void kmeans_end()
{
    CY_ITER_DATA(speciesList, node, specie, Specie*,
		cy_clear(&specie->points);
	);
	cy_clean(&speciesList);
	cy_clear(&pointList);

	free_vector(&points);
}

double distance_point( Point *p1, Point *p2 )
{
	return  sqrt( ( p1->x - p2->x ) * ( p1->x - p2->x ) +  ( p1->y - p2->y ) * ( p1->y - p2->y ) );
}

int speciate( clist* pointlist, clist* species )
{
	int changes = 0;
	Specie* nearest_specie;
	double nearest_specie_distance = 0;
	double target_specie_distance;
	CY_ITER_DATA( pointlist, point_node, point, Point*,
		nearest_specie_distance = 0;

		clist* specie_node = species;			
        Specie* specie;					
        do
        {							
            specie = (Specie*) specie_node->data;

			target_specie_distance = distance_point( &specie->centroid, point );
			if ( target_specie_distance < nearest_specie_distance || nearest_specie_distance == 0 )
			{
				nearest_specie = specie;
				nearest_specie_distance = target_specie_distance;
			}

            next(specie_node);				
        } while (specie_node != species);

		if (point->specie != nearest_specie->id)
		{
			changes++;
			point->specie = nearest_specie->id;
		}

		cy_insert(&nearest_specie->points, point);
	);

	return changes;
}