#include "tools/ui.h"

#include <SDL.h>

#include "data_structures/vector.h"

#include "tools/utils.h"
#include "tools/pcg_basic.h"

typedef struct Point
{
    double x;
    double y;
    int specie;
} Point;

typedef struct Specie
{
    int id;
    Point centroid;
} Specie;

vector points;
int point_count;
void ui_init()
{
    point_count = 10;
    points = new_vector( sizeof( Point ), point_count, 0 );

    for ( int i = 0; i < point_count; i++ )
    {
        VEC( points, Point, i ) = (Point) { pcg32_boundedrand( UI_WIDTH ), pcg32_boundedrand( UI_HEIGHT ), 0 };
    }

    SDL_RenderClear( ui_renderer );
    SDL_RenderPresent( ui_renderer );

    while ( ui_run() )
		SDL_Delay( 10 );
    

}

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
			case SDL_MOUSEMOTION:
				mx = e.button.x;
				my = e.button.y;
			case SDL_MOUSEBUTTONUP:
				break;
		}
	}

	SDL_SetRenderDrawColor( ui_renderer, 38, 54, 102, 255 );

    for ( int i = 0; i < point_count; i++ )
    {
		// NEXT: take new drawcircle sdl function
        DrawCircle( ui_renderer, (int)VEC(points, Point, i).x, (int)VEC(points, Point, i).y, 5);
    }

	// if ( keys[SDL_SCANCODE_SPACE] ) {
	//     SDL_Rect r = { 100, 100, mx-100, my-100 };
	// 	SDL_RenderDrawPoint( ui_renderer, 10, 10 );
	// 	SDL_RenderDrawLine( ui_renderer, 10, 20, 10, 100 );
	// 	SDL_RenderFillRect( ui_renderer, &r );
	// 	SDL_RenderDrawRect( ui_renderer, &r );
	// }

    if ( keys[SDL_SCANCODE_ESCAPE] )
        return false;

	// Update window
	SDL_RenderPresent( ui_renderer );

    return true;
}

void ui_exit()
{
	free_vector(&points);
}