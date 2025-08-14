# BUILD

```shell
sudo apt install nlohmann-json3-dev
mkdir build
cd build
cmake ..
cmake --build .
./maze_room.out
```

## Build for windows (not from)

```shell
sudo apt update
sudo apt install mingw-w64 wine nlohmann-json3-dev

mkdir build-windows
cd build-windows

cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw64.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .

wine ./maze_room.exe
```
