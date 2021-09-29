#include "tools/ui.h"

#if USE_SDL

#include <SDL.h>

#include "tools/utils.h"
#include "tools/pcg_basic.h"

void ui_init()
{

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
			case SDL_KEYUP:
				if ( e.key.keysym.sym == SDLK_SPACE)
				{
					
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

    // CY_ITER_DATA(speciesList, specie_node, specie, Specie*, 
    // 	CY_ITER_DATA(specie->points, point_node, point, Point*, 
	// 		SDL_SetRenderDrawColor( ui_renderer, specie->color[0], specie->color[1], specie->color[2], 255 );
    //     	DrawCircle( ui_renderer, point->x, point->y, 10);
	// 	);
	// 	SDL_SetRenderDrawColor( ui_renderer, specie->color[0], specie->color[1], specie->color[2], 255 );
	// 	DrawCircle( ui_renderer, specie->centroid.x, specie->centroid.y, 30);
	// );

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

}

#endif // !USE_SDL