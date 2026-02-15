
#include "raylib.h"
#include <time.h>

#define MAP_IMPLEMENTATION
#include "Map.h"

#define MAP_GENS_NUMBER 17
#define MAP_WIDTH  80
#define MAP_HEIGHT 100
#define MAX_ROOMS 30
#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 20
#define PERCENT_DRUNK 0.5

void DrawMinimap(Map map, int posX, int posY, int cellSize)
{
    for (int y = 0; y < map.h; y++)
    {
        for (int x = 0; x < map.w; x++)
        {
            char tile = map.walling[y][x];
            Color color = BROWN;      

            if (tile == '#') color = DARKGRAY;      
            else if (tile == '?') color = BLUE;      
            else if (tile == '*') color = RED;       
            else if (tile == '|') color = ORANGE;     
            else if (tile == '"') color = GREEN;      

            DrawRectangle(posX + x * cellSize, posY + y * cellSize, cellSize, cellSize, color);
        }
    }
}

void RegenerateDungeon(Map *map, int *what)
{
    *what = rand() % MAP_GENS_NUMBER;
    switch (*what)
    {
        case 0:  *map = xmgen_cellular(MAP_WIDTH, MAP_HEIGHT, 0.45, 1000); break;
        case 1:  *map = xmgen(MAP_WIDTH, MAP_HEIGHT, 3+rand()%4, MAX_ROOMS); break;
        case 2:  *map = xmgen_graph(MAP_WIDTH, MAP_HEIGHT, MAX_ROOMS, MIN_ROOM_SIZE, MAX_ROOM_SIZE, 1); break;
        case 3:  *map = xmgen_brogue(MAP_WIDTH, MAP_HEIGHT, MAX_ROOMS, MIN_ROOM_SIZE, MAX_ROOM_SIZE); break;
        case 4:  *map = xmgen_room_maze(MAP_WIDTH, MAP_HEIGHT, MAP_WIDTH-2, MAP_HEIGHT-2, MAX_ROOMS, MIN_ROOM_SIZE, MAX_ROOM_SIZE); break;
        case 5:  *map = xmgen_drunk(MAP_WIDTH, MAP_HEIGHT, PERCENT_DRUNK); break;
        case 6:  *map = xmgen_subtractive(MAP_WIDTH, MAP_HEIGHT, 20); break;
        case 7:  *map = xmgen_perlin(MAP_WIDTH, MAP_HEIGHT, 0.1); break;
        case 8:  *map = xmgen_maze(MAP_WIDTH, MAP_HEIGHT, MAP_WIDTH-2, MAP_HEIGHT-2); break;
        case 9:  *map = xmgen_bsp(MAP_WIDTH, MAP_HEIGHT, MIN_ROOM_SIZE); break;
        case 10: *map = xmgen_scatter(MAP_WIDTH, MAP_HEIGHT, MAX_ROOMS, MIN_ROOM_SIZE, MAX_ROOM_SIZE); break;
        case 11: *map = xmgen_zorbus_like(MAP_WIDTH, MAP_HEIGHT, 500, 90); break;
        case 12: *map = xmgen_hub(MAP_WIDTH, MAP_HEIGHT, 10, MAX_ROOMS, MIN_ROOM_SIZE, MAX_ROOM_SIZE); break;
        case 13: *map = xmgen_winding_path(MAP_WIDTH, MAP_HEIGHT, 20000, 1, 4, 5); break;
        case 14: *map = xmgen_cross_sections(MAP_WIDTH, MAP_HEIGHT, 5, 50); break;
        case 15: *map = xmgen_rings(MAP_WIDTH, MAP_HEIGHT, 100, 30, 8); break;
        case 16: *map = xmgen_prefab_rooms(MAP_WIDTH, MAP_HEIGHT, 50, 3); break;
        default: break;
    }
    xmgen_add_enviroment(map, '"', 0, 0, MAP_WIDTH, MAP_HEIGHT, 0.55);
    for (int i = 0; i < rand()%4; i++)
        xmgen_add_lake(map, '?', rand()%(MAP_HEIGHT - 30), rand()%(MAP_WIDTH - 30), 30, 30, 0.45);
    xmgen_add_lake(map, '|', rand()%(MAP_HEIGHT - 20), rand()%(MAP_WIDTH - 20), 20, 20, 0.45);
}

const char *generatorNames[] = {
    "Cellular Generator",
    "Delaunay Graph Generator",
    "Graph Generator",
    "Brogue Generator",
    "Room Maze Generator",
    "Drunk Generator",
    "Subtractive Generator",
    "Perlin Generator",
    "Maze Generator",
    "BSP Generator",
    "Dummy Generator",
    "Zorbus-like Generator",
    "Hub Generator",
    "Winding Drunk Generator",
    "Cross Sections Generator",
    "Ring Generator",
    "Prefab Generator"
};

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    srand(time(0));
    #ifndef __EMSCRIPTEN__
      //SetConfigFlags(FLAG_FULLSCREEN_MODE); 
      SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED);
    #endif 
    InitWindow(screenWidth, screenHeight, "raylib [models] - procedural cubicmap");

    int currentGenerator = 3;  // start with brogue
    Map map = xmgen_brogue(MAP_WIDTH, MAP_HEIGHT, MAX_ROOMS, MIN_ROOM_SIZE, MAX_ROOM_SIZE);
    xmgen_add_enviroment(&map, '"', 0, 0, MAP_WIDTH, MAP_HEIGHT, 0.55);
    for (int i = 0; i < rand()%4; i++)
        xmgen_add_lake(&map, '?', rand()%(MAP_HEIGHT - 30), rand()%(MAP_WIDTH - 30), 30, 30, 0.45);
    xmgen_add_lake(&map, '|', rand()%(MAP_HEIGHT - 20), rand()%(MAP_WIDTH - 20), 20, 20, 0.45);
    Camera camera = { 0 };
    camera.position = (Vector3){ 60.0f, 40.0f, 80.0f };   // elevated to see whole map
    camera.target = (Vector3){ MAP_WIDTH/2.0f, 0.0f, MAP_HEIGHT/2.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 120.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    bool pause = false;
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_R))
        {
            xmclose(map);
            RegenerateDungeon(&map, &currentGenerator);
        }

        if (IsKeyPressed(KEY_P)) pause = !pause;
        if (IsKeyPressed(KEY_TAB)) exit(-1);
        if (!pause) UpdateCamera(&camera, CAMERA_ORBITAL);
        //tBD TEXTURES height
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
                for (int y = 0; y < map.h; y++)
                {
                    for (int x = 0; x < map.w; x++)
                    {
                        char tile = map.walling[y][x];
                        Color color = BROWN;       // 
                        Vector3 pos = { (float)x + 0.5f, 0.5f, (float)y + 0.5f };
                        if (tile == '#') {color = DARKGRAY; pos.y+=2.0;}
                        else if (tile == '?') color = BLUE;
                        else if (tile == '*') color = RED;
                        else if (tile == '|') color = ORANGE;
                        else if (tile == '"') color = GREEN;
                        else if (tile == ' ' || tile == '+') color = BROWN;
                        else continue; 
                        
                        DrawCube(pos, 1.0f, 1.0f, 1.0f, color);
                    }
                }
            EndMode3D();

            int minimapCell = 5;                         
            int minimapWidth = map.w * minimapCell;
            int minimapHeight = map.h * minimapCell;
            int minimapX = screenWidth - minimapWidth - 20;
            int minimapY = 20;

            DrawMinimap(map, minimapX, minimapY, minimapCell);
            DrawRectangleLines(minimapX, minimapY, minimapWidth, minimapHeight, GREEN);

            DrawText(generatorNames[currentGenerator], 10, 10, 20, DARKGRAY);
            DrawText("R: new map | P: pause orbit | TAB: exit", 10, 35, 20, DARKGRAY);
            //DrawFPS(10, 60);

        EndDrawing();
    }

    xmclose(map);
    CloseWindow();
    return 0;
}
