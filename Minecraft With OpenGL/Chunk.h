#pragma once
#include <vector>
#include "VoxelData.h"
#include <iostream>
#include "FastNoiseLite.h"
#include <stdexcept>
#include "error.h"

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
	int8_t blocks[16][16][16] = { {{0}} };

	// PerlinNoise initilizer(s?)
	FastNoiseLite noise;
	// Scale of the Perlin Noise (scales the output and not the input)
	const float scale = 70;
	// Scale of the Perlin Noise (scales the input and not the output)
	const float scaleInput = 0.005;

	// The middle position for the terrain
	const int startYVal = 7;

	

public:
	int ID;
	int x = 0;
	int y = 0;

	// Self-explanatory
	int chunkMeshSize = 0;

	Chunk(int _ID, int seed)
	{
		ID = _ID;

		noise.SetSeed(seed);

		std::cout << "[CHUNKGEN/LOG]: Generating chunk " << std::to_string(ID) << "\n";

		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{
				// Generate blocks using 2D noise & throw an exception if a block is out of range

				float heightmapVal = startYVal + (noise.GetNoise((((float)x) * scaleInput) + ((ID * 16) * scaleInput), (((float)y) * scaleInput) + ((ID * 16) * scaleInput)) * scale);
				heightmapVal = random(-1, 1);

				blocks[x][8 + (int)heightmapVal][y] = 1;

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

	// Generates a mesh given a reference to a std::vector<float> (the vertices) and a reference to a int (the number of vertices)
	void genChunkMesh(std::vector<float>& verts, int& chunkVertSize)
	{
		int vertsAdded = 0;

		for (int y = 0; y < 16; y++)
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
								}
							}
						}

						if (y - 1 <= 15 and y - 1 >= 0)
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
								}
							}
						}

						if (y + 1 <= 15 and y + 1 >= 0)
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
								}
							}
						}
					}
				}
			}
		}
		
		chunkVertSize = verts.size() / 6; // Divided by 6 because 1 vert definition is 6 elements long
		chunkMeshSize = verts.size() / 6;
	}
};

