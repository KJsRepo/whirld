/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: keith
 *
 * Created on April 10, 2018, 7:05 AM
 */

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>
#include <vector>
#include <thread>
#include <cmath>
#include <Perlin/PerlinNoise.h>

using namespace std;
using namespace std::chrono;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;
const int HEIGHT_MAX = 255;
 
const float ABSORPTION_RATE = 0.005;
const float SURFACE_EVAPORATION_RATE = 0.00005;
const float GROUND_EVAPORATION_RATE = 0.001;
const float WATER_PERMEATE_RATE = 0.5;
const float WATER_FLOW_RATE = 0.05;
const float RAINFALL_RATE = 0.012;
const float EROSION_RATE = 0.1;
const float EROSION_FORCE_MODIFIER = 200;
const float VEGETATION_GROWTH_RATE = 1;

int frame = 0;
int updates = 0;
float worldTotalWater = 0;

class Tile {
     
    public:

        Tile() {
            this->xPos = xPos;
            this->yPos = yPos;
            this->zPos = zPos;
            this->rVal = rVal;
        }

        void Initialize(int xPos, int yPos, float zPos) {
            this->xPos = xPos;
            this->yPos = yPos;
            this->zPos = zPos;
            
            if(xPos > 0 && xPos < SCREEN_WIDTH && yPos > 0 && yPos < SCREEN_HEIGHT) {
                this->zPos = zPos;
            } else {
                this->zPos = 255;
            }
            
            this->saturation = 0.0;
            this->surfaceWater = 0.0;
        }
        
        
        void calculateSunlight(float neHeight) {

            // Calculate lighting
            float ratioAngleTowardsSun = this->zPos - neHeight;
            this->sunlight = ratioAngleTowardsSun;
            
        }
                
        void calculateColor() {
            
            float heightCol = (((float)this->zPos / HEIGHT_MAX) * 255);
            if(heightCol > 255) heightCol = 255;
            
            //  Standard color is brown
            this->rVal = heightCol * 140 / 255;
            this->gVal = heightCol * 70 / 255;
            this->bVal = heightCol * 20 / 255;
            
        }
        
        float getSurfaceWater() {
            return this->surfaceWater;
        }
        
        void setSurfaceWater(float val) {
            if(val < 0) val = 0;
            this->surfaceWater = val;
        }

        void setSaturation(float val) {
            if(val > 1) val = 1;
            if(val < 0) val = 0;
            this->saturation = val;
        }

        float getSaturation() {
            return this->saturation;
        }
        
        float getAbsoluteWaterLevel() {
            return this->zPos + this->surfaceWater;
        }
        
        float getSunlight() {
            return this->sunlight;
        }
        
        void setRedVal(int val) {
            if(val > 255) val = 255;
            if(val < 0) val = 0;
            this->rVal = val;
        }

        int getRedVal() {
            return this->rVal;
        }
        
        void setGreenVal(int val) {
            if(val > 255) val = 255;
            if(val < 0) val = 0;
            this->gVal = val;
        }

        int getGreenVal() {
            return this->gVal;
        }
        
        void setBlueVal(int val) {
            if(val > 255) val = 255;
            if(val < 0) val = 0;
            this->bVal = val;
        }

        int getBlueVal() {
            return this->bVal;
        }
        
        float getZPos() {
            return this->zPos;
        }
        
        void decreateZ(float val) {
            this->zPos -= val;
        }
        
        void increateZ(float val) {
            this->zPos += val;
        }
        
    private:
        int rVal;
        int gVal;
        int bVal;
        int xPos;
        int yPos;
        float zPos;
        float sunlight;
        float surfaceWater;
        float saturation;
        
};


class World {
    
    private:
        
        int sizeX;
        int sizeY;
        bool isRaining;
        Tile **tiles;
        vector <unsigned char> pixelData;
        
        void create_landscape() {
            
            //  Initialize the 2D Tile array
            this->tiles = new Tile*[this->sizeY];
            for(int y = 0; y < this->sizeY; y++) {
                this->tiles[y] = new Tile[sizeX];
            }
            
            PerlinNoise perlin;
            float mult = 0.02;
            float multlarge = 0.003;
            float multtiny = 0.05;
            
            for(int y = 0; y < this->sizeY; y++) {
                for(int x = 0; x < this->sizeX; x++) {
                    float pn = perlin.noise(x*mult, y*mult, 0);
                    float pnlarge = perlin.noise(x*multlarge, y*multlarge, 0);
                    float pntiny = perlin.noise(x*multtiny, y*multtiny, 0);
                    
                    float zPos = (((pn + pnlarge) / 2) * HEIGHT_MAX) + (pntiny * 5);
                    
                    this->tiles[x][y].Initialize(x, y, zPos);
                    
                }    
            }

            for(int y = 1; y < this->sizeY; y++) {
                for(int x = 0; x < this->sizeX - 1; x++) {            
                    this->tiles[x][y].calculateSunlight(this->tiles[x+1][y-1].getZPos());
                    if(x< 10 && y == 2) cout << this->tiles[x][y].getZPos() << " mine " << this->tiles[x+1][y-1].getZPos() << " sunlight "  << this->tiles[x][y].getSunlight() << endl;
                    this->tiles[x][y].calculateColor();
                }
            }
        }        
        
    public:
        
        World(int sizeX, int sizeY) : pixelData(sizeX * sizeY * 4, 0) {
            this->sizeX = sizeX;
            this->sizeY = sizeY;
            
            create_landscape();
            
        }
        
        void seedTile(int x, int y, int redVal) {
            this->tiles[x][y].setRedVal(redVal);
        }
        
        void getTileData(int x, int y) {
            cout << "X:" << x << 
                    "    Y:" << y << 
                    "    Z:" << this->tiles[x][y].getZPos() << 
                    "    R:" << this->tiles[x][y].getRedVal() << 
                    "    G:" << this->tiles[x][y].getGreenVal() << 
                    "    B:" << this->tiles[x][y].getBlueVal() << 
                    "    SurfaceWater:" << this->tiles[x][y].getSurfaceWater() << 
                    "    Water Level:" << this->tiles[x][y].getAbsoluteWaterLevel() <<
                    "    Saturation:" << this->tiles[x][y].getSaturation() << 
                    "    Sunlight:" << this->tiles[x][y].getSunlight() << endl;
        }
        
        void setRaining(bool val) {
            this->isRaining = val;
        }

        void update() {
            
            if(this->isRaining == true) {
                for(int y = 2; y < (this->sizeY - 1); y++) {
                    for(int x = 2; x < (this->sizeX - 1); x++) {
                        if(rand() % 2 == 1) this->tiles[x][y].setSurfaceWater(this->tiles[x][y].getSurfaceWater() + RAINFALL_RATE);
                    }
                }
            }
            
            //  Create tile buffer to read
            Tile tileBuffer[this->sizeX][this->sizeY];
            for(int y = 0; y < this->sizeY; y++) {
                for(int x = 0; x < this->sizeX; x++) {
                    tileBuffer[x][y] = this->tiles[x][y];
                }
            }
            

            
            worldTotalWater = 0;
            for(int offset = 0; offset <= 1; offset++) {
                for(int y = 2; y < (this->sizeY - 1); y++) {
                    for(int x = 2; x < (this->sizeX - 1); x++) {

                        //  Only process odd coords on odd frames, even on even
                        if((updates + offset) % 2 == x % 2 && (updates + offset) % 2 == y % 2) {
                            continue;
                        }


                        float surfaceWater = this->tiles[x][y].getSurfaceWater();
                        float saturation = this->tiles[x][y].getSaturation();
                        int terrainHeight = this->tiles[x][y].getZPos();


                        if(surfaceWater > 0) {
                            surfaceWater -= SURFACE_EVAPORATION_RATE;
                        } else {
                            saturation -= GROUND_EVAPORATION_RATE;
                        }

                        if(saturation < 1) {
                            if(surfaceWater < ABSORPTION_RATE) {
                                saturation += surfaceWater;
                                surfaceWater = 0;
                            } else {
                                saturation += ABSORPTION_RATE;
                                surfaceWater -= ABSORPTION_RATE;
                            }
                        }

                        float absWaterLevel = this->tiles[x][y].getZPos() + surfaceWater;

                        //  Process 5 random adjacent tiles
                        for(int i = 0; i < 5; i++) {
                            int rnd = rand() % 9;
                            int curx = (rnd % 3) - 1;
                            int cury = (rnd / 3) - 1;

                            if(curx == 0 && cury == 0) {
                                continue;
                            }
                            
                            float saturationAtPoint = this->tiles[x + curx][y + cury].getSaturation();                        
                            float waterAtPoint = this->tiles[x + curx][y + cury].getSurfaceWater();
                            float waterLevelAtPoint = this->tiles[x + curx][y + cury].getZPos() + waterAtPoint;
                            float terrainHeightAtPoint = this->tiles[x + curx][y + cury].getZPos();
                            float terrainDelta = terrainHeight - terrainHeightAtPoint;

                            if(waterLevelAtPoint < absWaterLevel) {    
                                //  Move half of the difference between each water level
                                float waterToMove = (absWaterLevel - waterLevelAtPoint) * WATER_FLOW_RATE;
                                if(waterToMove < surfaceWater) {
                                    this->tiles[x + curx][y + cury].setSurfaceWater(waterAtPoint + waterToMove);
                                    this->tiles[x][y].setSurfaceWater(surfaceWater - waterToMove);
                                } else {
                                    this->tiles[x + curx][y + cury].setSurfaceWater(waterAtPoint + surfaceWater);
                                    this->tiles[x][y].setSurfaceWater(0);
                                }
                                
                                float erosionForce = waterToMove * EROSION_RATE;
                                erosionForce = erosionForce + (1 + (EROSION_FORCE_MODIFIER * (erosionForce - 0.012)));
                                
                                this->tiles[x + curx][y + cury].increateZ(erosionForce);
                                this->tiles[x][y].decreateZ(erosionForce);

                                surfaceWater = this->tiles[x][y].getSurfaceWater();

                            }

                            if(saturation > saturationAtPoint) {
                                float waterToPermeate = (saturation - saturationAtPoint) * WATER_PERMEATE_RATE * (1 + ((0.5 * terrainDelta) * 0.2)); // TODO:  Change 0.1 to non static
                                this->tiles[x + curx][y + cury].setSaturation(saturationAtPoint + waterToPermeate);
                                this->tiles[x][y].setSaturation(saturation - waterToPermeate);
                                saturation = this->tiles[x][y].getSaturation();
                            }

                        }

                        this->tiles[x][y].setSaturation(saturation);
                        this->tiles[x][y].setSurfaceWater(surfaceWater);
                        worldTotalWater += surfaceWater;

                        //  Every 10th frame, update lighting
                        if(frame % 10 == 2) {
                            this->tiles[x][y].calculateSunlight(this->tiles[x+1][y-1].getZPos());
                            this->tiles[x][y].calculateColor();
                        }
                    }
                }
                
            }

        }
        
        void draw(SDL_Renderer *renderer, SDL_Texture *texture) {
            for(int y = 0; y < this->sizeY; y++) {
                for(int x = 0; x < this->sizeX; x++) {
                    
                    unsigned int offset = (this->sizeX * 4 * y ) + x * 4;
                    
                    
                    float saturation = this->tiles[x][y].getSaturation();
                    
                    int newR = this->tiles[x][y].getRedVal() * this->tiles[x][y].getSunlight();
                    int newG = this->tiles[x][y].getGreenVal() * this->tiles[x][y].getSunlight();
                    int newB = this->tiles[x][y].getBlueVal() * this->tiles[x][y].getSunlight();
                    

                    
                    if(this->tiles[x][y].getSaturation() > 0) {    
                        newG = newG + (saturation * 100);
                        
                        if(newG > 130) newG = 130;
                    }
                    
                    float surfaceWater = this->tiles[x][y].getSurfaceWater();
                    if(surfaceWater > 0) {
                        if(surfaceWater > 1) surfaceWater = 1;
                        newB = floor(float(newB) + (this->tiles[x][y].getSurfaceWater() * 255) + 80);
                    }
                    
                    
                    if(newR > 255) newR = 255;
                    if(newR < 0) newR = 0;
                    
                    if(newG > 255) newG = 255;
                    if(newG < 0) newG = 0;
                    
                    if(newB > 255) newB = 255;
                    if(newB < 0) newB = 0;
                    
                    this->pixelData[offset + 2] = newR;
                    this->pixelData[offset + 1] = newG;
                    this->pixelData[offset + 0] = newB;
                    this->pixelData[offset + 3] = SDL_ALPHA_OPAQUE;
                }
            }
            
            SDL_UpdateTexture(texture, NULL, &this->pixelData[0], this->sizeX * 4 );
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
};

int main(int argc, char** argv) {
    
    SDL_Event event;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    //Start SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
    } else {
        //Create window
        window = SDL_CreateWindow( "whirld", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        } else {
            renderer = SDL_CreateRenderer(window, 2, SDL_RENDERER_ACCELERATED);
            if( renderer == NULL ) {
                printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
            } else {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
            }
            
            
            //Update the surface
            SDL_RenderPresent(renderer);
        }
        
    }    
    
    SDL_RendererInfo info;
    SDL_GetRendererInfo( renderer, &info );
    cout << "Renderer name: " << info.name << endl;
    cout << "Texture formats: " << endl;
    for( Uint32 i = 0; i < info.num_texture_formats; i++ )
    {
        cout << SDL_GetPixelFormatName( info.texture_formats[i] ) << endl;
    }
    
    
    SDL_Texture* texture = SDL_CreateTexture
        (
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT
        );
    
    World whirld(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    std::chrono::high_resolution_clock::time_point t1 = high_resolution_clock::now();
    cout << "starting" << endl;
    double avgFrameTime;
    double time_since_draw = 0;
    double time_since_update = 0;
    
    while (true) {
        const Uint64 start = SDL_GetPerformanceCounter();
                
        if(time_since_update > 50) {
            if(worldTotalWater < 5000) {
                whirld.setRaining(true);
            } else if (worldTotalWater > 100000) {
                whirld.setRaining(false);
            }
            
            whirld.update();
            time_since_update = 0;
            updates++;  
        }
        
        if(time_since_draw > 16) {
            whirld.draw(renderer, texture);
            time_since_draw = 0;
            frame++;
        }


        
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if (SDL_PollEvent(&event)) {
            
            if(event.type == SDL_QUIT)
                break;
            
            if(event.type == SDL_MOUSEBUTTONDOWN) {
                for(int i = 0; i < 10; i++) {
                    int x = event.motion.x + i;
                    int y = event.motion.y;
                    whirld.getTileData(x, y);
                }
            }
        }
        
        const Uint64 end = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = ( end - start ) / static_cast< double >( freq );
        //cout << "Frame time: " << seconds * 1000.0 << "ms" << "     World Water Total:" << worldTotalWater << endl;
        time_since_draw += (seconds * 1000.0);
        time_since_update += (seconds * 1000.0);
        avgFrameTime += (seconds * 1000.0);
        
    }
    
    avgFrameTime = avgFrameTime / frame; 
    double avgFPS = 1 / (avgFrameTime / 1000);
    
    std::chrono::high_resolution_clock::time_point t2 = high_resolution_clock::now();
    
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    cout << "Running time: " << fp_ms.count() << "    Avg Frame Time: " << avgFrameTime << "    Avg FPS: " << avgFPS << endl;

    
    SDL_DestroyWindow( window );
    SDL_Quit();
    
    return 0;
}