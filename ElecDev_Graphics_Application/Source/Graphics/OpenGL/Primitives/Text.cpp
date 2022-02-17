//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "Graphics/Entities/EntityManager.h"
#include <iostream>
#include "OpenGL/Primitives/Text.h"
#include "Graphics/Fonts/FontLoader.h"
#include "External/Misc/ConsoleColor.h"
#include "OpenGL/RendererGL.h"
#include "OpenGL/SceneGL.h"

//=============================================================================================================================================//
//  Constructor.																															   //
//=============================================================================================================================================//

// Writes the text to the buffer based on the font loaded in the constructor.
Text::Text(const std::string& text, const glm::vec3& position, const glm::vec4& color, float scale, 
		   VertexArrayObject<VertexDataTextured>* VAO, Font* font, Entity* parent,
		   const std::string& horizontalAlignment, const std::string& verticalAlignment)
	: Primitive<VertexDataTextured>(parent)
{
	// ---------- //
	// S E T U P  //
	// ---------- //

	m_VAO = VAO;
	m_trackedCenter = position;  // This does not track the center, but rather the initial cursor position.
								 // This does not affect functionality but the name does not make sense.
	m_cursorStart = position;
	m_textScale = scale;
	m_font = font;
	m_verticalAlign = verticalAlignment;
	m_horizontalAlign = horizontalAlignment;
	m_colour = color;

	// --------------------- //
	//  T E X T   Q U A D S  //
	// --------------------- //

	generateText(text);
}

void Text::generateText(const std::string& text)
{
	// Init.
	m_cursorStart = m_trackedCenter;

	// Return if text is empty.
	// Push primitive so that the VAO still keeps track of it.
	if (!text.size()) 
	{ 
		m_vertexBufferPos = m_VAO->m_vertexCPU.size();
		m_indexBufferPos = m_VAO->m_indexCPU.size();
		m_VAO->pushPrimitive(this);
		return; 
	}

	// Create variables and reserve memory.
	std::vector<std::unique_ptr<VertexDataTextured>> vertices;
	std::vector<unsigned> indices;
	int charCount = text.length();
	indices.reserve((charCount + 1) * 6);	// Add one to the char count for
	vertices.reserve((charCount + 1) * 4);	// the text box.

	// Calculate the string length with kerning. 
	// (Kerning does not apply to the first character)
	m_textLength = 0;
	m_textLength += m_font->characterDictionary[text[0]].xAdvance;  
	for (int i = 1; i < charCount; i++)
	{
		// Retrieve kerning value from dictionary.
		float kerning = 0;
		// Create kerning pair.
		std::pair kerningPair = std::pair(m_font->characterDictionary.at(text[i]).id,
										  m_font->characterDictionary.at(text[i - 1]).id);

		// Check if kerning exists for current pair.  OPTIMIZE!
		if (m_font->kerningDictionary.count(kerningPair))	
			kerning = m_font->kerningDictionary.at(kerningPair);

		// Add character length to total.
		m_textLength += m_font->characterDictionary[text[i]].xAdvance + kerning;
	}

	// ------------------- //
	//  A L I G N M E N T  //
	// ------------------- //

	// Horizontal alignment.
	if (m_horizontalAlign == "C" || m_horizontalAlign == "c") 
	{ 
		m_cursorStart.x = m_cursorStart.x - (m_textLength / 2) * m_textScale; 
	}
	else if (m_horizontalAlign == "R" || m_horizontalAlign == "r") 
	{ 
		const Character& endChar = m_font->characterDictionary.at(text[charCount-1]);
		float offset = endChar.xAdvance - endChar.xPlaneBounds[1];
		m_cursorStart.x = m_cursorStart.x - (m_textLength - offset) * m_textScale;
	}
	else if (m_horizontalAlign == "L" || m_horizontalAlign == "l") 
	{ 
		const Character& initialChar = m_font->characterDictionary.at(text[0]);
		m_cursorStart.x = m_cursorStart.x - (initialChar.xPlaneBounds[0]) * m_textScale;
	}
	// Display error.
	else	
	{ 
		std::cout << red << "\n[OPENGL] [ERROR]: " << white << "'" << m_horizontalAlign << "' is not a valid horizontal alignment.\n"; return; 
	}

	// Vertical alignment.
	if (m_verticalAlign == "C" || m_verticalAlign == "c") 
	{
		m_cursorStart.y = m_cursorStart.y - ((m_font->ascender + m_font->descender) * m_textScale) / 2; 
	}
	else if (m_verticalAlign == "T" || m_verticalAlign == "t") 
	{
		m_cursorStart.y = m_cursorStart.y - ((m_font->ascender + m_font->descender) * m_textScale); 
	}
	else if (m_verticalAlign == "U" || m_verticalAlign == "u") 
	{
		m_cursorStart.y = m_cursorStart.y - ((m_font->descender) * m_textScale); 
	}
	else if (m_verticalAlign == "B" || m_verticalAlign == "b") 
	{
		// Bottom is the default setting.
	}
	// Display error.
	else	
	{
		std::cout << red << "\n[OPENGL] [ERROR]: " << white << "'" << m_verticalAlign << "' is not a valid vertical alignment.\n"; return; 
	}

	// ----------------- //
	//  T E X T   B O X  //
	// ----------------- //

	// Vertices layout.
	//
	//   2 ------------------ 3
	//	 |   /\	   /\	 /\	  |
	//   |	/__\  /__\	/__\  |
	//   | /    \/    \/    \ |
	//   1 ------------------ 4
	// 
	// The box default is an invisible quad.
	// Will also be rendered just behind the text so
	// that it does not interfere with the text when 
	// made visible.
	float boxZPos = m_cursorStart.z - 0.001;

	// -----------------------
	// Vertex 1.
	vertices.emplace_back(std::make_unique<VertexDataTextured>
	(
		glm::vec3(m_cursorStart.x, m_cursorStart.y + m_font->descender * m_textScale, boxZPos),
		m_boxColor, glm::vec2(0.f, 0.f), 0, m_entityID)
	);
	// -----------------------
	// Vertex2.
	vertices.emplace_back(std::make_unique<VertexDataTextured>
	(
		glm::vec3(m_cursorStart.x, m_cursorStart.y + m_font->ascender * m_textScale, boxZPos),
		m_boxColor, glm::vec2(0.f, 1.f), 0, m_entityID)
	);
	// -----------------------
	// Vertex 3.
	vertices.emplace_back(std::make_unique<VertexDataTextured>
	(
		glm::vec3(m_cursorStart.x + m_textLength * m_textScale, m_cursorStart.y + m_font->ascender * m_textScale, boxZPos),
		m_boxColor, glm::vec2((1.f, 1.f)), 0, m_entityID)
	);
	// -----------------------
	// Vertex 4.
	vertices.emplace_back(std::make_unique<VertexDataTextured>
	(
		glm::vec3(m_cursorStart.x + m_textLength * m_textScale, m_cursorStart.y + m_font->descender * m_textScale, boxZPos),
		m_boxColor, glm::vec2((0.f, 1.f)), 0, m_entityID)
	);
	// -----------------------
	// Indices.
	indices.insert(indices.end(),
		{
			0 + m_vertexCount,
			1 + m_vertexCount,
			2 + m_vertexCount,
			2 + m_vertexCount,
			3 + m_vertexCount,
			0 + m_vertexCount
		});
	// Increment counts.
	m_vertexCount += 4;
	m_indexCount += 6;
	// -----------------------

	// ----------------------------- //
	//  T E X T   R E N D E R I N G  //
	// ----------------------------- //

	// Vertices layout.
	//
	//   2 ------ 3
	//	 |   /\   |
	//   |	/__\  |
	//   | /    \ |
	//   1 ------ 4
	//
	// The texture ID '1' is reserved for font atlasses.
	// Our engine does not need the ability to be able to render text
	// with more than one font.  If this is required at a later stage we
	// can implement a system that binds the required font texture atlas
	// as needed before the draw calls.

	// Generate a quad for each character.
	float totalAdvance = 0;
	for (int i = 0; i < charCount; i++)
	{
		// -----------------------
		// Load character.
		const Character& c = m_font->characterDictionary.at(text[i]);
		// Retrieve kerning value from dictionary.
		float kerning = 0;
		// Kerning does not apply to the first character.
		if (i)  
		{
			// Create kerning pair.
			std::pair kerningPair = std::pair(m_font->characterDictionary.at(text[i]).id,
											  m_font->characterDictionary.at(text[i - 1]).id);

			// Check if kerning exists for current pair.
			if (m_font->kerningDictionary.count(kerningPair))	
				kerning = m_font->kerningDictionary.at(kerningPair);
		}

		// -----------------------
		// Vertex 1.
		vertices.emplace_back(std::make_unique<VertexDataTextured>
		(
			glm::vec3(m_cursorStart.x + (totalAdvance + c.xPlaneBounds[0] + kerning) * m_textScale,
					  m_cursorStart.y + (c.yPlaneBounds[0]) * m_textScale,
					  m_cursorStart.z),
			m_colour, glm::vec2(c.xTextureCoords[0], c.yTextureCoords[0]), 1, m_entityID)
		);
		// -----------------------
		// Vertex2.
		vertices.emplace_back(std::make_unique<VertexDataTextured>
		(
			glm::vec3(m_cursorStart.x + (totalAdvance + c.xPlaneBounds[0] + kerning) * m_textScale,
					  m_cursorStart.y + (c.yPlaneBounds[0] + c.height) * m_textScale,
					  m_cursorStart.z),
			m_colour, glm::vec2(c.xTextureCoords[0], c.yTextureCoords[1]), 1, m_entityID)
		);
		// -----------------------
		// Vertex 3.
		vertices.emplace_back(std::make_unique<VertexDataTextured>
		(
			glm::vec3(m_cursorStart.x + (totalAdvance + c.xPlaneBounds[0] + c.width + kerning) * m_textScale,
					  m_cursorStart.y + (c.yPlaneBounds[0] + c.height) * m_textScale,
					  m_cursorStart.z),
			m_colour, glm::vec2(c.xTextureCoords[1], c.yTextureCoords[1]), 1, m_entityID)
		);
		// -----------------------
		// Vertex 4.
		vertices.emplace_back(std::make_unique<VertexDataTextured>
		(
			glm::vec3(m_cursorStart.x + (totalAdvance + c.xPlaneBounds[0] + c.width + kerning) * m_textScale,
					  m_cursorStart.y + (c.yPlaneBounds[0]) * m_textScale,
					  m_cursorStart.z),
			m_colour, glm::vec2(c.xTextureCoords[1], c.yTextureCoords[0]), 1, m_entityID)
		);
		// -----------------------
		// Indices.
		indices.insert(indices.end(),
			{
				0 + m_vertexCount,
				1 + m_vertexCount,
				2 + m_vertexCount,
				2 + m_vertexCount,
				3 + m_vertexCount,
				0 + m_vertexCount
			});
		// Move cursor for next character.
		totalAdvance += c.xAdvance + kerning;
		// Increment counts.
		m_vertexCount += 4;
		m_indexCount += 6;
		// -----------------------
	}

	// Write data to VAO.
	m_VAO->appendVertexData(vertices, indices, &m_vertexBufferPos, &m_indexBufferPos);
	m_VAO->pushPrimitive(this);
}

//=============================================================================================================================================//
//  Text manipulation.																													       //
//=============================================================================================================================================//

void Text::updateText(const std::string& text) 
{
	wipeGPU();
	generateText(text);
}

void Text::setBoxColour(const glm::vec4& colour) 
{ 
	m_boxColor = colour;
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + 4; i++) 
		m_VAO->m_vertexCPU[i]->data.color = colour;
	m_VAO->sync(this);
}

void Text::setColor(const glm::vec4& color) 
{
	m_colour = color;
	for (int i = m_vertexBufferPos + 4; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i]->data.color = color;
	m_VAO->sync(this);
}

void Text::setLayer(float layer)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + 4; i++)
		m_VAO->m_vertexCPU[i]->data.position.z = layer - 0.001;
	for (int i = m_vertexBufferPos + 4; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i]->data.position.z = layer;
	m_VAO->sync(this);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//