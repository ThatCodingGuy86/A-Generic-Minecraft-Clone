#pragma once
#include <vector>
#include "VoxelData.h"
#include <iostream>
#include "perlin_noise.h"
#include <stdexcept>
#include "error.h"

class Chunk
{
	// Blocks array
	int8_t blocks[16][16][16] = { {{0}} };

	// PerlinNoise initilizer(s?)
	const siv::PerlinNoise::seed_type seed = 845275u;
	const siv::PerlinNoise perlin{ seed };
	// Scale of the Perlin Noise (scales the output and not the input)
	const float scale = 4;
	// Scale of the Perlin Noise (scales the input and not the output)
	const float scaleInput = 4;

	// The middle position for the terrain
	const int startYVal = 5;

	

public:
	int ID;
	int x = 0;
	int y = 0;

	// Self-explanatory
	int chunkMeshSize = 0;

	Chunk(int _ID, int seed)
	{
		ID = _ID;

		std::cout << "[CHUNKGEN/LOG]: Generating chunk " << std::to_string(ID) << "\n";

		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{
				// Generate blocks using 2D Perlin Noise & raise an out_of_range execption if a block is, well,
				// out of range

				float heightmapVal = (perlin.noise2D((((float)x) / scaleInput) + ((ID * 16) / scaleInput), (((float)y) / scaleInput) + ((ID * 16) / scaleInput)) * scale);
				

				if (startYVal + (int)heightmapVal >= 0 and startYVal + (int)heightmapVal <= 15)
				{
					blocks[x][startYVal + (int)heightmapVal][y] = 1;
				}
				else
				{
					std::stringstream error;

					error << "[CHUNKGEN/FATAL]: Error: Attempted to place block at invalid position " << x << " : " << startYVal + (int)(perlin.noise2D((((float)x) / scaleInput) + ((ID * 16) / scaleInput), (((float)y) / scaleInput) + ((ID * 16) / scaleInput)) + ((ID * 16) / scaleInput)) << " : " << y << "\n";

					err(error.str());

					throw std::exception("chunkgen_fail");

				}
					
					
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
						if (z - 1 >= 0 and z - 1 <= 15)
						{
							if (blocks[x][y][z - 1] == 0)
							{
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[0 + (6 * i)] + x);
									verts.insert(verts.end(), voxelData[1 + (6 * i)] + y);
									verts.insert(verts.end(), voxelData[2 + (6 * i)] + z);
									verts.insert(verts.end(), voxelData[3 + (6 * i)] + 16);
									verts.insert(verts.end(), voxelData[4 + (6 * i)]);
									verts.insert(verts.end(), voxelData[5 + (6 * i)]);
								}
							}
						}

						if (z + 1 <= 15 and z + 1 >= 0)
						{
							if (blocks[x][y][z + 1] == 0)
							{
								for (int i = 0; i < 6; i++)
								{
									verts.insert(verts.end(), voxelData[(0 + (6 * i)) + 6 * 6] + x);
									verts.insert(verts.end(), voxelData[(1 + (6 * i)) + 6 * 6] + y);
									verts.insert(verts.end(), voxelData[(2 + (6 * i)) + 6 * 6] + z);
									verts.insert(verts.end(), voxelData[(3 + (6 * i)) + 6 * 6] + 16);
									verts.insert(verts.end(), voxelData[(4 + (6 * i)) + 6 * 6]);
									verts.insert(verts.end(), voxelData[(5 + (6 * i)) + 6 * 6]);
								}
							}
						}

						if (x - 1 >= 0 and x - 1 <= 15)
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

						if (y - 1 >= 0 and y - 1 <= 15)
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

