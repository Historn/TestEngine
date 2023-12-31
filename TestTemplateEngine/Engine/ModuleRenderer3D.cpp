#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	SDL_GL_MakeCurrent(App->window->window, context);


	if (ret == true)
	{

		auto glew_init_error = glewInit();
		if (glew_init_error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(glew_init_error));
			ret = false;
		}
		/*if (glew_init_error != GLEW_OK) throw exception((char*)glewGetErrorString(glew_init_error));
		if (!GLEW_VERSION_3_1) throw exception("OpenGL 3.1 Not Supported!");*/

		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(App->camera->fov, App->camera->aspect, App->camera->zNear, App->camera->zFar);

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(App->camera->eye.x, App->camera->eye.y, App->camera->eye.z,
			App->camera->center.x, App->camera->center.y, App->camera->center.z,
			App->camera->up.x, App->camera->up.y, App->camera->up.z);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		//Initialize clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	//lights[0].SetPos(App->camera->position.x, App->camera->position.y, App->camera->position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// Update: Draw meshes and other stuff
update_status ModuleRenderer3D::Update(float dt)
{
	drawGrid(100, 1);
	drawAxis();
	
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(App->camera->fov, App->camera->aspect, App->camera->zNear, App->camera->zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(App->camera->eye.x, App->camera->eye.y, App->camera->eye.z,
		App->camera->center.x, App->camera->center.y, App->camera->center.z,
		App->camera->up.x, App->camera->up.y, App->camera->up.z);

}
