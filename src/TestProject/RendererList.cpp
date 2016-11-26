#include "RendererList.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

bool RendererList::DEBUG_DRAW = false;
bool RendererList::WIREFRAME_MODE = false;

/*void RendererList::Init(int width, int height) {
glGenFramebuffers(1, &gBuffer);
glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
// - Position color buffer
glGenTextures(1, &gPosition);
glBindTexture(GL_TEXTURE_2D, gPosition);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
// - Normal color buffer
glGenTextures(1, &gNormal);
glBindTexture(GL_TEXTURE_2D, gNormal);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
// - Color + Specular color buffer
glGenTextures(1, &gAlbedoSpec);
glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
glDrawBuffers(3, attachments);
// - Create and attach depth buffer (renderbuffer)
GLuint rboDepth;
glGenRenderbuffers(1, &rboDepth);
glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
// - Finally check if framebuffer is complete
if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
std::cout << "Framebuffer not complete!" << std::endl;
glBindFramebuffer(GL_FRAMEBUFFER, 0);

glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}*/

// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, int screenWidth, int screenHeight) {
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, screenWidth, screenHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void RendererList::InitFramebuffer(int width, int height) {
	screenWidth = width, screenHeight = height;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	textureColorbuffer = generateAttachmentTexture(false, false, screenWidth, screenHeight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
																								  // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RendererList::AddObject(GameObject *newObject) {
	m_objectList.push_back(newObject);
}

void RendererList::AddObject(DebugObject *newObject) {
	m_debugList.push_back(newObject);
}

void RendererList::AddLight(DirLight *newLight) {
	m_dirLight = newLight;
}

void RendererList::AddLight(PointLight *newLight, bool isStatic) {
	(isStatic) ? m_staticPointLightList.push_back(newLight) : m_dynamicPointLightList.push_back(newLight);
}

void RendererList::AddLight(SpotLight *newLight, bool isStatic) {
	(isStatic) ? m_staticSpotLightList.push_back(newLight) : m_dynamicSpotLightList.push_back(newLight);
}

/*void RendererList::DrawDeferred(ShaderProgram &gpProgram, ShaderProgram &lpProgram, GLCamera &camera) {
static GLboolean wireframe = false;

glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
gpProgram.Bind();
glUniformMatrix4fv(gpProgram.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));
for (auto gameObject : m_objectList) {
Transform &transformTemp = gameObject->transform;
glm::mat4 model = glm::translate(glm::mat4(), transformTemp.position);
model = glm::rotate(model, glm::radians(transformTemp.rotation.x), { 1,0,0 });
model = glm::rotate(model, glm::radians(transformTemp.rotation.y), { 0,1,0 });
model = glm::rotate(model, glm::radians(transformTemp.rotation.z), { 0,0,1 });
model = glm::scale(model, transformTemp.scale);
glUniformMatrix4fv(gpProgram.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
for (int i = 0; i < gameObject->meshRef->numMeshes; ++i) {
glActiveTexture(GL_TEXTURE0);
glUniform1i(gpProgram.getUniformLocation("texture_diffuse1"), 0);
glBindTexture(GL_TEXTURE_2D, gameObject->materialRef->materialData[i].diffuse.id);
glUniform1f(gpProgram.getUniformLocation("spec"), 1);
glBindVertexArray(gameObject->meshRef->meshData[i].vao);
glDrawElements(GL_TRIANGLES, gameObject->meshRef->meshData[i].numElements, GL_UNSIGNED_INT, nullptr);
}
glBindTexture(GL_TEXTURE_2D, 0);
glBindVertexArray(0);
}
glBindFramebuffer(GL_FRAMEBUFFER, 0);

glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//-------------------------------------------------
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
lpProgram.Bind();
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, gPosition);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, gNormal);
glActiveTexture(GL_TEXTURE2);
glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
// Also send light relevant uniforms
for (GLuint i = 0; i < m_spotLightList.size(); i++) {
glUniform3fv(lpProgram.getUniformLocation("lights[" + std::to_string(i) + "].Position"), 1, glm::value_ptr(m_spotLightList[i]->position));
glUniform3fv(lpProgram.getUniformLocation("lights[" + std::to_string(i) + "].Color"), 1, glm::value_ptr(m_spotLightList[i]->diffuse));
// Update attenuation parameters and calculate radius
const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
const GLfloat linear = 0.09f;
const GLfloat quadratic = 0.032f;
glUniform1f(lpProgram.getUniformLocation("lights[" + std::to_string(i) + "].Linear"), linear);
glUniform1f(lpProgram.getUniformLocation("lights[" + std::to_string(i) + "].Quadratic"), quadratic);
// Then calculate radius of light volume/sphere
const GLfloat lightThreshold = 5; // 5 / 256
const GLfloat maxBrightness = std::fmaxf(std::fmaxf(m_spotLightList[i]->diffuse.r, m_spotLightList[i]->diffuse.g), m_spotLightList[i]->diffuse.b);
GLfloat radius = (-linear + static_cast<float>(std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0 / lightThreshold) * maxBrightness)))) / (2 * quadratic);
glUniform1f(lpProgram.getUniformLocation("lights[" + std::to_string(i) + "].Radius"), radius);
}
glUniform3fv(lpProgram.getUniformLocation("viewPos"), 1, glm::value_ptr(camera.position));
glUniform1i(lpProgram.getUniformLocation("draw_mode"), DRAW_MODE);

static GLuint quadVAO = 0;
if (quadVAO == 0)
{
GLfloat quadVertices[] = {
// Positions        // Texture Coords
-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};
GLuint quadVBO;
// Setup plane VAO
glGenVertexArrays(1, &quadVAO);
glGenBuffers(1, &quadVBO);
glBindVertexArray(quadVAO);
glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
}
glBindVertexArray(quadVAO);
glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
glBindVertexArray(0);

glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
// depth buffer in another stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
glBlitFramebuffer(0, 0, 600, 450, 0, 0, 600, 450, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
glBindFramebuffer(GL_FRAMEBUFFER, 0);

}*/

void RendererList::SendStaticLightAttributes(ShaderProgram &program, GLCamera &camera) {
	program.Bind();
	// Directional light properties
	glUniform3fv(program.getUniformLocation("dirLight.direction"), 1, glm::value_ptr(m_dirLight->direction));
	glUniform3fv(program.getUniformLocation("dirLight.ambient"), 1, glm::value_ptr(m_dirLight->ambient)); //0.05f, 0.05f, 0.05f
	glUniform3fv(program.getUniformLocation("dirLight.diffuse"), 1, glm::value_ptr(m_dirLight->diffuse)); //0.4f, 0.4f, 0.4f
	glUniform3fv(program.getUniformLocation("dirLight.specular"), 1, glm::value_ptr(m_dirLight->specular)); //0.5f, 0.5f, 0.5f
	// Point light properties
	for (size_t i = 0; i < m_staticPointLightList.size(); ++i)
		glUniform3fv(program.getUniformLocation("pointLights[" + std::to_string(i) + "].position"), 1, glm::value_ptr(m_staticPointLightList[i]->position)),
		glUniform3fv(program.getUniformLocation("pointLights[" + std::to_string(i) + "].ambient"), 1, glm::value_ptr(m_staticPointLightList[i]->ambient)),
		glUniform3fv(program.getUniformLocation("pointLights[" + std::to_string(i) + "].diffuse"), 1, glm::value_ptr(m_staticPointLightList[i]->diffuse)),
		glUniform3fv(program.getUniformLocation("pointLights[" + std::to_string(i) + "].specular"), 1, glm::value_ptr(m_staticPointLightList[i]->specular)),
		glUniform1f(program.getUniformLocation("pointLights[" + std::to_string(i) + "].constant"), m_staticPointLightList[i]->constant),
		glUniform1f(program.getUniformLocation("pointLights[" + std::to_string(i) + "].linear"), m_staticPointLightList[i]->linear),
		glUniform1f(program.getUniformLocation("pointLights[" + std::to_string(i) + "].quadratic"), m_staticPointLightList[i]->quadratic);
	// Spot light properties
	for (size_t i = 0; i < m_staticSpotLightList.size(); ++i)
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(i) + "].position"), 1, glm::value_ptr(m_staticSpotLightList[i]->position)),
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(i) + "].direction"), 1, glm::value_ptr(m_staticSpotLightList[i]->direction)),
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(i) + "].ambient"), 1, glm::value_ptr(m_staticSpotLightList[i]->ambient)),
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(i) + "].diffuse"), 1, glm::value_ptr(m_staticSpotLightList[i]->diffuse)),
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(i) + "].specular"), 1, glm::value_ptr(m_staticSpotLightList[i]->specular)),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(i) + "].constant"), m_staticSpotLightList[i]->constant),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(i) + "].linear"), m_staticSpotLightList[i]->linear),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(i) + "].quadratic"), m_staticSpotLightList[i]->quadratic),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(i) + "].cutOff"), m_staticSpotLightList[i]->cutOff),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(i) + "].outerCutOff"), m_staticSpotLightList[i]->outerCutOff);
}

void RendererList::SendDynamicLightAttributes(ShaderProgram & program, GLCamera & camera) {
	program.Bind();
	// Point light properties
	for (size_t i = 0; i < m_dynamicPointLightList.size(); ++i)
		glUniform3fv(program.getUniformLocation("pointLights[" + std::to_string(m_staticPointLightList.size() + i) + "].position"), 1, glm::value_ptr(m_dynamicPointLightList[i]->position)),
		glUniform3fv(program.getUniformLocation("pointLights[" + std::to_string(m_staticPointLightList.size() + i) + "].ambient"), 1, glm::value_ptr(m_dynamicPointLightList[i]->ambient)),
		glUniform3fv(program.getUniformLocation("pointLights[" + std::to_string(m_staticPointLightList.size() + i) + "].diffuse"), 1, glm::value_ptr(m_dynamicPointLightList[i]->diffuse)),
		glUniform3fv(program.getUniformLocation("pointLights[" + std::to_string(m_staticPointLightList.size() + i) + "].specular"), 1, glm::value_ptr(m_dynamicPointLightList[i]->specular)),
		glUniform1f(program.getUniformLocation("pointLights[" + std::to_string(m_staticPointLightList.size() + i) + "].constant"), m_dynamicPointLightList[i]->constant),
		glUniform1f(program.getUniformLocation("pointLights[" + std::to_string(m_staticPointLightList.size() + i) + "].linear"), m_dynamicPointLightList[i]->linear),
		glUniform1f(program.getUniformLocation("pointLights[" + std::to_string(m_staticPointLightList.size() + i) + "].quadratic"), m_dynamicPointLightList[i]->quadratic);
	// Spot light properties
	for (size_t i = 0; i < m_dynamicSpotLightList.size(); ++i)
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].position"), 1, glm::value_ptr(m_dynamicSpotLightList[i]->position)),
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].direction"), 1, glm::value_ptr(m_dynamicSpotLightList[i]->direction)),
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].ambient"), 1, glm::value_ptr(m_dynamicSpotLightList[i]->ambient)),
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].diffuse"), 1, glm::value_ptr(m_dynamicSpotLightList[i]->diffuse)),
		glUniform3fv(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].specular"), 1, glm::value_ptr(m_dynamicSpotLightList[i]->specular)),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].constant"), m_dynamicSpotLightList[i]->constant),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].linear"), m_dynamicSpotLightList[i]->linear),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].quadratic"), m_dynamicSpotLightList[i]->quadratic),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].cutOff"), m_dynamicSpotLightList[i]->cutOff),
		glUniform1f(program.getUniformLocation("spotLights[" + std::to_string(m_staticSpotLightList.size() + i) + "].outerCutOff"), m_dynamicSpotLightList[i]->outerCutOff);
}

void RendererList::SendMaterialAttributes(ShaderProgram & program, GLCamera & camera) {
	program.Bind();
	// Material map properties
	glUniform1i(program.getUniformLocation("material.diffuse"), 0);
	glUniform1i(program.getUniformLocation("material.normal"), 1);
	glUniform1i(program.getUniformLocation("material.specular"), 2);
	//glUniform1i(program.getUniformLocation("gPosition"), 0);
	//glUniform1i(program.getUniformLocation("gNormal"), 1);
	//glUniform1i(program.getUniformLocation("gAlbedoSpec"), 2);
}

void RendererList::DrawObjects(ShaderProgram & program, GLCamera & camera) {
	program.Bind();
	// Send camera matrix to shader (projection + view)
	glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));
	// Set camera position as the viewer
	glUniform3fv(program.getUniformLocation("viewerPosition"), 1, glm::value_ptr(camera.position));
	for (auto gameObject : m_objectList) {
		if (gameObject != nullptr && gameObject->enabled) {
			SendDynamicLightAttributes(program, camera);
			// Transform properties
			Transform &transformTemp = gameObject->transform;
			glm::mat4 model = glm::translate(glm::mat4(), transformTemp.position);
			model = glm::rotate(model, glm::radians(transformTemp.rotation.x), { 1,0,0 });
			model = glm::rotate(model, glm::radians(transformTemp.rotation.y), { 0,1,0 });
			model = glm::rotate(model, glm::radians(transformTemp.rotation.z), { 0,0,1 });
			model = glm::scale(model, transformTemp.scale);
			glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
			for (int i = 0; i < gameObject->meshRef->numMeshes; ++i) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gameObject->materialRef->materialData[i].diffuse.id);
				glUniform3fv(program.getUniformLocation("material.specular"), 1, glm::value_ptr(gameObject->materialRef->materialData[i].specular));
				glUniform3fv(program.getUniformLocation("material.emissive"), 1, glm::value_ptr(gameObject->materialRef->materialData[i].emissive));
				glUniform1f(program.getUniformLocation("material.shininess"), gameObject->materialRef->materialData[i].shininess);
				glBindVertexArray(gameObject->meshRef->meshData[i].vao);
				glDrawElements(GL_TRIANGLES, gameObject->meshRef->meshData[i].numElements, GL_UNSIGNED_INT, nullptr);
			}
			glBindVertexArray(0);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
}

void RendererList::DrawFramebuffer(ShaderProgram &program, ShaderProgram &fbProgram, GLCamera &camera) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	program.Bind();
	// Send camera matrix to shader (projection + view)
	glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));
	// Set camera position as the viewer
	glUniform3fv(program.getUniformLocation("viewerPosition"), 1, glm::value_ptr(camera.position));
	for (auto gameObject : m_objectList) {
		if (gameObject != nullptr && gameObject->enabled) {
			SendDynamicLightAttributes(program, camera);
			// Transform properties
			Transform &transformTemp = gameObject->transform;
			glm::mat4 model = glm::translate(glm::mat4(), transformTemp.position);
			model = glm::rotate(model, glm::radians(transformTemp.rotation.x), { 1,0,0 });
			model = glm::rotate(model, glm::radians(transformTemp.rotation.y), { 0,1,0 });
			model = glm::rotate(model, glm::radians(transformTemp.rotation.z), { 0,0,1 });
			model = glm::scale(model, transformTemp.scale);
			glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
			for (int i = 0; i < gameObject->meshRef->numMeshes; ++i) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gameObject->materialRef->materialData[i].diffuse.id);
				glUniform3fv(program.getUniformLocation("material.specular"), 1, glm::value_ptr(gameObject->materialRef->materialData[i].specular));
				glUniform3fv(program.getUniformLocation("material.emissive"), 1, glm::value_ptr(gameObject->materialRef->materialData[i].emissive));
				glUniform1f(program.getUniformLocation("material.shininess"), gameObject->materialRef->materialData[i].shininess);
				glBindVertexArray(gameObject->meshRef->meshData[i].vao);
				glDrawElements(GL_TRIANGLES, gameObject->meshRef->meshData[i].numElements, GL_UNSIGNED_INT, nullptr);
			}
			glBindVertexArray(0);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

	GLfloat vertices[6][4] = {
		{ 650.0f,     25.0f + 160,   0.0, 0.0 },
		{ 650.0f,     25.0f,       0.0, 1.0 },
		{ 650.0f + 240, 25.0f,       1.0, 1.0 },

		{ 650.0f,     25.0f + 160,   0.0, 0.0 },
		{ 650.0f + 240, 25.0f,       1.0, 1.0 },
		{ 650.0f + 240, 25.0f + 160,   1.0, 0.0 }
	};
	// Setup screen VAO
	if (!quadVAO) {
		///Create VAO
		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);
		///Create VBO
		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	fbProgram.Bind();
	glUniformMatrix4fv(fbProgram.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(camera.ComputeOrthographicProjection()));

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer); // Use the color attachment texture as the texture of the quad plane

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	///glDrawElementsInstanced(GL_TRIANGLES, meshData[i].numElements, GL_UNSIGNED_INT, nullptr, 1); //TODO
	//glDrawArrays(GL_LINES, /*primer*/, /*num*/);
}

#define RENDER_LIGHT_TEMPLATE() \
		glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix())); \
		glm::mat4 model = glm::translate(glm::mat4(), gameLight->position); \
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model)); \
		glUniform3fv(program.getUniformLocation("lightColor"), 1, glm::value_ptr(gameLight->diffuse)); \
		glBindVertexArray(debugLight.vao); \
		glDrawElements(GL_TRIANGLES, debugLight.elements, GL_UNSIGNED_INT, nullptr); \
		glBindVertexArray(0);

void RendererList::DrawDebug(ShaderProgram & program, GLCamera &camera) {
	program.Bind();
	static DebugLight debugLight;
	glPolygonMode(GL_FRONT_AND_BACK, WIREFRAME_MODE ? GL_LINE : GL_FILL);
	for (auto gameLight : m_staticPointLightList) { RENDER_LIGHT_TEMPLATE(); }
	for (auto gameLight : m_staticSpotLightList) { RENDER_LIGHT_TEMPLATE(); }
	for (auto debugObject : m_debugList) {
		// Transform properties
		glm::mat4 model = glm::translate(glm::mat4(), debugObject->transform.position);
		model = glm::rotate(model, glm::radians(debugObject->transform.rotation.x), { 1,0,0 });
		model = glm::rotate(model, glm::radians(debugObject->transform.rotation.y), { 0,1,0 });
		model = glm::rotate(model, glm::radians(debugObject->transform.rotation.z), { 0,0,1 });
		model = glm::scale(model, debugObject->transform.scale);
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
		// Material properties
		glUniform3fv(program.getUniformLocation("lightColor"), 1, glm::value_ptr(debugObject->color));
		// Mesh properties
		for (int i = 0; i < debugObject->meshRef->numMeshes; ++i) {
			glBindVertexArray(debugObject->meshRef->meshData[i].vao);
			glDrawElements(GL_TRIANGLES, debugObject->meshRef->meshData[i].numElements, GL_UNSIGNED_INT, nullptr);
		} glBindVertexArray(0);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RendererList::Clear() {
	m_objectList.clear();
	m_debugList.clear();
	m_dirLight = nullptr;
	m_staticPointLightList.clear();
	m_staticSpotLightList.clear();
	m_dynamicPointLightList.clear();
	m_dynamicSpotLightList.clear();
}
