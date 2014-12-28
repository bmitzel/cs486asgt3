 * Programmer: Brian Mitzel
 * Email: bmitzel@csu.fullerton.edu
 * Course: CPSC 486
 * Assignment: 3 - View Frustum Culling
 * Due Date: 11/17/2014
 
vfculling.cpp is a C++ program that implements view frustum
culling, using a virtual trackball for the camera control
and axis-aligned bounding boxes for collision detection.

Three PLY models are rendered in a scene with a ground plane
and a sky box.

When the user clicks with the left mouse button on one
of the rendered models, its bounding volume is drawn.
Whenever a bounding volume (visible or no) is in contact
with an edge of the view frustum or entirely outside of the
view frustum, the corresponding model is culled (not drawn).

The virtual trackball is activated by holding down the
shift key along with the left mouse button. Then, drawing a
path on the screen will rotate the camera in first-person
perspective accordingly.

The user is able to toggle the program's lighting
between the custom GLSL shader program and the OpenGL
fixed function pipeline. Also, the user can toggle on or off
the drawing of all the bounding volumes at once.

See below for the full feature set.

------------------------------------------------------------
    
Features:

The following hotkeys are available:
    b - toggle rendering the bounding volumes
    f - toggle full screen mode (freeglut only)
    g - toggle between the GLSL program and the fixed
        function pipeline
    o - reset the window to its original resolution
    ESC or q - quit the program
    h - print a help message
    
Clicking the mouse on a model in the scene toggles on or off
the drawing of that model's axis-aligned bounding box.

Holding down the shift key and the left mouse button
activates the virtual trackball for controlling the camera.
The camera can then be rotated in first-person perspective
by dragging the mouse across the screen.

------------------------------------------------------------

Completed:

The following requirements for this assignment have been
completed:
    1. Render a scene with 3 smoothly shaded geometric
       models
    2. All 3 of the models are stored in PLY format
    3. The model data is stored in a linear, linked data
       structure
    4. Each object in the scene is bounded by an axis-
       aligned bounding box
    5. The scene includes a shaded ground plane and shaded
       sky box
    6. The entire scene is visible from the default camera
       position
    7. There is an interior point light source which
       illuminates the scene
    8. The shading is accomplished using the Blinn-Phong
       GLSL shader program
    9. The keyboard and mouse interface is fully functional
       as described in the Features section above
   10. View frustum culling is fully functional whenever a
       model's axis-aligned bounding box is in contact with
       or entirely outside of the view frustum
   11. Picking via ray/AABB intersection is fully
       functional and toggles the drawing of each model's
       bounding volume
   12. Virtual trackball camera controls are fully
       implemented as described in the assignment.

------------------------------------------------------------

Incomplete:

The only required feature for this assignment that has not
been implemented is texturing one or more of the models.
       
------------------------------------------------------------

Bonus:

All 3 PLY models are animated as bouncing and rotating
within the scene.
       
------------------------------------------------------------
       
Bugs:

There are currently no known bugs.

------------------------------------------------------------

Building:

Using OSX, Linux, or the FreeBSD VM distributed by Professor
Michael Shafae, open a terminal window to the source
directory and enter the following command at the shell
prompt:

    make

or:

    gmake

------------------------------------------------------------
    
Executing:

From the same directory where you built the executable,
enter the command:

    ./vfculling [<window_width> <window_height>]
        window_width: The optional width of the window
        window_height: The optional height of the window
        
The window width and height default to 1280 x 720 if
omitted from the command line.
