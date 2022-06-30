#pragma once
#include <vector>
#include "globals.h"

class World
{
public:

	unsigned int chunkVAOs[chunkRenderSize * chunkRenderSize];
	unsigned int chunkVBOs[chunkRenderSize * chunkRenderSize];

	int generateChunks()
	{
		size_t chunkArrSize = 0;

		// Initialize chunk VAOs, VBOs, and EBOs
		glGenVertexArrays(chunkRenderSize * chunkRenderSize, chunkVAOs);
		glGenBuffers(chunkRenderSize * chunkRenderSize, chunkVBOs);

		// Generate chunks
		int i = 0;
		for (int y = 0; y < chunkRenderSize; y++)
		{
			for (int x = 0; x < chunkRenderSize; x++)
			{
				// Try/catch for exceptions thrown while generating chunks
				try
				{
					chunkArray.push_back(new Chunk(i, rand()));
				}
				catch (std::exception e)
				{
					std::stringstream error;

					error << "[MAIN_CHUNKGEN/FATAL]: " << e.what() << "\n";
					err(error.str());

					std::cout << "[MAIN_FERR_MSG]: " << "Failed to generate chunk " << i << ".\n";

					return (e.what()[0] + e.what()[1]) * -1;
				}


				glBindVertexArray(chunkVAOs[i]);
				glBindBuffer(GL_ARRAY_BUFFER, chunkVBOs[i]);

				std::vector<float> chunkVerts;

				chunkArray[i]->genChunkMesh(chunkVerts, chunkArrSize);


				glBufferData(GL_ARRAY_BUFFER, chunkVerts.size() * sizeof(float), &chunkVerts.front(), GL_STATIC_DRAW);

				// XYZ attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				// UV attribute
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				// Light attribute
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
				glEnableVertexAttribArray(2);
				i++;
			}
		}
		return 0;
	}
};