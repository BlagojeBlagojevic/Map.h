# Map.h – Dungeon/Map Generation Library
Map.h is a single-header C library for procedural map generation. It provides a wide variety of algorithms to create dungeons, caves, mazes, rooms, and other tile-based environments – ideal for roguelikes, RPGs, and grid‑based games.

The library uses only standard C libraries (stdlib, stdio, math, etc.) and is easy to integrate: just #include "Map.h" and define MAP_IMPLEMENTATION in exactly one .c file.


## Quick start

```c
// In exactly one .c file:
#define MAP_IMPLEMENTATION
#include "Map.h"

int main() {
    // Generate a 80x25 dungeon using the default Delaunay graph method
    Map dungeon = xmgen(80, 25, 5, 15);

    // Print it to the console
    xmprint(dungeon);

    // Free memory
    xmclose(dungeon);
    return 0;
}

```

Compile with any C compiler (C99 or later). Example:
```bash
gcc -o example example.c -lm
./example
```

---

## API Overview

All public functions and types are prefixed with `xm` (or `Map`). The core data structure is `Map`:

```c
typedef struct {
    char** walling;   // 2D array of tiles ('#' = wall, ' ' = floor, '+' = door/corridor, etc.)
    int w;            // width
    int h;            // height
} Map;
```

### Memory Management
- `Map xmgen(...)` / `Map xmgen_xxx(...)` – create a new map.
- `void xmclose(Map map)` – free all memory used by the map.
- `void xmprint(Map map)` – print the map to stdout (useful for debugging).

### Generators

| Function | Description |
|----------|-------------|
| `xmgen(w, h, grid, max)` | Delaunay‑triangulation based dungeon with rooms at grid points. |
| `xmgen_graph(w, h, num_rooms, min_size, max_size, extra_connections)` | Place rooms randomly and connect them with corridors. |
| `xmgen_scatter(w, h, room_count, min_sz, max_sz)` | Scatter rooms and connect them in a chain. |
| `xmgen_drunk(w, h, floor_goal_percent)` | Drunkard’s walk (random walk) until a target floor percentage is reached. |
| `xmgen_cellular(w, h, wall_percent, iterations)` | Cellular automata cave generation. |
| `xmgen_brogue(w, h, max_rooms, min_size, max_size)` | Brogue‑style dungeon with shaped rooms and corridors. |
| `xmgen_bsp(w, h, min_room_size)` | Binary Space Partitioning dungeon. |
| `xmgen_perlin(w, h, threshold)` | Perlin noise map (values above threshold become floor). |
| `xmgen_maze(maze_w, maze_h, w, h)` | Perfect maze using recursive backtracker. |
| `xmgen_room_maze(wR, hR, w, h, num_rooms_to_try, min_room_size, max_room_size)` | Maze with rooms carved inside. |
| `xmgen_subtractive(w, h, carve_count)` | Random walks that carve out corridors, then clean up. |
| `xmgen_zorbus_like(w, h, iterations, percent_room)` | Inspired by Zorbus – expand from a start point, adding corridors or rooms. |
| `xmgen_hub(w, h, hub_radius, spoke_count, room_min, room_max)` | Central hub with spokes leading to rooms. |
| `xmgen_winding_path(w, h, max_path_len, room_chance, room_min, room_max)` | Random winding path with optional side rooms. |
| `xmgen_cross_sections(w, h, spacing, room_chance)` | Grid of corridors with occasional rooms at intersections. |
| `xmgen_rings(w, h, num_rings, ring_spacing, room_chance)` | Concentric rings connected by spokes. |
| `xmgen_prefab_rooms(w, h, num_rooms, min_dist)` | Place pre‑defined room shapes (30+ prefabs) and connect them. |

### Environment Modifiers
- `xmgen_add_lake(Map* map, char tile, int x, int y, int w, int h, float lakePercent)` – Overlay a cellular‑automata lake (or any tile) onto the map.
- `xmgen_add_enviroment(Map* map, char tile, int x, int y, int w, int h, float lakePercent)` – Similar to lake but only places tile on existing floors.

---



## License

This library is provided under the **MIT License**. See the `LICENSE` file for details. (If no license is specified, you may want to add one.)

---

## Contributing

Pull requests are welcome! If you add a new generator, please follow the naming convention `xmgen_yourname` and include a brief description in the README. For major changes, open an issue first to discuss.

---

## Acknowledgements
Tbd

*Happy dungeon crawling!* 🗝️

