#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Game.h"

Game::Game() : start(0), last(0), current(0), good(true), running(false), 
        width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT), 
        particles(std::vector<ParticleT>())
{
    // Seed the random number generator
    srand(time(0));

    // Fill calParticles array
    //for( size_t i; i < PARTICLE_COUNT; i++ ) std::fill( &calParticles[i], &(calParticles[i][PARTICLE_COUNT]), false );
    
    // initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        good = false;
        return;
    }
    
    // initialize SDL window
    window = SDL_CreateWindow("Gravity", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        good = false;
        return;
    }
    
    // initialize SDL renderer
    renderer = SDL_CreateRenderer(window, -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        good = false;
        return;
    }
    
    // initialize particle texture
    SDL_Surface* bmp = SDL_LoadBMP("particle.bmp");
    if (bmp == NULL)
    {
        good = false;
        return;
    }
    particleTexture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (particleTexture == NULL)
    {
        good = false;
        return;
    }
    
    // initialize our particles
    for (int i = 0; i < PARTICLE_COUNT; ++i)
    {
        particles.push_back(randomParticle());
    }
}

Game::~Game()
{
    if (!good)
    {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }
    if (particleTexture != NULL)
    {
        SDL_DestroyTexture(particleTexture);
    }
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

int Game::operator()()
{
    if (!good)
    {
        return -1;
    }
    running = true;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    start = SDL_GetTicks();
    last = start;
    while (running) // every iteration is 1 frame
    {
        current = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            handleEvent(event);
        }
        update((current - last) / 1000.0);
        render();
        last = current;
    }
    return 0;
}

void Game::update(double dt)
{
    Gravitation<double> Gravity;

    double* posX1 = nullptr;
    double* posY1 = nullptr;
    VelocityT* vel1 = nullptr;

    double* posX2 = nullptr;
    double* posY2 = nullptr;
    VelocityT* vel2 = nullptr;

    //std::cout << dt << " sec since last frame.\n";
    for( VectorT::iterator iter = particles.begin();
         iter != particles.end();
         ++iter ){

        posX1 = &( iter->getPos().getX() );
        posY1 = &( iter->getPos().getY() );
        vel1 = &( iter->getVelocity() );

        // Check if the particle is near another particle
        for( VectorT::iterator iter2 = particles.begin();
             iter2 != particles.end();
             ++iter2 ){

            if( iter != iter2 ){
                Gravity( (*iter), (*iter2) );

                posX2 = &( iter2->getPos().getX() );
                posY2 = &( iter2->getPos().getY() );
                vel2 = &( iter2->getVelocity() );

                *posX1 += Gravity.getAccel1() * std::cos( Gravity.getAngle1() * (3.14 / 180.0) );
                *posY1 += Gravity.getAccel1() * std::sin( Gravity.getAngle1() * (3.14 / 180.0) );

                *posX2 += Gravity.getAccel2() * std::cos( Gravity.getAngle2() * (3.14 / 180.0) );
                *posY2 += Gravity.getAccel2() * std::sin( Gravity.getAngle2() * (3.14 / 180.0) );
            }
        }

        // Move the particle according to its velocity.
        *posX1 += vel1->getXComponent() * dt;
        *posY1 -= vel1->getYComponent() * dt;

        // Check if the particle is near the end of the window
        if( *posX1 >= DEFAULT_WIDTH || *posX1 <= 0 ){
            vel1->setAngle( 180 - vel1->getAngle() );
        }
        if ( *posY1 >= DEFAULT_HEIGHT || *posY1 <= 0 ){
            vel1->setAngle( -vel1->getAngle() );
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    
    // rendering here would place objects beneath the particles
    
    for (const ParticleT& p : particles)
    {
        drawParticle(p);
    }
    
    // rendering here would place objects on top of the particles
    
    SDL_RenderPresent(renderer);
}

void Game::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    // Add your own event handling here if desired
    case SDL_QUIT:
        running = false;
        break;
    default:
        break;
    }
}

void Game::drawParticle(const ParticleT& p)
{
    SDL_Rect dst;
    double shift = p.getRadius();
    dst.x = (int) (p.getPos().getX() - shift);
    dst.y = (int) (p.getPos().getY() - shift);
    dst.w = shift * 2;
    dst.h = shift * 2;
    SDL_RenderCopy(renderer, particleTexture, NULL, &dst);
}

Game::ParticleT Game::randomParticle() const
{
    PointT pos(rand() % width, rand() % height);
    VelocityT vel( rand() % 100 + 20, rand() );
    // using some percentage of the mass of Jupiter
    //double mass = ((double) rand() / RAND_MAX) * 1.9e27 + 1.0;
    //double mass = 50;

    /*PointT pos( DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2 );
    VelocityT vel( 5, 90 );
    double mass = 10;*/
    
    return ParticleT(pos, rand() % 300 + 20, vel);
}
