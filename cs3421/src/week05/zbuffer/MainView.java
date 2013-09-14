package week05.zbuffer;

import java.awt.Point;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.nio.Buffer;
import java.nio.FloatBuffer;

import javax.media.opengl.GL;
import javax.media.opengl.GL2;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.glu.GLU;

import week05.Polygon;

/**
 * 3D view with ZBuffer display 
 *
 * @author malcolmr
 */
public class MainView implements GLEventListener, MouseMotionListener, KeyListener {

    private static final int ROTATION_SCALE = 1;
    private ZBufferExample myModel;
    private Point myMousePoint;
    private int myRotateX = 0;
    private int myRotateY = 0;

    private boolean myWireframe = false;
    private boolean myPerspective = false;
    private boolean myShowDepth = false;
    private int myWidth;
    private int myHeight;
    
    public MainView(ZBufferExample model) {
        myModel = model;
    }

    @Override
    public void init(GLAutoDrawable drawable) {
        GL2 gl = drawable.getGL().getGL2();

        // enable depth testing
        gl.glEnable(GL.GL_DEPTH_TEST);

    }

    @Override
    public void dispose(GLAutoDrawable drawable) {
    }

    @Override
    public void display(GLAutoDrawable drawable) {
        GL2 gl = drawable.getGL().getGL2();

        // set either a perspective or othographic camera
        gl.glMatrixMode(GL2.GL_PROJECTION);
        gl.glLoadIdentity();

        if (myPerspective) {
            GLU glu = new GLU();
            glu.gluPerspective(60, 1, 1, 8);            
        }
        else {
            gl.glOrtho(-3, 3, -3, 3, 1, 8);
        }

        // clear the window and the depth buffer
        gl.glClearColor(1, 1, 1, 1);
        gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);

        gl.glMatrixMode(GL2.GL_MODELVIEW);
        gl.glLoadIdentity();
        gl.glRotated(myRotateX, 1, 0, 0);
        gl.glRotated(myRotateY, 0, 1, 0);
        
        // draw the polygons
        
        if (myWireframe) {
            gl.glPolygonMode(GL2.GL_FRONT_AND_BACK, GL2.GL_LINE);
        }
        else {
            gl.glPolygonMode(GL2.GL_FRONT_AND_BACK, GL2.GL_FILL);            
        }

        for (Polygon p : myModel.getPolygons()) {
            p.draw(gl);
        }
        gl.glFlush();

        // if the show-depth flag is on, copy the depth-buffer
        // onto the screen as luminance, so depths are displayed as levels of grey
        
        if (myShowDepth) {
            Buffer pixels = FloatBuffer.allocate(myWidth * myHeight);
            gl.glReadPixels(0, 0, myWidth, myHeight, GL2.GL_DEPTH_COMPONENT, GL2.GL_FLOAT, pixels );
            gl.glDrawPixels(myWidth, myHeight, GL2.GL_LUMINANCE, GL2.GL_FLOAT, pixels);
        }
        
    }

    @Override
    public void reshape(GLAutoDrawable drawable, int x, int y, int width,
            int height) {

        GL2 gl = drawable.getGL().getGL2();

        myWidth = width;
        myHeight = height;
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        myMousePoint = e.getPoint();
    }

    @Override
    public void mouseDragged(MouseEvent e) {
        Point p = e.getPoint();

        if (myMousePoint != null) {
            int dx = p.x - myMousePoint.x;
            int dy = p.y - myMousePoint.y;

            // Note: dragging in the x dir rotates about y
            //       dragging in the y dir rotates about x
            myRotateY += dx * ROTATION_SCALE;
            myRotateX += dy * ROTATION_SCALE;

        }
        myMousePoint = p;
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    @Override
    public void keyPressed(KeyEvent e) {
    }

    @Override
    public void keyReleased(KeyEvent e) {
        switch (e.getKeyCode()) {
        case KeyEvent.VK_W:
            myWireframe = !myWireframe;
            break;
            
        case KeyEvent.VK_P:
            myPerspective = !myPerspective;
            break;
            
        case KeyEvent.VK_Z:
            myShowDepth  = !myShowDepth;
            break;
        }
    }

    
}
