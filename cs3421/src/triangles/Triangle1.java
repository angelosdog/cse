package triangles;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.media.opengl.GL;
import javax.media.opengl.GL2;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLProfile;
import javax.media.opengl.awt.GLJPanel;
import javax.swing.JFrame;

/**
 * A simple example of writing a JOGL program that draws a triangle in a window.
 *
 * @author malcolmr
 */

public class Triangle1 implements GLEventListener {

    public static void main(String[] args) {
        // Initialise OpenGL
        GLProfile glp = GLProfile.getDefault();
        GLCapabilities caps = new GLCapabilities(glp);

        // Create a panel to draw on
        GLJPanel panel = new GLJPanel(caps);

        // Put it in a window
        final JFrame jframe = new JFrame("Triangle 1");
        jframe.setSize(300, 300);
        jframe.add(panel);
        jframe.setVisible(true);

        // Catch window closing events and quit
        jframe.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                jframe.dispose();
                System.exit(0);
            }
        });

        // add a GL Event listener to handle rendering
        panel.addGLEventListener(new Triangle1());
    }

    @Override
    public void display(GLAutoDrawable drawable) {
        // redraw the scene

        GL2 gl = drawable.getGL().getGL2();

        // clear the window
        gl.glClear(GL.GL_COLOR_BUFFER_BIT);

        // draw a triangle filling the window
        gl.glBegin(GL.GL_TRIANGLES);
        {
            gl.glColor3f(1, 0, 0);
            gl.glVertex2d(-1, -1);

            gl.glColor3f(0, 1, 0);
            gl.glVertex2d(0, 1);

            gl.glColor3f(0, 0, 1);
            gl.glVertex2d(1, -1);
        }
        gl.glEnd();
    }

    @Override
    public void init(GLAutoDrawable drawable) {
        // do any initialisation
    }

    @Override
    public void dispose(GLAutoDrawable drawable) {
        // dispose of any allocated resources
    }

    @Override
    public void reshape(GLAutoDrawable drawable, int x, int y, int w, int h) {
        // respond to the window being resized
    }

}
