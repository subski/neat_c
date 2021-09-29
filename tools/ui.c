#include "tools/ui.h"

#if USE_SDL

#include <SDL.h>
#include <math.h>

#include "data_structures/vector.h"
#include "data_structures/clist.h"

#include "tools/utils.h"
#include "tools/pcg_basic.h"


vector points;
int point_count = 100;
int specie_count = 3;

clist *speciesList = NULL;
clist *pointList = NULL;


double distance_point( Point *p1, Point *p2 )
{
	return  sqrt( ( p1->x - p2->x ) * ( p1->x - p2->x ) +  ( p1->y - p2->y ) * ( p1->y - p2->y ) );
}

int speciate( clist* pointlist, clist* species )
{
	CY_ITER_DATA( species, specie_node, specie, Specie*,
		cy_clear(&specie->points);
	);

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

void ui_init()
{

    points = new_vector( sizeof( Point ), point_count, 0 );

	for ( int i = 0; i < point_count; i++ )
	{
		VEC( points, Point, i ) = (Point) { UI_WIDTH*pcg32_doublerand(), UI_HEIGHT*pcg32_doublerand() };
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

    SDL_RenderClear( ui_renderer );
    SDL_RenderPresent( ui_renderer );

    while ( ui_run() )
		SDL_Delay( 10 );
}

int step = 0;
int ui_run()
{
	#pragma warning ( disable : 4090 )
    unsigned char* keys = SDL_GetKeyboardState( NULL );

    SDL_Event e;
	// For mouse rectangle (static to presist between function calls)
	static int mx = -1, my = -1;

	// Clear the window to white
	SDL_SetRenderDrawColor( ui_renderer, 56, 138, 214, 255 );
	SDL_RenderClear( ui_renderer );

	// Event loop
	while ( SDL_PollEvent( &e ) != 0 ) {
		switch ( e.type ) {
			case SDL_QUIT:
				return false;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_KEYUP:
				if ( e.key.keysym.sym == SDLK_SPACE)
				{
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

						printf("Changes: %d\n", speciate(pointList, speciesList));
					}
				}
				break;
			case SDL_MOUSEMOTION:
				mx = e.button.x;
				my = e.button.y;
			case SDL_MOUSEBUTTONUP:
				break;
		}
	}

	SDL_SetRenderDrawColor( ui_renderer, 240, 185, 36, 255 );

    CY_ITER_DATA(speciesList, specie_node, specie, Specie*, 
    	CY_ITER_DATA(specie->points, point_node, point, Point*, 
			SDL_SetRenderDrawColor( ui_renderer, specie->color[0], specie->color[1], specie->color[2], 255 );
        	DrawCircle( ui_renderer, point->x, point->y, 10);
		);
		SDL_SetRenderDrawColor( ui_renderer, specie->color[0], specie->color[1], specie->color[2], 255 );
		DrawCircle( ui_renderer, specie->centroid.x, specie->centroid.y, 30);
	);

	// if ( keys[SDL_SCANCODE_SPACE] ) {
	//     SDL_Rect r = { 100, 100, mx-100, my-100 };
	// 	SDL_RenderDrawPoint( ui_renderer, 10, 10 );
	// 	SDL_RenderDrawLine( ui_renderer, 10, 20, 10, 100 );
	// 	SDL_RenderFillRect( ui_renderer, &r );
	// 	SDL_RenderDrawRect( ui_renderer, &r );
	// }

    if ( keys[SDL_SCANCODE_ESCAPE])
        return false;

	// Update window
	SDL_RenderPresent( ui_renderer );

    return true;
}

void ui_exit()
{
	CY_ITER_DATA(speciesList, node, specie, Specie*,
		cy_clear(&specie->points);
	);
	cy_clean(&speciesList);
	cy_clear(&pointList);

	free_vector(&points);
}

#endif // !USE_SDL