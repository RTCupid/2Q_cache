# 2Q_cache
Implementation of the 2Q cache algorithm and comparison of its efficiency with the "perfect" cache

## Content
- [0. Run the program](#run-the-program)
- [1. Annotation](#annotation)
- [2. Introduction](#introduction)
- [3. Methods](#methods)
- [4. Results](#results)
- [5. Сonclusions](#conclusions)

## Annotation
Implementations of the 2q cache and the Belady cache have been developed. A comparison of their efficiency revealed that the 2q cache has good precision.

## Introduction
In industrial applications, data is frequently retrieved from remote devices or servers. This downloading process is often time-consuming and can significantly degrade overall system performance.

While one potential solution is to create faster download methods, their development is typically costly and does not always lead to substantial gains.

A more efficient approach is to implement a local data store, or cache, which holds frequently accessed information. The strategy used to predict which data will be needed again is managed by a caching algorithm. This program implements two such algorithms—the 2Q and the Belady algorithms—and provides a comparative analysis of their effectiveness.

## Methods
### 2Q cache
<div align="center"><img src="img/cache_2q.jpg"></div><br>
  <div align="center"> Fig 1. 2Q cache</div><br>

### Belady cache
<div align="center"><img src="img/cache_belady.jpg"></div><br>
  <div align="center"> Fig 2. Belady cache</div><br>

## Results
## Сonclusions

## Run the program

1) Clone the repository
```shell
git clone ...
```
2) Go to repository folder
```bash
cd 2Q_cache
```
3) Create a build folder and go to it
```bash
mkdir build && cd build
```
4) Generate a file for assembly using cmake
```bash
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
```
5) Assemble the project
```bash
cmake --build .
```
6) Run the executable file of 2q cache
```bash
./cache_2q
```
6) *Or run the executable file of imitation belady cache
```bash
./cache_belady
```
6) **Or run tests
```bash
./cache_2q_tests
./cache_belady_tests
```
