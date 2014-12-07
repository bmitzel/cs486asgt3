/* Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 * Assignment: 2 - Picking
 * Due Date: 9/29/2014
 *
 * Filename: picking.cpp
 *
 * This program uses OpenGL to load and render 3 PLY models
 * in a scene containing a ground plane and sky box.
 * Clicking on any of the models toggles the drawing of its
 * bounding volume.
 *
 * This program also supports shading using either the
 * fixed function OpenGL pipeline or (by default) a custom
 * Blinn-Phong shader program.
 */

#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef FREEGLUT
#include <GL/freeglut_ext.h>
#endif

#include "GLSLShader.h"
#include "Scene.h"

//
// Preprocessor definitions
//

#define WINDOW_DEFAULT_WIDTH 1280
#define WINDOW_DEFAULT_HEIGHT 720
#define WINDOW_MIN_WIDTH 300
#define WINDOW_MIN_HEIGHT 200
#define WINDOW_MAX_WIDTH glutGet(GLUT_SCREEN_WIDTH)
#define WINDOW_MAX_HEIGHT glutGet(GLUT_SCREEN_HEIGHT)

//
// Function Prototypes
//

/* Initialization functions */
void validateArgs(int argc, char* argv[]);
void initProgram();
void initGL();

/* User interface functions */
void printHelpMessage();
void pick(int mouseX, int mouseY);

/* Drawing functions */
void drawGroundPlane(Camera*);
void drawSkyBox(Camera*);
void drawScene(Camera*);

/* GLUT callback functions */
void displayCallback();
void reshapeCallback(int width, int height);
void keyboardCallback(unsigned char key, int x, int y);
void mouseCallback(int button, int state, int x, int y);
void timerCallback(int x);

/* Math functions */
void vecCopy4f(float *dest, const float *src); /* from Professor Shafae */
void matMultVec4f(float vout[4], const float v[4], float m[16]); /* from Professor Shafae */
void transformVecByModelView(float outVec[4], const float inVec[4]); /* from Professor Shafae */

/* Debugging functions */
void msglPrintMatrix16dv(const char *varName, double matrix[16]); /* from Professor Shafae */
void printTopOfBothStacks(const char* msg); /* from Professor Shafae */
void msglVersion(); /* from Professor Shafae */

//
// Global Identifiers
//

/* Global constants */
static const char   windowTitle[]       = "Picking";    /* window title */

/* Global variables */
static int      windowInitialWidth;                     /* initial window width */
static int      windowInitialHeight;                    /* initial window height */
static int      windowWidth;                            /* current window width */
static int      windowHeight;                           /* current window height */
static bool     isFullScreen;                           /* window full screen flag */
static bool     isDrawingBoundingVolumes;               /* drawing bounding volumes flag */
static bool     isUsingGLSLShader;                      /* using GLSL shader program flag */
static Scene    scene;                                  /* the scene to render */

/* GLSL shader program */
GLSLProgram* shaderProgram;

/* Shader program values */
const float light0_world_pos[] = {0.0f, 14.5f, 0.0f, 1.0f}; /* light position in world space */
float light0_model_pos[4];                                  /* light position in modelview space */

/* Shader program uniform variables */
unsigned int uLight0_position;
unsigned int uLight0_color;
unsigned int uAmbient;
unsigned int uDiffuse;
unsigned int uSpecular;
unsigned int uShininess;

//
// Function Definitions
//

/**
 * Begins program execution
 * @param argc - The number of command line arguments
 * @param argv - An array of C strings containing each command line argument
 * @return - The exit code
 */
int main(int argc, char* argv[])
{
    /* Initialize GLUT */
    glutInit(&argc, argv);

    /* Validate and process the command line arguments */
    validateArgs(argc, argv);

    /* Initialize program parameters */
    initProgram();

    /* Create the window */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(::windowWidth, ::windowHeight);
    glutCreateWindow(::windowTitle);

    /* Initialize GLEW */
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "GLEW init failed.\n");
        exit(1);
    }

    /* Initialize OpenGL */
    initGL();

    /* Print OpenGL version debug info */
    msglVersion();

    /* Print help text */
    puts("Press 'h' to see a help message at any time.");

    /* Begin GLUT main loop */
    glutMainLoop();

    return 0;
} /* main() */

/**
 * Validates command line arguments
 * @param argc - The number of command line arguments
 * @param argv - An array of C strings containing each command line argument
 */
void validateArgs(int argc, char* argv[])
{
    /* Validate command line arguments */
    if (3 == argc)
    {
        /* Set initial window width */
        ::windowInitialWidth = strtol(argv[1], NULL, 0);

        /* Check for value out of range */
        if (ERANGE == errno)
        {
            perror("strtol");
            exit(-1);
        }

        /* Set initial window height */
        ::windowInitialHeight = strtol(argv[2], NULL, 0);

        /* Check for value out of range */
        if (ERANGE == errno)
        {
            perror("strtol");
            exit(-1);
        }

        /* Set up an array for error messages */
        char errorMsgs[2][80] = {0};
        size_t numErrors = 0;

        /* Validate window width */
        if (WINDOW_MIN_WIDTH > ::windowInitialWidth
                || WINDOW_MAX_WIDTH < ::windowInitialWidth)
        {
            snprintf(errorMsgs[numErrors++], 79,
                    "Error: width must be between %d and %d\n",
                    WINDOW_MIN_WIDTH, WINDOW_MAX_WIDTH);
        }

        /* Validate window height */
        if (WINDOW_MIN_HEIGHT > ::windowInitialHeight
                || WINDOW_MAX_HEIGHT < ::windowInitialHeight)
        {
            snprintf(errorMsgs[numErrors++], 79,
                    "Error: height must be between %d and %d\n",
                    WINDOW_MIN_HEIGHT, WINDOW_MAX_HEIGHT);
        }

        /* Print error messages and exit */
        if (0 < numErrors)
        {
            for (size_t i = 0; i < numErrors; i++)
            {
                fprintf(stderr, errorMsgs[i]);
            }

            exit(-1);
        }
    }
    /* No command line arguments */
    else if (1 == argc)
    {
        /* Clamp to screen width and set window width */
        if (WINDOW_MAX_WIDTH < WINDOW_DEFAULT_WIDTH)
        {
            ::windowInitialWidth = WINDOW_MAX_WIDTH;
        }
        else
        {
            ::windowInitialWidth = WINDOW_DEFAULT_WIDTH;
        }

        /* Clamp to screen height and set window height */
        if (WINDOW_MAX_HEIGHT < WINDOW_DEFAULT_HEIGHT)
        {
            ::windowInitialHeight = WINDOW_MAX_HEIGHT;
        }
        else
        {
            ::windowInitialHeight = WINDOW_DEFAULT_HEIGHT;
        }
    }
    /* Invalid command line arguments */
    else
    {
        /* Print command line usage and exit */
        fprintf(stderr, "Usage: %s [<width> <height>]\n", argv[0]);
        exit(-1);
    }
} /* validateArgs() */

/**
 * Initializes program parameters
 */
void initProgram()
{
    ::windowWidth = ::windowInitialWidth;
    ::windowHeight = ::windowInitialHeight;
    ::isFullScreen = false;
    ::isDrawingBoundingVolumes = false;
    ::isUsingGLSLShader = true;

    /* Seed the random number generator for setting the
     * initial height and rotation of models in the scene */
    srand(time(NULL));
} /* initProgram() */

/**
 * Initialize OpenGL parameters
 */
void initGL()
{
    /* Set the OpenGL state */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   /* uses black for clearing the color buffers */
    glEnable(GL_DEPTH_TEST);                /* enables z-buffer depth testing */
    glDepthRange(0.0, 1.0);                 /* set the range for the z-buffer */
    glDepthFunc(GL_LEQUAL);                 /* depth test uses <= comparison function */
    glEnable(GL_CULL_FACE);                 /* enables face culling */
    glCullFace(GL_BACK);                    /* back faces are culled */
    glFrontFace(GL_CCW);                    /* front faces are calculated using CCW winding */
    glEnable(GL_NORMALIZE);                 /* normalizes all normals in fixed function pipeline */

    /* Load the shader program */
    const char* vertexShaderSource = "blinn_phong.vert.glsl";
    const char* fragmentShaderSource = "blinn_phong.frag.glsl";
    FragmentShader fragmentShader(fragmentShaderSource);
    VertexShader vertexShader(vertexShaderSource);
    ::shaderProgram = new GLSLProgram();
    ::shaderProgram->attach(vertexShader);
    ::shaderProgram->attach(fragmentShader);
    ::shaderProgram->link();

    /* Activate the shader program */
    if (::isUsingGLSLShader)
    {
        ::shaderProgram->activate();
        printf("Shader program built from %s and %s.\n", vertexShaderSource, fragmentShaderSource);
        if (::shaderProgram->isActive())
        {
            printf("Shader program is loaded and active with id %d.\n", ::shaderProgram->id());
        }
        else
        {
            printf("Shader program did not load and activate correctly. Exiting.\n");
            exit(1);
        }
    }

    /* Set up shader program uniform variables */
    ::uLight0_position = glGetUniformLocation(::shaderProgram->id(), "light0_position");
    ::uLight0_color = glGetUniformLocation(::shaderProgram->id(), "light0_color");
    ::uAmbient = glGetUniformLocation(::shaderProgram->id(), "ambient");
    ::uDiffuse = glGetUniformLocation(::shaderProgram->id(), "diffuse");
    ::uSpecular = glGetUniformLocation(::shaderProgram->id(), "specular");
    ::uShininess = glGetUniformLocation(::shaderProgram->id(), "shininess");

    /* Initialize the lighting for the fixed function pipeline */
    glShadeModel(GL_SMOOTH);
    GLfloat light0_ambient[]  = { 0.0f,  0.0f, 0.0f, 1.0f};
    GLfloat light0_diffuse[]  = { 1.0f,  1.0f, 1.0f, 1.0f};
    GLfloat light0_position[] = {-2.0f, 14.5f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT , light0_ambient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE , light0_diffuse );
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glEnable (GL_LIGHT0);

    /* Enable the lighting for the fixed function pipeline */
    if (!::isUsingGLSLShader)
    {
        glEnable(GL_LIGHTING);
    }

    /* Add the PLY models to the scene */
    ::scene.Insert("data/dragon_vrip_res4.ply", Point3(-2.0f, 1.5f, -0.5f));
    ::scene.Insert("data/dragon_vrip_res4.ply", Point3(2.0f, 1.5f, -0.5f));
    ::scene.Insert("data/bun_zipper_res2.ply", Point3(0.0f, 1.5f, 0.5f));

    /* Register GLUT callback functions */
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseCallback);
    glutTimerFunc(16, timerCallback, 0);

    msglError();
} /* initGL() */

/**
 * Prints user help text to the console
 */
void printHelpMessage()
{
    puts("Press 'b' to toggle rendering the bounding volumes.");
    puts("Press 'f' to toggle full screen mode (freeglut only).");
    puts("Press 'g' to toggle between the GLSL program and the fixed function pipeline.");
    puts("Press 'o' to reset the window to its original resolution.");
    puts("Press ESC or 'q' to quit.");
    puts("Press 'h' to print this message again.");
} /* printHelpMessage() */

/**
 * Draws the ground plane
 * @param camera - The camera used for the viewing matrix
 */
void drawGroundPlane(Camera* camera)
{
    /* Set the material properties for the ground plane */
    if (::isUsingGLSLShader)
    {
        float light0_color[] = {0.0f, 0.7f, 0.0f, 1.0f};
        float specular[]     = {1.0f, 1.0f, 1.0f, 1.0f};
        float diffuse[]      = {0.5f, 0.5f, 0.5f, 1.0f};
        float ambient[]      = {0.2f, 0.2f, 0.2f, 1.0f};
        float shininess[]    = {1.0f};
        glUniform4fv(::uLight0_color, 1, light0_color);
        glUniform4fv(::uAmbient     , 1, ambient     );
        glUniform4fv(::uDiffuse     , 1, diffuse     );
        glUniform4fv(::uSpecular    , 1, specular    );
        glUniform1fv(::uShininess   , 1, shininess   );
    }
    else
    {
        GLfloat mAmbient[]  = {0.8f, 0.8f, 0.8f};
        GLfloat mDiffuse[]  = {0.1f, 0.8f, 0.1f};
        GLfloat mSpecular[] = {0.6f, 0.7f, 0.6f};
        GLfloat mShininess  =  0.6f;
        glMaterialfv(GL_FRONT, GL_AMBIENT  , mAmbient          );
        glMaterialfv(GL_FRONT, GL_DIFFUSE  , mDiffuse          );
        glMaterialfv(GL_FRONT, GL_SPECULAR , mSpecular         );
        glMaterialf (GL_FRONT, GL_SHININESS, mShininess * 128.0);
    }

    /* Set the viewing matrix */
    glLoadIdentity();
    gluLookAt(camera->eyePosition.x, camera->eyePosition.y, camera->eyePosition.z,
              camera->refPoint.x   , camera->refPoint.y   , camera->refPoint.z   ,
              camera->upVector.x   , camera->upVector.y   , camera->upVector.z   );

    /* Draw the ground plane */
    glBegin(GL_QUADS);
    glNormal3f(  0.0f, 1.0f,   0.0f);
    glVertex3f(-15.0f, 0.0f, -15.0f);
    glVertex3f(-15.0f, 0.0f,  15.0f);
    glVertex3f( 15.0f, 0.0f,  15.0f);
    glVertex3f( 15.0f, 0.0f, -15.0f);
    glEnd();
} /* drawGroundPlane() */

/**
 * Draws the sky box
 * @param camera - The camera used for the viewing matrix
 */
void drawSkyBox(Camera* camera)
{
    /* Set the material properties for the sky box */
    if (::isUsingGLSLShader)
    {
        float light0_color[] = {0.0f, 0.0f, 0.7f, 1.0f};
        float specular[]     = {1.0f, 1.0f, 1.0f, 1.0f};
        float diffuse[]      = {0.5f, 0.5f, 0.5f, 1.0f};
        float ambient[]      = {0.2f, 0.2f, 0.2f, 1.0f};
        float shininess[]    = {1.0f};
        glUniform4fv(::uLight0_color, 1, light0_color);
        glUniform4fv(::uAmbient     , 1, ambient     );
        glUniform4fv(::uDiffuse     , 1, diffuse     );
        glUniform4fv(::uSpecular    , 1, specular    );
        glUniform1fv(::uShininess   , 1, shininess   );
    }
    else
    {
        GLfloat mAmbient[]  = {1.0f, 1.0f, 1.0f};
        GLfloat mDiffuse[]  = {0.1f, 0.1f, 1.0f};
        GLfloat mSpecular[] = {0.0f, 0.0f, 0.0f};
        GLfloat mShininess  =  0.0f;
        glMaterialfv(GL_FRONT, GL_AMBIENT  , mAmbient          );
        glMaterialfv(GL_FRONT, GL_DIFFUSE  , mDiffuse          );
        glMaterialfv(GL_FRONT, GL_SPECULAR , mSpecular         );
        glMaterialf (GL_FRONT, GL_SHININESS, mShininess * 128.0);
    }

    /* Set the viewing matrix */
    glLoadIdentity();
    gluLookAt(camera->eyePosition.x, camera->eyePosition.y, camera->eyePosition.z,
              camera->refPoint.x   , camera->refPoint.y   , camera->refPoint.z   ,
              camera->upVector.x   , camera->upVector.y   , camera->upVector.z   );

    /* Draw the sky box */
    glBegin(GL_QUADS);
    /* Front sky */
    glNormal3f( -1.0f,  0.0f,   1.0f);
    glVertex3f( 15.0f,  0.0f, -15.0f);
    glNormal3f( -1.0f, -1.0f,   1.0f);
    glVertex3f( 15.0f, 15.0f, -15.0f);
    glNormal3f(  1.0f, -1.0f,   1.0f);
    glVertex3f(-15.0f, 15.0f, -15.0f);
    glNormal3f(  1.0f,  0.0f,   1.0f);
    glVertex3f(-15.0f,  0.0f, -15.0f);
    /* Rear sky */
    glNormal3f(  1.0f,  0.0f,  -1.0f);
    glVertex3f(-15.0f,  0.0f,  15.0f);
    glNormal3f(  1.0f, -1.0f,  -1.0f);
    glVertex3f(-15.0f, 15.0f,  15.0f);
    glNormal3f( -1.0f, -1.0f,  -1.0f);
    glVertex3f( 15.0f, 15.0f,  15.0f);
    glNormal3f( -1.0f,  0.0f,  -1.0f);
    glVertex3f( 15.0f,  0.0f,  15.0f);
    /* Left sky */
    glNormal3f(  1.0f,  0.0f,   1.0f);
    glVertex3f(-15.0f,  0.0f, -15.0f);
    glNormal3f(  1.0f, -1.0f,   1.0f);
    glVertex3f(-15.0f, 15.0f, -15.0f);
    glNormal3f(  1.0f, -1.0f,  -1.0f);
    glVertex3f(-15.0f, 15.0f,  15.0f);
    glNormal3f(  1.0f,  0.0f,  -1.0f);
    glVertex3f(-15.0f,  0.0f,  15.0f);
    /* Right sky */
    glNormal3f( -1.0f,  0.0f,  -1.0f);
    glVertex3f( 15.0f,  0.0f,  15.0f);
    glNormal3f( -1.0f, -1.0f,  -1.0f);
    glVertex3f( 15.0f, 15.0f,  15.0f);
    glNormal3f( -1.0f, -1.0f,   1.0f);
    glVertex3f( 15.0f, 15.0f, -15.0f);
    glNormal3f( -1.0f,  0.0f,   1.0f);
    glVertex3f( 15.0f,  0.0f, -15.0f);
    /* Top sky */
    glNormal3f( -1.0f, -1.0f,   1.0f);
    glVertex3f( 15.0f, 15.0f, -15.0f);
    glNormal3f( -1.0f, -1.0f,  -1.0f);
    glVertex3f( 15.0f, 15.0f,  15.0f);
    glNormal3f(  1.0f, -1.0f,  -1.0f);
    glVertex3f(-15.0f, 15.0f,  15.0f);
    glNormal3f(  1.0f, -1.0f,   1.0f);
    glVertex3f(-15.0f, 15.0f, -15.0f);
    glEnd();
} /* drawSkyBox() */

/**
 * Draws the PLY models in the scene
 * @param camera - The camera used for the viewing matrix
 */
void drawScene(Camera* camera)
{
    std::list<Model*>* models = ::scene.GetModels();

    /* Iterate through all the models in the scene */
    for (std::list<Model*>::const_iterator itr = models->begin(); itr != models->end(); itr++)
    {
        /* Update the model's transformation */
        (*itr)->Update();

        /* Set the material properties for the models */
        if (::isUsingGLSLShader)
        {
            float light0_color[] = {0.7f, 0.7f, 0.7f, 1.0f};
            float specular[]     = {1.0f, 1.0f, 1.0f, 1.0f};
            float diffuse[]      = {0.5f, 0.5f, 0.5f, 1.0f};
            float ambient[]      = {0.2f, 0.2f, 0.2f, 1.0f};
            float shininess[]    = {1.0f};
            glUniform4fv(::uLight0_color, 1, light0_color);
            glUniform4fv(::uAmbient     , 1, ambient     );
            glUniform4fv(::uDiffuse     , 1, diffuse     );
            glUniform4fv(::uSpecular    , 1, specular    );
            glUniform1fv(::uShininess   , 1, shininess   );
        }
        else
        {
            GLfloat mAmbient[]  = {0.5f, 0.5f, 0.5f};
            GLfloat mDiffuse[]  = {0.9f, 0.9f, 0.9f};
            GLfloat mSpecular[] = {0.0f, 0.0f, 0.0f};
            GLfloat mShininess  =  0.0f;
            glMaterialfv(GL_FRONT, GL_AMBIENT  , mAmbient          );
            glMaterialfv(GL_FRONT, GL_DIFFUSE  , mDiffuse          );
            glMaterialfv(GL_FRONT, GL_SPECULAR , mSpecular         );
            glMaterialf (GL_FRONT, GL_SHININESS, mShininess * 128.0);
        }

        /* Set the viewing matrix */
        glLoadIdentity();
        gluLookAt(camera->eyePosition.x, camera->eyePosition.y, camera->eyePosition.z,
                  camera->refPoint.x   , camera->refPoint.y   , camera->refPoint.z   ,
                  camera->upVector.x   , camera->upVector.y   , camera->upVector.z   );

        /* Get the face list */
        FaceList* faceList = (*itr)->GetFaceList();

        /* Translate, rotate, and scale the model */
        glTranslatef(faceList->center[0], faceList->center[1], faceList->center[2]);
        glRotatef((*itr)->GetRotation(), 0.0f, 1.0f, 0.0f);
        glScalef((*itr)->GetScaleFactor(), (*itr)->GetScaleFactor(), (*itr)->GetScaleFactor());

        /* Draw the model */
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < faceList->fc; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glColor3dv(faceList->colors[faceList->faces[i][j]]);
                glNormal3dv(faceList->v_normals[faceList->faces[i][j]]);
                glVertex3dv(faceList->vertices[faceList->faces[i][j]]);
            }
        }
        glEnd();

        /* Draw the bounding volumes */
        if ((*itr)->GetIsDrawingBoundingSphere())
        {
            /* Set the material properties for the bounding volumes */
            if (::isUsingGLSLShader)
            {
                float light0_color[] = {0.2f, 0.2f, 0.0f, 0.4f};
                float specular[]     = {0.0f, 0.0f, 0.0f, 0.4f};
                float diffuse[]      = {0.4f, 0.4f, 0.4f, 0.4f};
                float ambient[]      = {0.2f, 0.2f, 0.2f, 0.4f};
                float shininess[]    = {1.0f};
                glUniform4fv(::uLight0_color, 1, light0_color);
                glUniform4fv(::uAmbient     , 1, ambient     );
                glUniform4fv(::uDiffuse     , 1, diffuse     );
                glUniform4fv(::uSpecular    , 1, specular    );
                glUniform1fv(::uShininess   , 1, shininess   );
            }
            /* Enable transparency */
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_COLOR_MATERIAL);
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

            /* Draw the sphere */
            glColor4f(1.0f, 0.33f, 1.0f, 0.5f);
            glutSolidSphere(faceList->radius, 32, 32);

            /* Disable transparency */
            glDisable(GL_COLOR_MATERIAL);
            glDisable(GL_BLEND);
        }
    }
} /* drawScene() */

/**
 * Tests for intersection with mouse click and scene objects
 * @param mouseX - The x position of the mouse click
 * @param mouseY - The y position of the mouse click
 */
void pick(int mouseX, int mouseY)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLdouble windowX = mouseX;
    GLdouble windowY;

    /* Initialize matrices and window coordinates */
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // deprecated
    glGetDoublev(GL_PROJECTION_MATRIX, projection); // deprecated
    windowY = viewport[3] - mouseY - 1;

    /* Find points on the front and back of the view frustum */
    GLdouble xx;
    GLdouble yy;
    GLdouble zz;
    gluUnProject(windowX, windowY, 0.0, modelview, projection, viewport, &xx, &yy, &zz);
    Point3 near(xx, yy, zz); // point on front of view frustum
    gluUnProject(windowX, windowY, 1.0, modelview, projection, viewport, &xx, &yy, &zz);
    Point3 far(xx, yy, zz);  // point on back of view frustum

    /* Cast a ray and check for intersection with scene objects */
    Ray ray(near, far);
    std::list<Model*>* models = ::scene.GetModels();
    for (std::list<Model*>::const_iterator itr = models->begin(); itr != models->end(); itr++)
    {
        if ((*itr)->Intersects(ray))
        {
            puts("Intersect");
            (*itr)->ToggleDrawingBoundingSphere();
        }
    }
} /* pick() */

/**
 * Renders the scene
 * This is the GLUT display callback function
 */
void displayCallback()
{
    msglError();

    /* Clear buffers and use the modelview matrix */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    /* Get the camera */
    Camera* camera = ::scene.GetCamera();

    /* Set the light position for the shader program */
    if (::isUsingGLSLShader)
    {
        transformVecByModelView(::light0_model_pos, ::light0_world_pos);
        glUniform4fv(::uLight0_position, 1, ::light0_model_pos);
    }

    /* Draw the ground plane and sky box */
    glPushMatrix();
    drawGroundPlane(camera);
    glPopMatrix();
    glPushMatrix();
    drawSkyBox(camera);
    glPopMatrix();

    /* Draw the PLY models */
    glPushMatrix();
    drawScene(camera);
    glPopMatrix();

    glutSwapBuffers();

    msglError();
} /* displayCallback() */

/**
 * Recalculates the perspective when the window is resized
 * This is the GLUT reshape callback function
 * @param width - The current width of the window
 * @param height - The current height of the window
 */
void reshapeCallback(int width, int height)
{
    /* Save the new window width and height */
    if (!::isFullScreen)
    {
        ::windowWidth = width;
        ::windowHeight = height;
    }

    /* Avoid division by zero */
    if (0 == height)
    {
        height = 1;
    }

    /* Update the projection matrix */
    double ratio = static_cast<double>(width) / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    gluPerspective(45.0, ratio, 1.0, 25.0);

    /* Reset the modelview matrix */
    Camera* camera = ::scene.GetCamera();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera->eyePosition.x, camera->eyePosition.y, camera->eyePosition.z,
              camera->refPoint.x   , camera->refPoint.y   , camera->refPoint.z   ,
              camera->upVector.x   , camera->upVector.y   , camera->upVector.z   );
} /* reshapeCallback() */

/**
 * Handles normal keyboard events
 * This is the GLUT keyboard callback function
 * @param key - The ASCII value of the key being pressed
 * @param x - The x coordinate of the mouse position
 * @param y - The y coordinate of the mouse position
 */
void keyboardCallback(unsigned char key, int x, int y)
{
    std::list<Model*>* models = ::scene.GetModels();

    switch(toupper(key))
    {
    /* Toggle drawing bounding volumes */
    case 'B':
        ::isDrawingBoundingVolumes = !::isDrawingBoundingVolumes;
        printf("Drawing Bounding Volumes is %s\n", ::isDrawingBoundingVolumes ? "on" : "off");
        for (std::list<Model*>::const_iterator itr = models->begin(); itr != models->end(); itr++)
        {
            (*itr)->SetIsDrawingBoundingSphere(::isDrawingBoundingVolumes);
        }
        break;
#ifdef FREEGLUT
    /* Toggle full screen mode */
    case 'F':
        if (::isFullScreen)
        {
            /* Disable full screen mode */
            ::isFullScreen = false;
            glutReshapeWindow(::windowWidth, ::windowHeight);
            puts("Full Screen Mode is disabled");
        }
        else
        {
            /* Enable full screen mode */
            ::isFullScreen = true;
            glutFullScreen();
            puts("Full Screen Mode is enabled");
        }
        break;
#endif
        /* Toggle using GLSL shader program */
    case 'G':
        if (::isUsingGLSLShader)
        {
            /* Disable GLSL shader */
            glEnable(GL_LIGHTING);
            ::shaderProgram->deactivate();
            puts("GLSL Shader Program is off");
        }
        else
        {
            /* Enable GLSL shader */
            glDisable(GL_LIGHTING);
            ::shaderProgram->activate();
            puts("GLSL Shader Program is on");
        }
        ::isUsingGLSLShader = !::isUsingGLSLShader;
        break;
    /* Restore original window width and height */
    case 'O':
        glutReshapeWindow(::windowInitialWidth, ::windowInitialHeight);
#ifdef FREEGLUT
        if (::isFullScreen)
        {
            ::isFullScreen = false;
        }
        else
        {
            glutPostRedisplay();
        }
#else
        glutPostRedisplay();
#endif
        puts("Original Window Resolution is restored");
        break;
    /* Quit the program */
    case 'Q':
    case  27:   /* ESC key */
        puts("Exiting...");
#ifdef FREEGLUT
        glutLeaveMainLoop();
#else
        exit(0);
#endif
        break;
    /* Print help text */
    case 'H':
        printHelpMessage();
        break;
    }
} /* keyboardCallback() */

/**
 * Handles mouse events
 * This is the GLUT mouse callback function
 * @param button - The mouse button that triggered the event
 * @param state - Whether the mouse button is up or down
 * @param x - The x coordinate of the mouse position
 * @param y - The y coordinate of the mouse position
 */
void mouseCallback(int button, int state, int x, int y)
{
    if (GLUT_LEFT_BUTTON == button && GLUT_DOWN == state)
    {
        printf("Mouse click at (%d, %d)\n", x, y);
        pick(x, y);
    }
} /* mouseCallback() */

/**
 * Redraws the scene every 16ms
 * This is the GLUT timer callback function
 * @param x - Unused value parameter
 */
void timerCallback(int x)
{
    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0);
} /* timerCallback() */

/**
 * Copies a 4x4 matrix of floats into another 4x4 matrix of floats
 * from Professor Shafae
 * @param dest - The destination matrix to copy to
 * @param src - The source matrix to copy from
 */
void vecCopy4f(float *dest, const float *src)
{
    for(int i = 0; i < 4; i++)
    {
        dest[i] = src[i];
    }
} /* vecCopy4f() */

/**
 * Multiplies a 4D vector by a 4x4 matrix of floats for performing transformations
 * from Professor Shafae
 * @param vout - The returned vector where the result of the computation is stored
 * @param v - The input vector to be multiplied
 * @param m - The input matrix to multiply by
 */
void matMultVec4f(float vout[4], const float v[4], float m[16])
{
    float c[4];
    vecCopy4f(c, v);
    vout[0] = m[0] * c[0] + m[4] * c[1] + m[8]  * c[2] + m[12] * c[3];
    vout[1] = m[1] * c[0] + m[5] * c[1] + m[9]  * c[2] + m[13] * c[3];
    vout[2] = m[2] * c[0] + m[6] * c[1] + m[10] * c[2] + m[14] * c[3];
    vout[3] = m[3] * c[0] + m[7] * c[1] + m[11] * c[2] + m[15] * c[3];
} /* matMultVec4f() */

/**
 * Transforms a vector by the current modelview matrix
 * from Professor Shafae
 * @param outVec - The transformed vector to be returned
 * @param inVec - The input vector to transform
 */
void transformVecByModelView(float outVec[4], const float inVec[4])
{
    float modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    matMultVec4f(outVec, inVec, modelview);
} /* transformVecByModelView() */

/**
 * Prints the contents of a 4x4 matrix of doubles
 * from Professor Shafae
 * @param varName - The identifier of the matrix being printed
 * @param matrix - The matrix to print
 */
void msglPrintMatrix16dv(const char *varName, double matrix[16])
{
    int i = 0;

    if (varName != NULL)
    {
        fprintf(stderr, "%s = [\n", varName);
    }

    for (i = 0; i < 4; i++)
    {
        fprintf(stderr, "%.5f %.5f %.5f %.5f\n", matrix[i + 0], matrix[i + 4], matrix[i + 8],
                matrix[i + 12]);
    }

    if (varName != NULL)
    {
        fprintf(stderr, " ]\n");
    }
} /* msglPrintMatrix16dv() */

/**
 * Prints the matrices stored on top of the projection and modelview stacks
 * from Professor Shafae
 * @param msg - A text message to print before the matrices
 */
void printTopOfBothStacks(const char* msg)
{
    double matrix[16];

    msglError();

    if (msg != NULL)
    {
        fprintf(stderr, "%s\n", msg);
    }

    fprintf(stderr, "Top of Projection Stack:\n");
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    msglPrintMatrix16dv("proj", matrix);
    fprintf(stderr, "\n\n" );
    fprintf(stderr, "Top of Modelview Stack:\n");
    glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
    msglPrintMatrix16dv("mv", matrix);

    msglError();
} /* printTopOfBothStacks() */

/**
 * Prints OpenGL version debug info
 * from Professor Shafae
 */
void msglVersion()
{
    fprintf(stderr, "Vendor: %s\n", glGetString(GL_VENDOR));
    fprintf(stderr, "Renderer: %s\n", glGetString(GL_RENDERER));
    fprintf(stderr, "OpenGL Version: %s\n", glGetString(GL_VERSION));
    fprintf(stderr, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
} /* msglVersion() */
