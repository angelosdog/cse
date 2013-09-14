package ass1;

import javax.media.opengl.GL;
import javax.media.opengl.GL2;
import javax.media.opengl.glu.GLU;

/**
 * The camera is a GameObject that can be moved, rotated and scaled like any other.
 *
 * @author malcolmr, vtra143
 */
public class Camera extends GameObject {

    private float[] myBackground;
    private double myAspect;

    public Camera(GameObject parent) {
        super(parent);
        myBackground = new float[4];
        myAspect = 1.0;
    }

    public Camera() {
        this(GameObject.ROOT);
    }
    
    public float[] getBackground() {
        return myBackground;
    }

    public void setBackground(float[] background) {
        myBackground = background;
    }

    public void setView(GL2 gl) {
        gl.glClearColor(myBackground[0], myBackground[1], myBackground[2], myBackground[3]);
        gl.glClear(GL.GL_COLOR_BUFFER_BIT);
        gl.glMatrixMode(GL2.GL_MODELVIEW);
        gl.glLoadIdentity();
        gl.glScaled(1 / (this.getGlobalScale() * myAspect), 1 / this.getGlobalScale(), 1);
        gl.glRotated(-this.getGlobalRotation(), 0, 0, 1);
        gl.glTranslated(-this.getGlobalPosition()[0], -this.getGlobalPosition()[1], 0);
    }

    public void reshape(GL2 gl, int x, int y, int width, int height) {
        myAspect = 1.0 * width / height;
        
        gl.glMatrixMode(GL2.GL_PROJECTION);
        gl.glLoadIdentity();
        
        // coordinate system (left, right, bottom, top)
        GLU glu = new GLU();
        
        if (this.isShowing()) {
            glu.gluOrtho2D(-1, 1, -1, 1);
        }
        else {            
            glu.gluOrtho2D(-20, 20, -20, 20);
        }
    }
}
