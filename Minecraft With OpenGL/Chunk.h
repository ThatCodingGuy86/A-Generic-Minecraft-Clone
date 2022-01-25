#pragma once
#include <vector>
#include "VoxelData.h"
#include <iostream>
#include "FastNoiseLite.h"
#include <stdexcept>
#include "error.h"

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
	const double scale = 70;
	// Input scale of the noise (scales the input)
	const double scaleInput = 0.005;

	// The middle position for the terrain
	const int startYVal = 7;

	// XYZ to block array position
	int XYZtB(int X, int Y, int Z)
	{
		return Z * 16 * 32 + Y * 16 + Z;
	}

public:
	int ID;
	int x = 0;
	int y = 0;

	size_t chunkMeshSize = 0;

	Chunk(int ID, int seed) : ID(ID)
	{
		noise.SetSeed(seed);

		std::cout << "[CHUNKGEN/LOG]: Generating chunk " << std::to_string(ID) << "\n";

		// Generate blocks using 2D noise & throw an exception if a block is out of range
		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{

				//double heightmapVal = startYVal + (noise.GetNoise((((double)x) * scaleInput) + ((ID * 16) * scaleInput), (((double)y) * scaleInput) + ((ID * 16) * scaleInput)) * scale);
				int heightmapVal = random(-1, 1);

				blocks[x][startYVal + (int)heightmapVal][y] = 1;

				/*if ((int)heightmapVal > 15 or (int)heightmapVal < 1)
				{
					std::cout << (int)heightmapVal << '\n';
				}

				if ((int)heightmapVal > 1 and (int)heightmapVal <= 15)
				{
					
				}
				else
				{
					std::stringstream error;

					error << "[CHUNKGEN/FATAL]: Error: Attempted to place block at invalid position " << x << " : " << heightmapVal << " : " << y << "\n";

					err(error.str());

					//throw std::exception("chunkgen_fail");

				}*/
					
					
			}
		}
	}

	// Generates a mesh given a reference to a std::vector<float> (the vertices) and a reference to a size_t (the number of vertices)
	void genChunkMesh(std::vector<float>& verts, size_t& chunkVertSize)
	{
		int vertsAdded = 0;
		for (int y = 0; y < 32; y++)
		{
			for (int z = 0; z < 16; z++)
			{
				for (int x = 0; x < 16; x++)
				{
					if (blocks[x][y][z] != 0)
					{
						if (z - 1 <= 15 and z - 1 >= 0)
						{
							if (blocks[x][y][z - 1] == 0)
							{
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[0 + (6 * i) + (6 * 6) * 0] + x);
									verts.insert(verts.end(), voxelData[1 + (6 * i) + (6 * 6) * 0] + y);
									verts.insert(verts.end(), voxelData[2 + (6 * i) + (6 * 6) * 0] + z);
									verts.insert(verts.end(), voxelData[3 + (6 * i) + (6 * 6) * 0] + 16);
									verts.insert(verts.end(), voxelData[4 + (6 * i) + (6 * 6) * 0]);
									verts.insert(verts.end(), voxelData[5 + (6 * i) + (6 * 6) * 0]);
									vertsAdded += 1;
								}
							}
						}

						if (z + 1 <= 15 and z + 1 >= 0)
						{
							if (blocks[x][y][z + 1] == 0)
							{
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 1] + x);
									verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 1] + y);
									verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 1] + z);
									verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 1] + 16);
									verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 1]);
									verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 1]);
									vertsAdded += 1;
								}
							}
						}

						if (x - 1 <= 15 and x - 1 >= 0)
						{
							if (blocks[x - 1][y][z] == 0)
							{
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 2] + x);
									verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 2] + y);
									verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 2] + z);
									verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 2] + 16);
									verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 2]);
									verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 2]);
									vertsAdded += 1;
								}
							}
						}

						if (x + 1 <= 15 and x + 1 >= 0)
						{
							if (blocks[x + 1][y][z] == 0)
							{
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 3] + x);
									verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 3] + y);
									verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 3] + z);
									verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 3] + 16);
									verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 3]);
									verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 3]);
									vertsAdded += 1;
								}
							}
						}

						if (y - 1 <= 31 and y - 1 >= 0)
						{
							if (blocks[x][y - 1][z] == 0)
							{
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 4] + x);
									verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 4] + y);
									verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 4] + z);
									verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 4]);
									verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 4]);
									verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 4]);
									vertsAdded += 1;
								}
							}
						}

						if (y + 1 <= 31 and y + 1 >= 0)
						{
							if (blocks[x][y + 1][z] == 0)
							{
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 5] + x);
									verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 5] + y);
									verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 5] + z);
									verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 5] + 32);
									verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 5]);
									verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 5]);
									vertsAdded += 1;
								}
							}
						}
					}
				}
			}
		}
		if (vertsAdded != verts.size() / 6)
		{
			std::cout << vertsAdded << " : " << verts.size() / 6 << '\n';
		}
		chunkVertSize = vertsAdded;
		chunkMeshSize = vertsAdded;
	}
};

