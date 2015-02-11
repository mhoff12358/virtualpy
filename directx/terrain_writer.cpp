#include "terrain_writer.h"

// Public functions
void TerrainWriter::prepareFileForWrite(const char* file_name) {
	point_file.reset(new std::fstream);
	point_file->open(file_name, std::fstream::out | std::fstream::binary);
}

void TerrainWriter::prepareFileForRead(const char* file_name) {
	point_file.reset(new std::fstream);
	point_file->open(file_name, std::fstream::in | std::fstream::binary);
}

void TerrainWriter::releaseFile() {
	if (point_file.get() != NULL) {
		point_file->close();
		point_file.reset(NULL);
	}
}

void TerrainWriter::readAllPointsFromFile(std::unordered_map<std::array<int, 2>, TerrainPoint>& loaded_unordered_map) {
	readAllPointsFromFile(point_file.get(), loaded_unordered_map);
}

void TerrainWriter::writeAllPointsToFile(const std::unordered_map<std::array<int, 2>, TerrainPoint>& loaded_unordered_map) {
	writeAllPointsToFile(point_file.get(), loaded_unordered_map);
}

// Private functions
void TerrainWriter::writeTerrainToFile(std::fstream* fs, const TerrainPoint& point) {
	fs->write((char *)&point.height, sizeof(point.height));
	fs->write((char *)point.color.data(), sizeof(float)*3);
}

void TerrainWriter::readTerrainFromFile(std::fstream* fs, TerrainPoint& point) {
	fs->read((char *)&point.height, sizeof(point.height));
	fs->read((char *)point.color.data(), sizeof(float)*3);
}

void TerrainWriter::writeIndexToFile(std::fstream* fs, const std::array<int, 2>& index) {
	fs->write((char *)index.data(), sizeof(int)*2);
}

int TerrainWriter::terrainWriteSize = sizeof(float) + sizeof(float)*3;
int TerrainWriter::indexWriteSize = sizeof(int)*2;
int TerrainWriter::pointWriteSize = terrainWriteSize+indexWriteSize;

void TerrainWriter::readIndexFromFile(std::fstream* fs, std::array<int, 2>& index) {
	fs->read((char *)index.data(), sizeof(int)*2);
}

void TerrainWriter::writeAllPointsToFile(std::fstream* fs, const std::unordered_map<std::array<int, 2>, TerrainPoint>& points) {
	for (const std::pair<std::array<int, 2>, TerrainPoint>& terrain_point : points) {
		if (logging) {
			std::cout << "Writing " << terrain_point.first[0] << ", " << terrain_point.first[1] <<
			": " << terrain_point.second.height << std::endl;
		}
		writeIndexToFile(fs, terrain_point.first);
		writeTerrainToFile(fs, terrain_point.second);
	}
}

void TerrainWriter::readAllPointsFromFile(std::fstream* fs, std::unordered_map<std::array<int, 2>, TerrainPoint>& points) {
	std::streampos start_position = fs->tellg();
	fs->seekg(0, std::ios::end);
	int bytes_to_read = fs->tellg()-start_position;
	fs->seekg(0, std::ios::beg);
	readPointsFromFile(fs, points, bytes_to_read/TerrainWriter::pointWriteSize);
}

void TerrainWriter::readPointsFromFile(std::fstream* fs, std::unordered_map<std::array<int, 2>, TerrainPoint>& points, int num_points) {
	for (int i = 0; i < num_points; i++) {
		std::array<int, 2> new_index;
		TerrainPoint new_terrain;
		readIndexFromFile(fs, new_index);
		readTerrainFromFile(fs, new_terrain);
		if (logging) {
			std::cout << "Loading " << new_index[0] << ", " << new_index[1] <<
			": " << new_terrain.height << std::endl;
		}
		points[new_index] = new_terrain;
	}
}
