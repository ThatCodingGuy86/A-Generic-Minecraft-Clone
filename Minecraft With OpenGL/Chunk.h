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
	const siv::PerlinNoise::seed_type seed = 1u;
	const siv::PerlinNoise perlin{ seed };
	// Scale of the Perlin Noise (scales the output and not the input)
	const float scale = 4;

public:
	int ID;
	int x = 0;
	int y = 0;

	Chunk(int _ID, int seed)
	{
		ID = _ID;

		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{
				// Generate blocks using 2D Perlin Noise & raise an out_of_range execption if a block is, well,
				// out of range

				if (1 + (int)(perlin.noise2D((((float)x) / 4) + (ID * 16), (((float)y) / 4) + (ID * 16)) * scale) >= 0 and 1 + (int)(perlin.noise2D((((float)x) / 4) + (ID * 16), (((float)y) / 4) + (ID * 16)) * scale) < 17)
				{
					blocks[x][4 + (int)(perlin.noise2D((((float)x) / 4) + (ID * 16), (((float)y) / 4) + (ID * 16)) * scale)][y] = 1;
					//std::cout << 1 + (int)(perlin.noise2D((((float)x) / 4) + (ID * 16), (((float)y) / 4) + (ID * 16)) * scale) << "\n";
				}
				else
				{
					std::stringstream error;

					error << "[CHUNKGEN/FATAL]: Error: Attempted to place block at invalid position " << x << " : " << 1 + (int)(perlin.noise2D((((float)x) / 4) + (ID * 16), (((float)y) / 4) + (ID * 16)) + (ID * 16)) << " : " << y << std::endl;

					err(error.str());

					throw std::out_of_range("chunkgen_fail");

				}
					
					
			}
		}
	}

	// Generates a mesh given a reference to a std::vector<float> and a reference to a int (the number of vertices)
	void genChunkMesh(std::vector<float>& verts, int& chunkVertSize)
	{
		for (int y = 0; y < 16; y++)
		{
			for (int z = 0; z < 16; z++)
			{
				for (int x = 0; x < 16; x++)
				{

					//if (blocks[x][y][z] != 0 and z - 1 > -1 and blocks[x][y][z - 1] == 0)
					//{
					//	for (int i = 0; i < 6; i++)
					//	{
					//		verts.insert(verts.end(), voxelData[0 + (6 * i)] + x);
					//		verts.insert(verts.end(), voxelData[1 + (6 * i)] + y);
					//		verts.insert(verts.end(), voxelData[2 + (6 * i)] + z);
					//		verts.insert(verts.end(), voxelData[3 + (6 * i)] + 16);
					//		verts.insert(verts.end(), voxelData[4 + (6 * i)]);
					//		verts.insert(verts.end(), voxelData[5 + (6 * i)]);
					//	}
					//}

					//if (blocks[x][y][z] != 0 and z + 1 < 16 and blocks[x][y][z + 1] == 0)
					//{
					//	for (int i = 0; i < 6; i++)
					//	{
					//		verts.insert(verts.end(), voxelData[(0 + (6 * i)) + 6 * 6] + x);
					//		verts.insert(verts.end(), voxelData[(1 + (6 * i)) + 6 * 6] + y);
					//		verts.insert(verts.end(), voxelData[(2 + (6 * i)) + 6 * 6] + z);
					//		verts.insert(verts.end(), voxelData[(3 + (6 * i)) + 6 * 6] + 16);
					//		verts.insert(verts.end(), voxelData[(4 + (6 * i)) + 6 * 6]);
					//		verts.insert(verts.end(), voxelData[(5 + (6 * i)) + 6 * 6]);
					//	}
					//}

					//if (blocks[x][y][z] != 0 and x - 1 > -1 and blocks[x - 1][y][z] == 0)
					//{
					//	for (int i = 0; i < 6; i++)
					//	{
					//		verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 2] + x);
					//		verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 2] + y);
					//		verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 2] + z);
					//		verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 2] + 16);
					//		verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 2]);
					//		verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 2]);
					//	}
					//}

					//if (blocks[x][y][z] != 0 and x + 1 < 16 and blocks[x + 1][y][z] == 0)
					//{
					//	for (int i = 0; i < 6; i++)
					//	{
					//		verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 3] + x);
					//		verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 3] + y);
					//		verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 3] + z);
					//		verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 3] + 16);
					//		verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 3]);
					//		verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 3]);
					//	}
					//}

					//if (blocks[x][y][z] != 0 and y - 1 > -1 and blocks[x][y - 1][z] == 0)
					//{
					//	for (int i = 0; i < 6; i++)
					//	{
					//		verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 4] + x);
					//		verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 4] + y);
					//		verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 4] + z);
					//		verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 4]);
					//		verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 4]);
					//		verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 4]);
					//	}
					//}

					//if (blocks[x][y][z] != 0 and y + 1 < 16 and blocks[x][y + 1][z] == 0)
					//{
					//	for (int i = 0; i < 6; i++)
					//	{
					//		verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 5] + x);
					//		verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 5] + y);
					//		verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 5] + z);
					//		verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 5] + 32);
					//		verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 5]);
					//		verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 5]);
					//	}
					//}

					

					if (blocks[x][y][z] != 0 and (y > 0 and y < 16))
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

						for (int i = 0; i < 6; i++)
						{
							verts.insert(verts.end(), voxelData[(0 + (6 * i)) + 6 * 6] + x);
							verts.insert(verts.end(), voxelData[(1 + (6 * i)) + 6 * 6] + y);
							verts.insert(verts.end(), voxelData[(2 + (6 * i)) + 6 * 6] + z);
							verts.insert(verts.end(), voxelData[(3 + (6 * i)) + 6 * 6] + 16);
							verts.insert(verts.end(), voxelData[(4 + (6 * i)) + 6 * 6]);
							verts.insert(verts.end(), voxelData[(5 + (6 * i)) + 6 * 6]);
						}


						for (int i = 0; i < 6; i++)
						{
							verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 2] + x);
							verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 2] + y);
							verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 2] + z);
							verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 2] + 16);
							verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 2]);
							verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 2]);
						}

						for (int i = 0; i < 6; i++)
						{
							verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 3] + x);
							verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 3] + y);
							verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 3] + z);
							verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 3] + 16);
							verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 3]);
							verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 3]);
						}

						for (int i = 0; i < 6; i++)
						{
							verts.insert(verts.end(), voxelData[(0 + (6 * i)) + (6 * 6) * 4] + x);
							verts.insert(verts.end(), voxelData[(1 + (6 * i)) + (6 * 6) * 4] + y);
							verts.insert(verts.end(), voxelData[(2 + (6 * i)) + (6 * 6) * 4] + z);
							verts.insert(verts.end(), voxelData[(3 + (6 * i)) + (6 * 6) * 4]);
							verts.insert(verts.end(), voxelData[(4 + (6 * i)) + (6 * 6) * 4]);
							verts.insert(verts.end(), voxelData[(5 + (6 * i)) + (6 * 6) * 4]);
						}

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
		chunkVertSize = verts.size() / 6; // Divided by 6 because 1 vert definition is 6 elements long
	}
};

