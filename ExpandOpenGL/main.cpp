#define STB_IMAGE_IMPLEMENTATION

#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.hpp"

#include "Window.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "Material.hpp"

#include "Model.hpp"

#include "Skybox.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace std;

const float toRadians = 3.14159265f / 180.0f;

GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColor = 0,
uniformDirection = 0, uniformDiffuseIntensity = 0,
uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

Window mainWindow;
vector<Mesh*> meshList;
vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model SpaceShip;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLfloat SpaceShipAngle = 0.0f;

float multiplier;

//vertex shader
static const char* vShader = "Shaders/shader.vert";

//fragment shader
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset) {

	for (size_t i = 0; i < indicesCount; i += 3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}
	for (size_t i = 0; i < verticesCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; 
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() {

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//x     y       z    u     v       Normal
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f

	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh;
	obj1->createMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh;
	obj2->createMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh;
	obj3->createMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

}

void createShaders() {

	Shader* shader1 = new Shader;
	shader1->createFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.createFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader.createFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

void RenderScene() {

	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, -1.0f, -1.0f));
	model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(multiplier * 1.0f, multiplier * 1.0f, multiplier * 1.0f));
	//glUniform1f(uniformModel, xoffset);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->renderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 2.0f, -2.5f));
	//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->renderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->renderMesh();

	SpaceShipAngle += 0.025f;
	if (SpaceShipAngle > 360.0f)
	{
		SpaceShipAngle = 0.1f;
	}

	model = glm::mat4(1.0f);
	model = glm::rotate(model, -SpaceShipAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(7.0f, 2.0f, 0.0f));
	model = glm::rotate(model, 25 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	SpaceShip.RenderModel();

}

void DirectionalShadowMapPass(DirectionalLight* light) {

	directionalShadowShader.useShader();

	glViewport(0, 0, light->GetShadowMap()->getShadowWitdth(), light->GetShadowMap()->getShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.getModelLocation();
	glm::mat4 lightTransform = light->CalculateLightTransform();
	directionalShadowShader.setDirectionalLightTransform(&lightTransform);

	directionalShadowShader.validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void OmniShadowMapPass(PointLight* light) {

	omniShadowShader.useShader();

	glViewport(0, 0, light->GetShadowMap()->getShadowWitdth(), light->GetShadowMap()->getShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.getModelLocation();
	uniformOmniLightPos = omniShadowShader.getOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.getFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->getPosition().x, light->getPosition().y, light->getPosition().z);
	glUniform1f(uniformFarPlane, light->getFarPlane());
	omniShadowShader.setLightMatrices(light->CalculateLightTransform());

	omniShadowShader.validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {

	glViewport(0, 0, 1366, 768);

	//clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0].useShader();

	uniformModel = shaderList[0].getModelLocation();
	uniformProjection = shaderList[0].getProjectionLocation();
	uniformView = shaderList[0].getViewLocation();
	uniformEyePosition = shaderList[0].getEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].getSpecularIntensity();
	uniformShininess = shaderList[0].getShininess();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0].setDirectionalLight(&mainLight);
	shaderList[0].setPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].setSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	glm::mat4 lightTransform = mainLight.CalculateLightTransform();
	shaderList[0].setDirectionalLightTransform(&lightTransform);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].setTexture(1);
	shaderList[0].setDirectionalShadowMap(2);

	shaderList[0].validate();

	RenderScene();

}

int main() {

	mainWindow = Window(1366, 768);
	mainWindow.initialize();

	CreateObjects();
	createShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 3.0f, 0.25f); // second from the end is changing movement speed and last one is mouse speed

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	SpaceShip = Model();
	SpaceShip.LoadModel("Models/SpaceShip/Intergalactic_Spaceship-(Wavefront).obj");

	mainLight = DirectionalLight(2048, 2048,
								 1.0f, 0.53f, 0.3f, 
								 0.1f, 0.9f,
								 -10.0f, -12.0f, 19.0f);

	pointLights[0] = PointLight(1024.0f, 1024.0f,
								0.01f, 100.0f,
								0.0f, 0.0f, 1.0f,
								0.0f, 0.1f,
								1.0f, 2.0f, -1.5f,
								0.3f, 0.1f, 0.01f);
	pointLightCount++;
	pointLights[1] = PointLight(1024.0f, 1024.0f,
								0.1f, 100.0f,
								0.0f, 1.0f, 0.0f,
								0.0f, 0.1f,
								-4.0f, 3.0f, 0.0f,
								0.3f, 0.1f, 0.01f);
	pointLightCount++;

	spotLights[0] = SpotLight(1024.0f, 1024.0f,
							0.01f, 100.0f, 
							1.0f, 1.0f, 1.0f,
							0.0f, 2.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, -1.0f, 0.0f,
							1.0f, 0.0f, 0.0f,
							20.0f);
	spotLightCount++;

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow.getRawWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	//loop
	while (!mainWindow.getShouldClose()) {

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//user events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++) {
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++) {
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(projection, camera.calculateViewMatrix());

		//implement imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool open = true;
		ImGui::Begin("My game's gui, HELLO THERE!", &open, ImGuiWindowFlags_NoResize);
		ImGui::Text("Hello traveler...");
		ImGui::SliderFloat("Size of triangle 1", &multiplier, 0.0f, 10.0f);
		ImGui::InputFloat("Size of triangle 2", &multiplier, 0.0f, 10.0f);
		ImGui::Button("button 1");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//end of imgui

		glUseProgram(0);

		mainWindow.swapBuffers();

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}