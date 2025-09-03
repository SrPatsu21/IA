#!/bin/bash

declare -A commands

commands["T1_bfs"]="sudo perf stat ./maze_room.out bfs 0 0 7 6"
commands["T1_dfs"]="sudo perf stat ./maze_room.out dfs 0 0 7 6"
commands["T1_greedy"]="sudo perf stat ./maze_room.out greedy 0 0 7 6"
commands["T1_astar"]="sudo perf stat ./maze_room.out astar 0 0 7 6"

commands["T2_bfs"]="sudo perf stat ./maze_room.out bfs 0 0 7 0"
commands["T2_dfs"]="sudo perf stat ./maze_room.out dfs 0 0 7 0"
commands["T2_greedy"]="sudo perf stat ./maze_room.out greedy 0 0 7 0"
commands["T2_astar"]="sudo perf stat ./maze_room.out astar 0 0 7 0"

commands["T3_bfs"]="sudo perf stat ./maze_room.out bfs 0 0 0 6"
commands["T3_dfs"]="sudo perf stat ./maze_room.out dfs 0 0 0 6"
commands["T3_greedy"]="sudo perf stat ./maze_room.out greedy 0 0 0 6"
commands["T3_astar"]="sudo perf stat ./maze_room.out astar 0 0 0 6"

commands["T4_bfs"]="sudo perf stat ./maze_room.out bfs 0 0 4 3"
commands["T4_dfs"]="sudo perf stat ./maze_room.out dfs 0 0 4 3"
commands["T4_greedy"]="sudo perf stat ./maze_room.out greedy 0 0 4 3"
commands["T4_astar"]="sudo perf stat ./maze_room.out astar 0 0 4 3"

commands["T5_bfs"]="sudo perf stat ./maze_room.out bfs 0 0 0 0"
commands["T5_dfs"]="sudo perf stat ./maze_room.out dfs 0 0 0 0"
commands["T5_greedy"]="sudo perf stat ./maze_room.out greedy 0 0 0 0"
commands["T5_astar"]="sudo perf stat ./maze_room.out astar 0 0 0 0"

commands["T6_bfs"]="sudo perf stat ./maze_room.out bfs 4 3 5 3"
commands["T6_dfs"]="sudo perf stat ./maze_room.out dfs 4 3 5 3"
commands["T6_greedy"]="sudo perf stat ./maze_room.out greedy 4 3 5 3"
commands["T6_astar"]="sudo perf stat ./maze_room.out astar 4 3 5 3"

commands["T7_bfs"]="sudo perf stat ./maze_room.out bfs 4 3 7 0"
commands["T7_dfs"]="sudo perf stat ./maze_room.out dfs 4 3 7 0"
commands["T7_greedy"]="sudo perf stat ./maze_room.out greedy 4 3 7 0"
commands["T7_astar"]="sudo perf stat ./maze_room.out astar 4 3 7 0"

commands["T8_bfs"]="sudo perf stat ./maze_room.out bfs 4 3 0 6"
commands["T8_dfs"]="sudo perf stat ./maze_room.out dfs 4 3 0 6"
commands["T8_greedy"]="sudo perf stat ./maze_room.out greedy 4 3 0 6"
commands["T8_astar"]="sudo perf stat ./maze_room.out astar 4 3 0 6"

REPS=100

function avg_time {
    local cmd="$1"
    local total=0

    for ((i=1; i<=REPS; i++))
    do
        t=$( $cmd 2>&1 | grep "seconds time elapsed" | awk '{print $1}' )
        total=$(echo "$total + $t" | bc -l)
    done

    avg=$(echo "$total / $REPS" | bc -l)
    echo "$avg"
}

echo "Calculando médias (tempo em segundos) ..."
for key in "${!commands[@]}"
do
    avg=$(avg_time "${commands[$key]}")
    echo "$key: $avg"
done