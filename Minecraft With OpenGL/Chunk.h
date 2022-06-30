#pragma once
#include <vector>
#include "VoxelData.h"
#include <iostream>
#include "FastNoiseLite.h"
#include <stdexcept>
#include "error.h"
#include <random>
#include "globals.h"

// Util function for generating a random number within a range (why is this not in std::random already)
template<typename T>
T random(T range_from, T range_to) {
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<T>    distr(range_from, range_to);
	return distr(generator);
}


class Chunk
{
	// Blocks array
	int8_t blocks[16][32][16] = { {{0}} };

	// Noise class instance
	FastNoiseLite noise;

	// Scale of the noise (scales the output)
	const double scale = 2;

	// Input scale of the noise
	const double scaleInput = 0.001;

	// The middle position for the terrain
	const int startYVal = 7;

	// XYZ to block array position
	int XYZtB(int X, int Y, int Z)
	{
		return X * 16 * 32 + Y * 16 + Z;
	}

public:
	int ID;
	int chunkX = 0;
	int chunkZ = 0;

	size_t chunkMeshSize = 0;

	Chunk(int ID, int seed) : ID(ID)
	{
		noise.SetSeed(seed);

		chunkX = ID % 10;
		chunkZ = ID / 10;

		std::cout << "[CHUNKGEN/LOG]: Generating chunk " << std::to_string(ID) << "\n";

		// Generate blocks using 2D noise & throw an exception if a block is out of range

		for (int y = 0; y < 32; y++)
		{
			for (int z = 0; z < 16; z++)
			{
				for (int x = 0; x < 16; x++)
				{
					double heightmapVal = startYVal + ((noise.GetNoise((double)x + chunkX / scaleInput, (double)z + chunkZ / scaleInput)) * scale);
					//int heightmapVal = random(6, 7);

					/*if (true)
					{
						blocks[x][startYVal + (int)heightmapVal][y] = 1;
						//blocks[x][startYVal + (int)((y % 2) * -1)][y] = 1;
						//blocks[x][startYVal + (int)0][y] = 1;
					}*/


					if ((int)heightmapVal > 32 or (int)heightmapVal < 1)
					{
						std::cout << (int)heightmapVal << '\n';
					}

					if ((int)heightmapVal > 1 and (int)heightmapVal <= 32)
					{
						blocks[x][(int)heightmapVal][z] = 1;
					}
					else
					{
						std::stringstream error;

						error << "[CHUNKGEN/FATAL]: Error: Attempted to place block at invalid position " << x << " : " << (int)heightmapVal << " : " << z << "\n";

						err(error.str());

						throw std::exception("chunkgen_fail");
					}
				}
			}
		}
	}

	uint8_t getBlock(int x, int y, int z)
	{
		return blocks[x][y][z];
	}

	// Generates a mesh given a reference to a std::vector<float> (the vertices) 
	// and a reference to a size_t (the number of vertices)
	void genChunkMesh(std::vector<float>& verts, size_t& chunkVertSize)
	{
		unsigned long long vertsAdded = 0;
		for (int y = 0; y < 32; y++)
		{
			for (int z = 0; z < 16; z++)
			{
				for (int x = 0; x < 16; x++)
				{
					// If the current voxel is not air
					if (blocks[x][y][z] != 0)
					{
						// Bounds check
						if (z - 1 <= 15 and z - 1 >= 0)
						{
							// If the voxel in the Z - 1 direction is air
							if (blocks[x][y][z - 1] == 0)
							{
								// Generate the back face
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[0][i][0] + x);
									verts.insert(verts.end(), voxelData[0][i][1] + y);
									verts.insert(verts.end(), voxelData[0][i][2] + z);
									verts.insert(verts.end(), voxelData[0][i][3] + 16);
									verts.insert(verts.end(), voxelData[0][i][4]);
									verts.insert(verts.end(), voxelData[0][i][5]);
									vertsAdded += 1;
								}
							}
						}

						// Bounds check
						if (z + 1 <= 15 and z + 1 >= 0)
						{
							// If the voxel in the Z + 1 direction is air
							if (blocks[x][y][z + 1] == 0)
							{
								// Generate the front face
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[1][i][0] + x);
									verts.insert(verts.end(), voxelData[1][i][1] + y);
									verts.insert(verts.end(), voxelData[1][i][2] + z);
									verts.insert(verts.end(), voxelData[1][i][3] + 16);
									verts.insert(verts.end(), voxelData[1][i][4]);
									verts.insert(verts.end(), voxelData[1][i][5]);
									vertsAdded += 1;
								}
							}
						}

						// Bounds check
						if (x - 1 <= 15 and x - 1 >= 0)
						{
							// If the voxel in the X - 1 direction is air
							if (blocks[x - 1][y][z] == 0)
							{
								// Generate the left face
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[2][i][0] + x);
									verts.insert(verts.end(), voxelData[2][i][1] + y);
									verts.insert(verts.end(), voxelData[2][i][2] + z);
									verts.insert(verts.end(), voxelData[2][i][3] + 16);
									verts.insert(verts.end(), voxelData[2][i][4]);
									verts.insert(verts.end(), voxelData[2][i][5]);
									vertsAdded += 1;
								}
							}
						}

						// Bounds check
						if (x + 1 <= 15 and x + 1 >= 0)
						{
							// If the voxel in the X + 1 direction is air
							if (blocks[x + 1][y][z] == 0)
							{
								// Generate the right face
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[3][i][0] + x);
									verts.insert(verts.end(), voxelData[3][i][1] + y);
									verts.insert(verts.end(), voxelData[3][i][2] + z);
									verts.insert(verts.end(), voxelData[3][i][3] + 16);
									verts.insert(verts.end(), voxelData[3][i][4]);
									verts.insert(verts.end(), voxelData[3][i][5]);
									vertsAdded += 1;
								}
							}
						}

						// Bounds check
						if (y - 1 <= 31 and y - 1 >= 0)
						{
							// If the voxel in the Y - 1 direction is air
							if (blocks[x][y - 1][z] == 0)
							{
								// Generate the bottom face
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[4][i][0] + x);
									verts.insert(verts.end(), voxelData[4][i][1] + y);
									verts.insert(verts.end(), voxelData[4][i][2] + z);
									verts.insert(verts.end(), voxelData[4][i][3]);
									verts.insert(verts.end(), voxelData[4][i][4]);
									verts.insert(verts.end(), voxelData[4][i][5]);
									vertsAdded += 1;
								}
							}
						}

						// Bounds check
						if (y + 1 <= 31 and y + 1 >= 0)
						{
							// If the voxel in the Y + 1 direction is air
							if (blocks[x][y + 1][z] == 0)
							{
								// Generate the top face
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[5][i][0] + x);
									verts.insert(verts.end(), voxelData[5][i][1] + y);
									verts.insert(verts.end(), voxelData[5][i][2] + z);
									verts.insert(verts.end(), voxelData[5][i][3] + 32);
									verts.insert(verts.end(), voxelData[5][i][4]);
									verts.insert(verts.end(), voxelData[5][i][5]);
									vertsAdded += 1;
								}
							}
						}
					}
				}
			}
		}
		chunkVertSize = vertsAdded;
		chunkMeshSize = vertsAdded;
	}
};

