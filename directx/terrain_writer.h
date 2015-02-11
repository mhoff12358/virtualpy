#include <array>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <memory>

#include "terrain_point.h"

#ifndef __TERRAIN_WRITER_H_INCLUDED__
#define __TERRAIN_WRITER_H_INCLUDED__

class TerrainWriter {
public:
	TerrainWriter() { point_file.reset(NULL); };

	void prepareFileForWrite(const char* file_name);
	void prepareFileForRead(const char* file_name);
	void releaseFile();

	void readAllPointsFromFile(std::unordered_map<std::array<int, 2>, TerrainPoint>& loaded_unordered_map);
	void writeAllPointsToFile(const std::unordered_map<std::array<int, 2>, TerrainPoint>& loaded_unordered_map);

private:
	void writeTerrainToFile(std::fstream* fs, const TerrainPoint& point);
	void readTerrainFromFile(std::fstream* fs, TerrainPoint& point);

	void writeIndexToFile(std::fstream* fs, const std::array<int, 2>& index);
	void readIndexFromFile(std::fstream* fs, std::array<int, 2>& index);

	void writeAllPointsToFile(std::fstream* fs, const std::unordered_map<std::array<int, 2>, TerrainPoint>& points);
	void readAllPointsFromFile(std::fstream* fs, std::unordered_map<std::array<int, 2>, TerrainPoint>& loaded_unordered_map);

	void readPointsFromFile(std::fstream* fs, std::unordered_map<std::array<int, 2>, TerrainPoint>& loaded_unordered_map, int num_points);

	bool logging = true;
	std::unique_ptr<std::fstream> point_file;

	static int terrainWriteSize;
	static int indexWriteSize;
	static int pointWriteSize;
};

#endif