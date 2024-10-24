#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>

#ifdef ALL_EXTRAS
    #define EXTRA_POINT
    #define CHECK_1
    #define CHECK_2
    #define CHECK_3
#endif

// constants
#define MAX_PATH_LENGTH 1000
#define MAX_N_ROWS 40
#define MAX_N_COLUMNS 100
#define MAX_ENEMIES_PER_SET 10
#define ENEMY_SETS 3

// custom types
#define bool char
#define false 0
#define true 1

#define command_key char
#define BYPASS '\n'
#define NEXT 's'
#define SHOOT ' '
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'
#define INVINCIBILITY 'j'
#define REVERSE_DIRECTION 'l'

// types declarations
typedef char tEntityTile[3][3];

//  Statistics declarations
typedef struct {
    int total_moves_count;
    int effective_shots_count;
    int missed_shots_count;
    int enemy_descends_count;
} tStatistics;

tStatistics tStatistics_init();
tStatistics tStatistics_increment_total_moves(tStatistics statistics);
tStatistics tStatistics_increment_effective_shots(tStatistics statistics);
tStatistics tStatistics_increment_missed_shots(tStatistics statistics);
tStatistics tStatistics_increment_descends(tStatistics statistics);
tStatistics tStatistics_set_descends(tStatistics statistics, int value);
int tStatistics_get_total_moves(tStatistics statistic);
int tStatistics_get_effective_shots(tStatistics statistic);
int tStatistics_get_missed_shots(tStatistics statistic);
int tStatistics_get_descends(tStatistics statistic);

// Ranking definitions;
typedef struct {
    int index;
    int enemy_set;
    int row;
    int iteration;
} tRankEntry;

int tRankEntry_get_index(tRankEntry entry);
int tRankEntry_get_enemy_set(tRankEntry entry);
int tRankEntry_get_row(tRankEntry entry);
int tRankEntry_get_iteration(tRankEntry entry);
tRankEntry tRankEntry_set_index(tRankEntry entry, int index);
tRankEntry tRankEntry_set_enemy_set(tRankEntry entry, int enemy_set);
tRankEntry tRankEntry_set_row(tRankEntry entry, int row);
tRankEntry tRankEntry_set_iteration(tRankEntry entry, int iteration);

typedef struct {
    tRankEntry rank[MAX_ENEMIES_PER_SET * ENEMY_SETS];
    int entries;
} tRanking;

tRanking tRanking_init();
int tRanking_get_nEntries(tRanking ranking);
tRankEntry tRanking_get_entry(tRanking ranking, int index);
tRanking tRanking_set_entry(tRanking ranking, int index, tRankEntry entry);
tRanking tRanking_add_entry(tRanking ranking, tRankEntry entry);

// Entity declarations
typedef struct {
    int x, y;
    bool active;
} tEntity;

int tEntity_get_x(tEntity entity);
int tEntity_get_y(tEntity entity);
tEntity tEntity_read_player(FILE* file);
bool tEntity_is_active(tEntity entity);
tEntity tEntity_set_x(tEntity entity, int x);
tEntity tEntity_set_y(tEntity entity, int x);
tEntity tEntity_move_down(tEntity entity);
tEntity tEntity_move_left(tEntity entity);
tEntity tEntity_move_right(tEntity entity);
tEntity tEntity_set_active(tEntity entity, bool active);

// Enemy set declarations
typedef struct {
    tEntity enemies[MAX_ENEMIES_PER_SET];
    int num_enemies;
} tEnemySet;

tEnemySet tEnemySet_init();
int tEnemySet_get_nEnemies(tEnemySet set);
tEntity tEnemySet_get_enemy(tEnemySet set, int index);
tEnemySet tEnemySet_set_enemy(tEnemySet set, tEntity entity, int index);

// Enemy blocks declarations
typedef struct {
    tEnemySet enemy_sets[ENEMY_SETS];
    command_key enemy_direction;
    bool move_down;
    int enemy_descends_count;
} tEnemyBlock;

int tEnemyBlock_get_descends(tEnemyBlock block);
bool tEnemyBlock_move_down(tEnemyBlock block);
tEnemySet tEnemyBlock_get_enemySet(tEnemyBlock block, int index);
command_key tEnemyBlock_get_direction(tEnemyBlock block);
tEnemyBlock tEnemyBlock_check_initial_downward_movement(tEnemyBlock block, int map_width, int iterations, FILE *summary_file);
tEnemyBlock tEnemyBlock_move(tEnemyBlock, int map_columns, int iterations, FILE *summary_file);
tEnemyBlock tEnemyBlock_init(FILE *map_file);
tEnemyBlock tEnemyBlock_set_enemySet(tEnemyBlock block, tEnemySet set, int index);
tEnemyBlock tEnemyBlock_swap_direction(tEnemyBlock block);

// Map declarations
typedef struct {
    int n_rows, n_columns;
    int marker_line;
    char content[MAX_N_ROWS][MAX_N_COLUMNS];
} tMap;

int tMap_get_nRows(tMap map);
int tMap_get_nColumns(tMap map);
int tMap_get_markerLine(tMap map);
tMap tMap_set_markerLine(tMap map, int y);
char tMap_get_content(tMap map, int row, int column);
tMap tMap_read_shape(FILE* file);
tMap tMap_clear(tMap map);
tMap tMap_draw_entity(tMap map, tEntityTile tile, const int x, const int y);
tMap tMap_draw_enemies(tMap map, tEnemyBlock block, tEntityTile *tiles, int iterations, bool animated_enemies);
void tMap_print_to_file(FILE *file, tMap map);
void tMap_print(tMap map);
tMap tMap_draw_player(tMap map, tEntity player, tEntityTile player_tile);

// Heatmap declarations
typedef struct {
    int n_rows, n_columns;
    unsigned int temperature[MAX_N_ROWS][MAX_N_COLUMNS];
} tHeatMap;

tHeatMap tHeatMap_clear(int n_rows, int n_columns);
tHeatMap tHeatMap_update(tHeatMap heatmap, tEntity player, tEntity shoot);
int tHeatMap_get_nRows(tHeatMap map);
int tHeatMap_get_nColumns(tHeatMap map);
unsigned int tHeatMap_get_temperature(tHeatMap map, int row, int column);

// Game declarations
typedef struct {
    tMap map;
    tHeatMap heatmap;
    tEntity player;
    tEntity shoot;
    tEntityTile player_tile;
    tEntityTile enemy_tiles[3];
    tEnemyBlock enemy_block;
    FILE *summary_file;
    tRanking ranking;
    tStatistics statistics;
    int points;
    int iterations;
    int full_path_len;
    char full_path[MAX_PATH_LENGTH];
    bool animated_enemies;
    #ifdef CHECK_1
        bool invincibility;
    #endif
} tGame;

tGame tGame_read_enemy_tile(tGame game);
tGame tGame_set_path(tGame game, int argc, char const **argv);
tGame tGame_init(tGame game);
tGame tGame_close(tGame game);
tGame tGame_loop(tGame game);
tGame tGame_input(tGame game, command_key command);
tGame tGame_move_shoot(tGame game);
tGame tGame_check_collision(tGame game);
bool tGame_player_won(tGame game);
bool tGame_player_lost(tGame game);
void tGame_print_head(int points, int iterations);
tGame tGame_shoot(tGame game);
tMap tMap_draw_shoot(tMap map, tEntity shoot, int iterations, bool animated_enemies);
tGame tGame_move_player(tGame game, command_key command);
FILE* tGame_create_summary_file(tGame game);

// utils
void clear_file_line(FILE* file);
void check_file(FILE *file_ptr, char *file_path);
void write_initialization_file(char *path, int path_len, tEntity player, tMap map);
void write_ranking_file(char *path, int path_len, tRanking ranking);
void write_heatmap_file(char *path, int path_len, tHeatMap heatmap);
void write_statistics_file(char *path, int path_len, tStatistics statistics);

int main(int argc, char const **argv) {
    tGame game;
    game = tGame_set_path(game, argc, argv);
    game = tGame_init(game);
    game = tGame_loop(game);
    tGame_close(game);

    return 0;
}

// tGame Definitions
tGame tGame_set_path(tGame game, int argc, char const **argv) {
    // check if the path has been specified as an argument for the program
    if (argc < 2) {
        puts("ERRO: Informe o diretorio com os arquivos de configuracao.");
        exit(1);
    }

    // fix the end of the string
    game.full_path_len = strlen(argv[1]);
    if (argv[1][game.full_path_len - 1] == '/') game.full_path_len--;

    // copy the path from arguments and clean the rest
    strcpy(game.full_path, argv[1]);
    memset(game.full_path + game.full_path_len, '\0', MAX_PATH_LENGTH - game.full_path_len);

    return game;
}

tGame tGame_init(tGame game) {
    // read the map file to initialize the player and enemies
    strcpy(game.full_path + game.full_path_len, "/mapa.txt\0");
    FILE *map_file = fopen(game.full_path, "r");
    check_file(map_file, game.full_path);

    // read settings
    game.map = tMap_read_shape(map_file);
    game.player = tEntity_read_player(map_file);
    game.enemy_block = tEnemyBlock_init(map_file);
    fclose(map_file);

    // initial values
    game.animated_enemies = false;
    game = tGame_read_enemy_tile(game);
    game.ranking = tRanking_init();
    game.statistics = tStatistics_init();
    game.shoot = tEntity_set_active(game.shoot, false);
    game.points = game.iterations = 0;
    game.map = tMap_clear(game.map);
    game.map = tMap_set_markerLine(game.map, tEntity_get_y(game.player) - 1);
    game.heatmap = tHeatMap_clear(tMap_get_nRows(game.map), tMap_get_nColumns(game.map));
    #ifdef CHECK_1
        game.invincibility = false;
    #endif

    // define the player's tiles
    game.player_tile[0][0] = 'M', game.player_tile[0][1] = ' ', game.player_tile[0][2] = 'M';
    game.player_tile[1][0] = 'M', game.player_tile[1][1] = 'M', game.player_tile[1][2] = 'M';
    game.player_tile[2][0] = 'M', game.player_tile[2][1] = 'M', game.player_tile[2][2] = 'M';

    game.summary_file = tGame_create_summary_file(game);
    game.map = tMap_draw_player(game.map, game.player, game.player_tile);
    game.map = tMap_draw_enemies(game.map, game.enemy_block, game.enemy_tiles, game.iterations, game.animated_enemies);

    // print the first map
    tGame_print_head(game.points, game.iterations);
    tMap_print(game.map);
    write_initialization_file(game.full_path, game.full_path_len, game.player, game.map);

    return game;
}

tGame tGame_close(tGame game) {
    fclose(game.summary_file);
    // write complementary files
    write_ranking_file(game.full_path, game.full_path_len, game.ranking);
    write_heatmap_file(game.full_path, game.full_path_len, game.heatmap);
    write_statistics_file(game.full_path, game.full_path_len, game.statistics);
}

tGame tGame_loop(tGame game) {
    command_key command = BYPASS;

    #ifdef SAVE_INPUT
        strcpy(game.full_path + game.full_path_len, "/entrada.txt\0");
        FILE *input_txt = fopen(game.full_path, "w");
    #endif

    do {
        // check game states
        if (tGame_player_won(game)) {
            printf("Parabéns, você ganhou!\n");
            game.heatmap = tHeatMap_update(game.heatmap, game.player, game.shoot);
            break;
        }
        if (tGame_player_lost(game)) {
            printf("Você perdeu, tente novamente!\n");
            game.heatmap = tHeatMap_update(game.heatmap, game.player, game.shoot);
            break;
        }

        // get command from stdin
        scanf("%c", &command);

        #ifdef SAVE_INPUT
            fprintf(input_txt, "%c", command);
        #endif

        if (command == BYPASS) {
            continue;
        } else {
            game.heatmap = tHeatMap_update(game.heatmap, game.player, game.shoot);
            game = tGame_check_collision(game);
            game.enemy_block = tEnemyBlock_move(
                game.enemy_block, tMap_get_nColumns(game.map),
                game.iterations, game.summary_file
            );
            game = tGame_move_shoot(game);
            game.iterations++;
            game = tGame_input(game, command);
            #ifdef ANIMATED_INPUT
                printf("\033[H\033[J");
                usleep(200 * 1000);
            #endif
        }

        // updates map content
        game.map = tMap_clear(game.map);
        game.map = tMap_draw_player(game.map, game.player, game.player_tile);
        game.map = tMap_draw_enemies(game.map, game.enemy_block,  game.enemy_tiles, game.iterations, game.animated_enemies);
        game.map = tMap_draw_shoot(game.map, game.shoot, game.iterations, game.animated_enemies);

        // show current game state
        tGame_print_head(game.points, game.iterations);
        tMap_print(game.map);
    } while (true);

    // update statistics
    game.statistics = tStatistics_set_descends(
        game.statistics , tEnemyBlock_get_descends(game.enemy_block));

    #ifdef SAVE_INPUT
        fprintf(input_txt, "\n");
        fclose(input_txt);
    #endif

    return game;
}

tGame tGame_input(tGame game, command_key command) {
    switch (command) {
        case NEXT:
            return game;

        case SHOOT:
            game = tGame_shoot(game);
            return game;

        case MOVE_LEFT:
        case MOVE_RIGHT:
            game = tGame_move_player(game, command);
            return game;

        #ifdef CHECK_1
            case INVINCIBILITY:
                game.invincibility = !game.invincibility;
                return game;
        #endif

        #ifdef CHECK_3
            case REVERSE_DIRECTION:
                game.enemy_block = tEnemyBlock_swap_direction(game.enemy_block);
                return game;
        #endif

        default:
            printf("ERRO: A tecla '%c' nao representa um comando valido!\n", command);
            return game;
    }
}

tGame tGame_move_shoot(tGame game) {
    if (tEntity_is_active(game.shoot)) {
        if (tEntity_get_y(game.shoot) <= 0) {
            game.shoot = tEntity_set_active(game.shoot, false);
            game.statistics = tStatistics_increment_missed_shots(game.statistics);
        } else {
            game.shoot = tEntity_set_y(game.shoot, tEntity_get_y(game.shoot) - 1);
        }
    }
    return game;
}

tGame tGame_check_collision(tGame game) {
    if (!tEntity_is_active(game.shoot)) return game;

    for (int i = 0; i < ENEMY_SETS; i++) {
        tEnemySet set = tEnemyBlock_get_enemySet(game.enemy_block, i);

        for (int j = 0; j < tEnemySet_get_nEnemies(set); j++) {
            tEntity enemy = tEnemySet_get_enemy(set, j);
            if (tEntity_is_active(enemy)) {
                // compare the enemy`s entire 3x3 matrix with to the shoot
                for (int x = -1; x < 2; x++)
                    for (int y = -1; y < 2; y++)
                        if ((tEntity_get_x(enemy) + x == tEntity_get_x(game.shoot)) &&
                            (tEntity_get_y(enemy) + y == tEntity_get_y(game.shoot))
                            #ifdef CHECK_1
                                && !game.invincibility
                            #endif
                            ) {
                            // remove shoot and enemy
                            tEntity enemy = tEnemySet_get_enemy(set, j);
                            enemy = tEntity_set_active(enemy, false);
                            set = tEnemySet_set_enemy(set, enemy, j);
                            game.enemy_block = tEnemyBlock_set_enemySet(game.enemy_block, set, i);
                            game.shoot = tEntity_set_active(game.shoot, false);

                            // add points
                            game.points += (tEntity_get_x(enemy) + 1) * (
                                tMap_get_nRows(game.map) - tEntity_get_y(enemy) - 1);

                            // add enemy information to summary
                            fprintf(game.summary_file,
                                "[%d] Inimigo de indice %d da fileira %d foi atingido na posicao (%d %d).\n",
                                game.iterations, j + 1, i + 1,
                                tEntity_get_x(game.shoot) + 1, tEntity_get_y(game.shoot) + 1);

                            // add enemy information to ranking
                            tRankEntry entry;
                            entry = tRankEntry_set_iteration(entry, game.iterations);
                            entry = tRankEntry_set_enemy_set(entry, i + 1);
                            entry = tRankEntry_set_index(entry, j + 1);
                            entry = tRankEntry_set_row(entry, tMap_get_nRows(game.map) - tEntity_get_y(enemy) - y);
                            game.ranking = tRanking_add_entry(game.ranking, entry);
                        }
            }
        }
    }

    return game;
}

bool tGame_player_won(tGame game) {
    for (int i = 0; i < ENEMY_SETS; i++) {
        tEnemySet set = tEnemyBlock_get_enemySet(game.enemy_block, i);
        for (int j = 0; j < tEnemySet_get_nEnemies(set); j++) {
            if (tEntity_is_active(tEnemySet_get_enemy(set, j))) {
                return false;
            }
        }
    }
    return true;
}

bool tGame_player_lost(tGame game) {
    for (int i = 0; i < ENEMY_SETS; i++) {
        tEnemySet set = tEnemyBlock_get_enemySet(game.enemy_block, i);
        for (int j = 0; j < tEnemySet_get_nEnemies(set); j++) {
            if (tEntity_is_active(tEnemySet_get_enemy(set, j)) &&
                tEntity_get_y(tEnemySet_get_enemy(set, j)) + 1 >= tMap_get_markerLine(game.map)) {
                return true;
            }
        }
    }
    return false;
}

void tGame_print_head(int points, int iterations) {
    printf("Pontos: %d | Iteracoes: %d\n", points, iterations);
}

tGame tGame_move_player(tGame game, command_key command) {
    if (command == MOVE_LEFT) {
        if (tEntity_get_x(game.player) > 1) {
            game.player = tEntity_set_x(game.player, tEntity_get_x(game.player) - 1);
            game.statistics = tStatistics_increment_total_moves(game.statistics);
        } else {
            fprintf(
                game.summary_file,
                "[%d] Jogador colidiu na lateral esquerda.\n",
                game.iterations
            );
        }
    } else if (command == MOVE_RIGHT) {
        if (tEntity_get_x(game.player) + 1 < tMap_get_nColumns(game.map) - 1) {
            game.statistics = tStatistics_increment_total_moves(game.statistics);
            game.player = tEntity_set_x(game.player, tEntity_get_x(game.player) + 1);
        } else {
            fprintf(
                game.summary_file,
                "[%d] Jogador colidiu na lateral direita.\n",
                game.iterations
            );
        }
    }
    return game;
}

tGame tGame_shoot(tGame game) {
    if (!tEntity_is_active(game.shoot)) {
        game.shoot = tEntity_set_active(game.shoot, true);
        game.shoot = tEntity_set_x(game.shoot, tEntity_get_x(game.player));
        game.shoot = tEntity_set_y(game.shoot, tEntity_get_y(game.player) - 2);
        game.statistics = tStatistics_increment_effective_shots(game.statistics);
    }
    return game;
}

FILE* tGame_create_summary_file(tGame game) {
    // create file
    strcpy(game.full_path + game.full_path_len, "/saida/resumo.txt");
    FILE *summary_file = fopen(game.full_path, "w");
    check_file(summary_file, game.full_path);

    // clear path 
    memset(game.full_path + game.full_path_len, '\0', 17);

    return summary_file;
}

tGame tGame_read_enemy_tile(tGame game) {
    // read the enemy tile file
    strcpy(game.full_path + game.full_path_len, "/inimigo.txt");
    FILE *enemy_tile_file = fopen(game.full_path, "r");
    check_file(enemy_tile_file, game.full_path);

    int flag;
    fscanf(enemy_tile_file, "%d", &flag);
    game.animated_enemies = flag;
    clear_file_line(enemy_tile_file);

    // read tiles
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            fscanf(enemy_tile_file, "%c%c%c", &game.enemy_tiles[i][j][0],
                &game.enemy_tiles[i][j][1], &game.enemy_tiles[i][j][2]);
            clear_file_line(enemy_tile_file);
        }
    }

    // clear settings from the file
    fclose(enemy_tile_file);

    return game;
}

// tEntity definitions
int tEntity_get_x(tEntity entity) {
    return entity.x;
}

tEntity tEntity_set_x(tEntity entity, int x) {
    entity.x = x;
    return entity;
}

int tEntity_get_y(tEntity entity) {
    return entity.y;
}

tEntity tEntity_set_y(tEntity entity, int y) {
    entity.y = y;
    return entity;
}

bool tEntity_is_active(tEntity entity) {
    return entity.active;
}

tEntity tEntity_set_active(tEntity entity, bool active) {
    entity.active = active;
    return entity;
}

tEntity tEntity_move_right(tEntity entity) {
    entity.x++;
    return entity;
}

tEntity tEntity_move_left(tEntity entity) {
    entity.x--;
    return entity;
}

tEntity tEntity_move_down(tEntity entity) {
    entity.y++;
    return entity;
}

tEntity tEntity_read_player(FILE* file) {
    tEntity player;
    fscanf(file, "(%d %d)", &player.x, &player.y);
    clear_file_line(file);
    player.active = true;

    // adjust the positions to fit within the matrix
    player.x--, player.y--;

    return player;
}

// tMap definitions
int tMap_get_nRows(tMap map) {
    return map.n_rows;
}

int tMap_get_nColumns(tMap map) {
    return map.n_columns;
}

int tMap_get_markerLine(tMap map) {
    return map.marker_line;
}

tMap tMap_set_markerLine(tMap map, int y) {
    map.marker_line = y;
    return map;
}

char tMap_get_content(tMap map, int row, int column) {
    return map.content[row][column];
}

tMap tMap_read_shape(FILE* file) {
    tMap map;
    fscanf(file, "%d %d", &map.n_columns, &map.n_rows);
    clear_file_line(file);
    return map;
}

tMap tMap_clear(tMap old_map) {
    // copy data from old map
    tMap new_map = {
        .n_rows = old_map.n_rows,
        .n_columns = old_map.n_columns,
        .marker_line = old_map.marker_line
    };

    // clear all content
    for (int i = 0; i < old_map.n_rows; i++) {
        for (int j = 0; j < old_map.n_columns; j++) {
            new_map.content[i][j] = ' ';
        }
    }

    return new_map;
}

tMap tMap_draw_enemies(
    tMap map,
    tEnemyBlock block,
    tEntityTile *tiles,
    int iterations,
    bool animated_enemies
) {
    for (int i = 0; i < ENEMY_SETS; i++) {
        tEnemySet set = tEnemyBlock_get_enemySet(block, i);
        for (int j = 0; j < tEnemySet_get_nEnemies(set); j++) {
            if (tEntity_is_active(tEnemySet_get_enemy(set, j))) {
                map = tMap_draw_entity(
                    map,
                    #ifdef EXTRA_POINT
                        animated_enemies ? tiles[iterations % 3]:tiles[0],
                    #else
                        tiles[0],
                    #endif
                    block.enemy_sets[i].enemies[j].x,
                    block.enemy_sets[i].enemies[j].y);
            }
        }
    }
    return map;
}

tMap tMap_draw_shoot(tMap map, tEntity shoot, int iterations, bool animated_enemies) {
    if (tEntity_is_active(shoot)) {
        #ifdef CHECK_2
            if (animated_enemies && iterations % 2 == 0)
                map.content[tEntity_get_y(shoot)][tEntity_get_x(shoot)] = '0';
            else
        #endif
            map.content[tEntity_get_y(shoot)][tEntity_get_x(shoot)] = 'o';
    }
    return map;
}

tMap tMap_draw_entity(tMap map, tEntityTile tile, const int x, const int y) {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            map.content[y + i][x + j] = tile[i + 1][j + 1];
        }
    }
    return map;
}

tMap tMap_draw_player(tMap map, tEntity player, tEntityTile player_tile) {
    int player_x = tEntity_get_x(player);
    int player_y = tEntity_get_y(player);
    return tMap_draw_entity(map, player_tile, player_x, player_y);
}

void tMap_print_to_file(FILE *file, tMap map) {
    // add 2 more to accommodate both vertical and lateral borders
    for (int i = 0; i < map.n_rows + 2; i++) {
        for (int j = 0; j < map.n_columns + 2; j++) {
            // check the border style according to the position, if it's within the map, draw the content
            if ((i == 0 || i == map.n_rows + 1) && (j == 0 || j == map.n_columns + 1)) fprintf(file, "+");
            else if ((j == 0 || j == map.n_columns + 1) && (i == map.marker_line)) fprintf(file, "-");
            else if (j == 0 || j == map.n_columns + 1) fprintf(file, "|");
            else if (i == 0 || i == map.n_rows + 1) fprintf(file, "-");
            else fprintf(file, "%c", map.content[i - 1][j - 1]);
        }
        fprintf(file, "\n");
    }
}

void tMap_print(tMap map) {
    tMap_print_to_file(stdout, map);
}

// tHeatMap definitions
int tHeatMap_get_nRows(tHeatMap map) {
    return map.n_rows;
}

int tHeatMap_get_nColumns(tHeatMap map) {
    return map.n_columns;
}

unsigned int tHeatMap_get_temperature(tHeatMap map, int row, int column) {
    return map.temperature[row][column];
}

tHeatMap tHeatMap_clear(int n_rows, int n_columns) {
    tHeatMap new_map = {
        .n_rows = n_rows,
        .n_columns = n_columns
    };

    // reset all positions
    for (int i = 0; i < new_map.n_rows; i++)
        for (int j = 0; j < new_map.n_columns; j++)
            new_map.temperature[i][j] = 0;

    return new_map;
}

tHeatMap tHeatMap_update(tHeatMap heatmap, tEntity player, tEntity shoot) {
    // add player position to heatmap
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
            heatmap.temperature[tEntity_get_y(player) + i][tEntity_get_x(player) + j]++;

    // add shoot data to heatmap
    if (tEntity_is_active(shoot))
        heatmap.temperature[tEntity_get_y(shoot)][tEntity_get_x(shoot)]++;

    return heatmap;
}

// tEnemySet definitions
tEntity tEnemySet_get_enemy(tEnemySet set, int index) {
    return set.enemies[index];
}

int tEnemySet_get_nEnemies(tEnemySet set) {
    return set.num_enemies;
}

tEnemySet tEnemySet_set_enemy(tEnemySet set, tEntity entity, int index) {
    set.enemies[index] = entity;
    return set;
}

tEnemySet tEnemySet_init() {
    tEnemySet empty = { .num_enemies = 0};
    for (int enemy_id = 0; enemy_id < MAX_ENEMIES_PER_SET; enemy_id++)
        empty.enemies[enemy_id].active = false;
    return empty;
}

// tEnemyBlock definitions
command_key tEnemyBlock_get_direction(tEnemyBlock block) {
    return block.enemy_direction;
}

int tEnemyBlock_get_descends(tEnemyBlock block) {
    return block.enemy_descends_count;
}

bool tEnemyBlock_move_down(tEnemyBlock block) {
    return block.move_down;
}

tEnemySet tEnemyBlock_get_enemySet(tEnemyBlock block, int index) {
    return block.enemy_sets[index];
}

tEnemyBlock tEnemyBlock_check_initial_downward_movement(tEnemyBlock block, int map_width, int iterations, FILE *summary_file) {
    // check at the beginning of the game if you have to move down
    block.move_down = false;

    for (int i = 0; i < ENEMY_SETS; i++) {
        tEnemySet set = tEnemyBlock_get_enemySet(block, i);
        for (int j = 0; j < tEnemySet_get_nEnemies(set); j++) {
            if (!tEntity_is_active(tEnemySet_get_enemy(set, j))) continue;

            if (block.enemy_direction == MOVE_RIGHT) {
                if (tEntity_get_x(tEnemySet_get_enemy(set, j)) + 2 >= map_width) {
                    block.move_down = true;
                    fprintf(summary_file,
                            "[%d] Inimigo de indice %d da fileira %d colidiu na lateral direita.\n",
                            iterations, j + 1, i + 1);
                }
            } else {
                if (tEntity_get_x(tEnemySet_get_enemy(set, j)) - 1 <= 0) {
                    block.move_down = true;
                    fprintf(summary_file,
                            "[%d] Inimigo de indice %d da fileira %d colidiu na lateral esquerda.\n",
                            iterations, j + 1, i + 1);
                }
            }
        }
    }

    return block;
}

tEnemyBlock tEnemyBlock_move(
    tEnemyBlock block,
    int map_columns,
    int iterations,
    FILE *summary_file
) {
    block = tEnemyBlock_check_initial_downward_movement(block, map_columns, iterations, summary_file);

    if (block.move_down) {
        // move all enemies one position down
        for (int i = 0; i < ENEMY_SETS; i++) {
            for (int j = 0; j < tEnemySet_get_nEnemies(block.enemy_sets[i]); j++) {
                if (tEntity_is_active(tEnemySet_get_enemy(block.enemy_sets[i], j))) {
                    tEntity enemy = tEnemySet_get_enemy(block.enemy_sets[i], j);
                    enemy = tEntity_move_down(enemy);
                    block.enemy_sets[i] = tEnemySet_set_enemy(block.enemy_sets[i], enemy, j);
                }
            }
        }

        // update direction values
        block = tEnemyBlock_swap_direction(block);
        block.enemy_descends_count++;
    } else {
        // move horizontally
        for (int i = 0; i < ENEMY_SETS; i++) {
            for (int j = 0; j < tEnemySet_get_nEnemies(block.enemy_sets[i]); j++) {
                if (!tEntity_is_active(tEnemySet_get_enemy(block.enemy_sets[i], j))) continue;
                tEntity enemy = tEnemySet_get_enemy(block.enemy_sets[i], j);

                if (block.enemy_direction == MOVE_RIGHT) {
                    enemy = tEntity_move_right(enemy);

                    // check if the leftmost one is touching
                    if (tEntity_get_x(enemy) + 1 == map_columns - 1) {
                        block.move_down = true;
                    }
                } else {
                    enemy = tEntity_move_left(enemy);

                    // check if the rightmost one is touching
                    if (tEntity_get_x(enemy) == 1) {
                        block.move_down = true;
                    }
                }

                block.enemy_sets[i] = tEnemySet_set_enemy(block.enemy_sets[i], enemy, j);
            }
        }
    }

    return block;
}

tEnemyBlock tEnemyBlock_set_enemySet(tEnemyBlock block, tEnemySet set, int index) {
    block.enemy_sets[index] = set;
    return block;
}

tEnemyBlock tEnemyBlock_swap_direction(tEnemyBlock block) {
    block.enemy_direction = block.enemy_direction == MOVE_LEFT ? MOVE_RIGHT:MOVE_LEFT;
    return block;
}

tEnemyBlock tEnemyBlock_init(FILE *map_file) {
    char current_character;
    tEnemyBlock block = {
        .enemy_descends_count = 0,
        .enemy_direction = MOVE_RIGHT,
        .move_down = false
    };

    // read enemy positions in the file
    for (int enemy_set = 0; enemy_set < ENEMY_SETS; enemy_set++) {
        block.enemy_sets[enemy_set] = tEnemySet_init();

        do {
            // check if there is an enemy at that position to read
            fscanf(map_file, "%c", &current_character);
            if (current_character == '\n') break;
            if (current_character == ' ') continue;

            // read enemy position
            fscanf(map_file, "%d %d)",
                &block.enemy_sets[enemy_set].enemies[block.enemy_sets[enemy_set].num_enemies].x,
                &block.enemy_sets[enemy_set].enemies[block.enemy_sets[enemy_set].num_enemies].y);
            
            // adjust the values of the enemy
            block.enemy_sets[enemy_set].enemies[block.enemy_sets[enemy_set].num_enemies].x--;
            block.enemy_sets[enemy_set].enemies[block.enemy_sets[enemy_set].num_enemies].y--;
            block.enemy_sets[enemy_set].enemies[block.enemy_sets[enemy_set].num_enemies].active = true;
            block.enemy_sets[enemy_set].num_enemies++;
        } while (true);
    }

    return block;
}

// tStatistics definitions
tStatistics tStatistics_init() {
    tStatistics statistics = {
        .effective_shots_count = 0,
        .enemy_descends_count = 0,
        .missed_shots_count = 0,
        .total_moves_count = 0,
    };
    return statistics;
}

tStatistics tStatistics_increment_total_moves(tStatistics statistics) {
    statistics.total_moves_count++;
    return statistics;
}

tStatistics tStatistics_increment_effective_shots(tStatistics statistics) {
    statistics.effective_shots_count++;
    return statistics;
}

tStatistics tStatistics_increment_missed_shots(tStatistics statistics) {
    statistics.missed_shots_count++;
    return statistics;
}

tStatistics tStatistics_increment_descends(tStatistics statistics) {
    statistics.enemy_descends_count++;
    return statistics;
}

tStatistics tStatistics_set_descends(tStatistics statistics, int value) {
    statistics.enemy_descends_count = value;
    return statistics;
}

int tStatistics_get_total_moves(tStatistics statistic) {
    return statistic.total_moves_count;
}

int tStatistics_get_effective_shots(tStatistics statistic) {
    return statistic.effective_shots_count;
}

int tStatistics_get_missed_shots(tStatistics statistic) {
    return statistic.missed_shots_count;
}

int tStatistics_get_descends(tStatistics statistic) {
    return statistic.enemy_descends_count;
}

// tRankEntry definitions
int tRankEntry_get_index(tRankEntry entry) {
    return entry.index;
}

tRankEntry tRankEntry_set_index(tRankEntry entry, int index) {
    entry.index = index;
    return entry;
}

int tRankEntry_get_enemy_set(tRankEntry entry) {
    return entry.enemy_set;
}

tRankEntry tRankEntry_set_enemy_set(tRankEntry entry, int enemy_set) {
    entry.enemy_set = enemy_set;
    return entry;
}

int tRankEntry_get_row(tRankEntry entry) {
    return entry.row;
}

tRankEntry tRankEntry_set_row(tRankEntry entry, int row) {
    entry.row = row;
    return entry;
}

int tRankEntry_get_iteration(tRankEntry entry) {
    return entry.iteration;
}

tRankEntry tRankEntry_set_iteration(tRankEntry entry, int iteration) {
    entry.iteration = iteration;
    return entry;
}

// tRanking definitions
tRanking tRanking_init() {
    tRanking ranking = { .entries = 0 };
    return ranking;
}

int tRanking_get_nEntries(tRanking ranking) {
    return ranking.entries;
}

tRankEntry tRanking_get_entry(tRanking ranking, int index) {
    return ranking.rank[index];
}

tRanking tRanking_set_entry(tRanking ranking, int index, tRankEntry entry) {
    ranking.rank[index] = entry;
    return ranking;
}

tRanking tRanking_add_entry(tRanking ranking, tRankEntry entry) {
    ranking.rank[ranking.entries] = entry;
    ranking.entries++;
    return ranking;
}

// utils definitions
void clear_file_line(FILE* file) {
    char new_line = '\0';
    while (new_line != '\n' && fscanf(file, "%c", &new_line) != EOF);
}

void check_file(FILE *file_ptr, char *file_path) {
    if (!file_ptr) {
        printf("ERRO: Nao foi possivel abrir o arquivo '%s'.\n", file_path);
        exit(EXIT_FAILURE);
    }
}

void write_initialization_file(char *path, int path_len, tEntity player, tMap map) {
    // create the game initialization file
    strcpy(path + path_len, "/saida/inicializacao.txt\0");
    FILE *init_file = fopen(path, "w");
    check_file(init_file, path);

    // print map and player's starting position
    tMap_print_to_file(init_file, map);
    fprintf(init_file,
        "A posicao central do jogador iniciara em (%d %d).",
        tEntity_get_x(player) + 1, tEntity_get_y(player) + 1);

    fclose(init_file);
}

void write_ranking_file(char *path, int path_len, tRanking ranking) {
    // create ranking file
    strcpy(path + path_len, "/saida/ranking.txt\0");
    FILE *ranking_file = fopen(path, "w");
    check_file(ranking_file, path);

    // sort rank entries
    for (int i = 0; i < tRanking_get_nEntries(ranking); i++) {
        for (int j = i; j < tRanking_get_nEntries(ranking); j++) {
            if ((tRankEntry_get_row(tRanking_get_entry(ranking, j)) < tRankEntry_get_row(tRanking_get_entry(ranking, i))) ||
                (tRankEntry_get_row(tRanking_get_entry(ranking, j)) == tRankEntry_get_row(tRanking_get_entry(ranking, i)))
                && (tRankEntry_get_iteration(tRanking_get_entry(ranking, j)) < tRankEntry_get_iteration(tRanking_get_entry(ranking, i)))) {
                // swap entries
                tRankEntry tmp = tRanking_get_entry(ranking, i);
                ranking = tRanking_set_entry(ranking, i, tRanking_get_entry(ranking, j));
                ranking = tRanking_set_entry(ranking, j, tmp);
            }
        }
    }

    // write csv data
    fprintf(ranking_file, "indice,fileira,linha,iteracao\n");
    for (int i = 0; i < tRanking_get_nEntries(ranking); i++)
        fprintf(ranking_file, "%d,%d,%d,%d\n",
            tRankEntry_get_index(tRanking_get_entry(ranking, i)),
            tRankEntry_get_enemy_set(tRanking_get_entry(ranking, i)),
            tRankEntry_get_row(tRanking_get_entry(ranking, i)),
            tRankEntry_get_iteration(tRanking_get_entry(ranking, i))
        );

    // clear settings from the file
    fclose(ranking_file);
}

void write_heatmap_file(char *path, int path_len, tHeatMap heatmap) {
    // create heatmap file
    strcpy(path + path_len, "/saida/heatmap.txt\0");
    FILE *heatmap_file = fopen(path, "w");
    check_file(heatmap_file, path);

    // write heatmap temperatures
    for (int i = 0; i < tHeatMap_get_nRows(heatmap); i++) {
        for (int j = 0; j < tHeatMap_get_nColumns(heatmap); j++) {
            int temperature = tHeatMap_get_temperature(heatmap, i, j);
            temperature = (temperature > 999) ? 999:temperature;
            fprintf(heatmap_file, "%3d ", temperature);
        }
        fprintf(heatmap_file, "\n");
    }

    fclose(heatmap_file);
}

void write_statistics_file(char *path, int path_len, tStatistics statistics) {
    // create statistics file
    strcpy(path + path_len, "/saida/estatisticas.txt\0");
    FILE *stats_file = fopen(path, "w");
    check_file(stats_file, path);

    // write heatmap temperatures
    fprintf(stats_file, "Numero total de movimentos (A ou D): %d;\n", tStatistics_get_total_moves(statistics));
    fprintf(stats_file, "Numero de tiros efetivos: %d;\n", tStatistics_get_effective_shots(statistics));
    fprintf(stats_file, "Numero de tiros que nao acertaram: %d;\n", tStatistics_get_missed_shots(statistics));
    fprintf(stats_file, "Numero de descidas dos inimigos: %d;\n", tStatistics_get_descends(statistics));

    fclose(stats_file);
}
