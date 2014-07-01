/* 
 *  Simple trackball-like motion adapted (ripped off) from projtex.c
 *  (written by David Yu and David Blythe).  See the SIGGRAPH '96
 *  Advanced OpenGL course notes.
 *
 *
 *  Usage:
 *  
 *  o  call strokeInit() in before any other stroke call
 *  o  call strokeReshape() from the reshape callback
 *  o  call strokeMatrix() to get the trackball matrix rotation
 *  o  call strokeStartMotion() to begin trackball movememt
 *  o  call strokeStopMotion() to stop trackball movememt
 *  o  call strokeMotion() from the motion callback
 *  o  call strokeAnimate(GL_TRUE) if you want the trackball to continue 
 *     spinning after the mouse button has been released
 *  o  call strokeAnimate(GL_FALSE) if you want the trackball to stop 
 *     spinning after the mouse button has been released
 *
 *  Typical setup:
 *
 *
    void
    init(void)
    {
      strokeInit(GLUT_MIDDLE_BUTTON);
      strokeAnimate(GL_TRUE);
      . . .
    }

    void
    reshape(int width, int height)
    {
      strokeReshape(width, height);
      . . .
    }

    void
    display(void)
    {
      glPushMatrix();

      strokeMatrix();
      . . . draw the scene . . .

      glPopMatrix();
    }

    void
    mouse(int button, int state, int x, int y)
    {
      strokeMouse(button, state, x, y);
      . . .
    }

    void
    motion(int x, int y)
    {
      strokeMotion(x, y);
      . . .
    }

    int
    main(int argc, char** argv)
    {
      . . .
      init();
      glutReshapeFunc(reshape);
      glutDisplayFunc(display);
      glutMouseFunc(mouse);
      glutMotionFunc(motion);
      . . .
    }
 *
 * */


#ifndef __PEN_STROKE_H
#define __PEN_STROKE_H

/* functions */
//void
//strokeInit(GLuint button);

//void strokeMatrix();

void
strokeReshape(int width, int height);

void
strokeMouse(int button, int state, int x, int y);

void strokeMotion(int x, int y);


//void
//strokeAnimate(GLboolean animate);

#endif //__PEN_STROKE_H
