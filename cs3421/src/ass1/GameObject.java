package ass1;

import java.util.ArrayList;
import java.util.List;

import javax.media.opengl.GL2;


/**
 * A GameObject is an object that can move around in the game world.
 * 
 * GameObjects form a scene tree. The root of the tree is the special ROOT object.
 * 
 * Each GameObject is offset from its parent by a rotation, a translation and a scale factor. 
 *
 * @author malcolmr, vtra143
 */
public class GameObject {

    // the list of all GameObjects in the scene tree
    public final static List<GameObject> ALL_OBJECTS = new ArrayList<GameObject>();
    
    // the root of the scene tree
    public final static GameObject ROOT = new GameObject();
    
    // the links in the scene tree
    private GameObject myParent;
    private List<GameObject> myChildren;

    // the local transformation
    private double myRotation;
    private double myScale;
    private double[] myTranslation;
    
    // the model view transform of the current object
    private double[][] modelViewTransform;
    
    // is this part of the tree showing?
    private boolean amShowing;

    /**
     * Special private constructor for creating the root node. Do not use otherwise.
     */
    private GameObject() {
        myParent = null;
        myChildren = new ArrayList<GameObject>();

        myRotation = 0;
        myScale = 1;
        myTranslation = new double[2];
        myTranslation[0] = 0;
        myTranslation[1] = 0;
        
        // Initialise the model view transform matrix to the identity
        modelViewTransform = new double[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i == j) modelViewTransform[i][j] = 1;
                else modelViewTransform[i][j] = 0;
            }
        }
        
        amShowing = true;
        
        ALL_OBJECTS.add(this);
    }

    /**
     * Public constructor for creating GameObjects, connected to a parent (possibly the ROOT).
     *  
     * New objects are created at the same location, orientation and scale as the parent.
     *
     * @param parent
     */
    public GameObject(GameObject parent) {
        myParent = parent;
        myChildren = new ArrayList<GameObject>();

        parent.myChildren.add(this);

        myRotation = 0;
        myScale = 1;
        myTranslation = new double[2];
        myTranslation[0] = 0;
        myTranslation[1] = 0;
        
        // Initialise the model view transform to that of its parent
        modelViewTransform = myParent.modelViewTransform;
        
        // initially showing
        amShowing = true;

        ALL_OBJECTS.add(this);
    }

    /**
     * Remove an object and all its children from the scene tree.
     */
    public void destroy() {
        for (GameObject child : myChildren) {
            child.destroy();
        }
        
        myParent.myChildren.remove(this);
        ALL_OBJECTS.remove(this);
    }

    /**
     * Get the parent of this game object
     * 
     * @return
     */
    public GameObject getParent() {
        return myParent;
    }

    /**
     * Get the children of this object
     * 
     * @return
     */
    public List<GameObject> getChildren() {
        return myChildren;
    }

    /**
     * Get the local rotation (in degrees)
     * 
     * @return
     */
    public double getRotation() {
        return myRotation;
    }

    /**
     * Set the local rotation (in degrees)
     * 
     * @return
     */
    public void setRotation(double rotation) {
        myRotation = rotation;
        this.computeModelTransform();
    }

    /**
     * Rotate the object by the given angle (in degrees)
     * 
     * @param angle
     */
    public void rotate(double angle) {
        myRotation += angle;
        this.computeModelTransform();
    }
    
    /**
     * Get the local scale
     * 
     * @return
     */
    public double getScale() {
        return myScale;
    }

    /**
     * Set the local scale
     * 
     * @param scale
     */
    public void setScale(double scale) {
        myScale = scale;
        this.computeModelTransform();
    }
    
    /**
     * Multiply the scale of the object by the given factor
     * 
     * @param factor
     */
    public void scale(double factor) {
        myScale *= factor;
        this.computeModelTransform();
    }
    
    /**
     * Get the local position of the object 
     * 
     * @return
     */
    public double[] getPosition() {
        double[] t = new double[2];
        t[0] = myTranslation[0];
        t[1] = myTranslation[1];
        return t;
    }

    /**
     * Set the local position of the object
     * 
     * @param x
     * @param y
     */
    public void setPosition(double x, double y) {
        myTranslation[0] = x;
        myTranslation[1] = y;
        this.computeModelTransform();
    } 
    
    /**
     * Move the object by the specified offset in local coordinates
     * 
     * @param dx
     * @param dy
     */
    public void translate(double dx, double dy) {
        myTranslation[0] += dx;
        myTranslation[1] += dy;
        this.computeModelTransform();
    }
    
    /**
     * Test if the object is visible
     * 
     * @return
     */
    public boolean isShowing() {
        return amShowing;
    }

    /**
     * Set the showing flag to make the object visible (true) or invisible (false).
     * This flag should also apply to all descendents of this object.
     * 
     * @param showing
     */
    public void show(boolean showing) {
        amShowing = showing;
    }

    /**
     * Update the object. This method is called once per frame. 
     * 
     * This does nothing in the base GameObject class. Override this in subclasses.
     * 
     * @param dt The amount of time since the last update (in seconds)
     */
    public void update(double dt) {
        // do nothing
    }

    /**
     * Draw the object (but not any descendants)
     * 
     * This does nothing in the base GameObject class. Override this in subclasses.
     * 
     * @param gl
     */
    public void drawSelf(GL2 gl) {
        // do nothing
    }
    
    public double[][] getModelViewTransform(){
        return modelViewTransform;
    }
    
    /**
     * Compute the model view transform from the parent matrix and local transformations
     */ 
    private void computeModelTransform() {
        modelViewTransform = MathUtil.multiply(myParent.modelViewTransform, MathUtil.translationMatrix(myTranslation));
        modelViewTransform = MathUtil.multiply(modelViewTransform, MathUtil.rotationMatrix(myRotation));
        modelViewTransform = MathUtil.multiply(modelViewTransform, MathUtil.scaleMatrix(myScale));
        for(GameObject child : myChildren) {
            child.computeModelTransform();
        }
    }
    
    /**
     * Draw the object and all of its descendants recursively.
     * 
     * @param gl
     */
    public void draw(GL2 gl) {
        
        // don't draw if it is not showing
        if (!amShowing) {
            return;
        }
        gl.glMatrixMode(GL2.GL_MODELVIEW);
        gl.glPushMatrix();
        {
            gl.glTranslated(myTranslation[0], myTranslation[1], 0);
            gl.glRotated(myRotation, 0, 0, 1);
            gl.glScaled(myScale, myScale, 1);
            drawSelf(gl);
            for(GameObject child:this.getChildren()) {
                child.draw(gl);
            }
        }
        gl.glPopMatrix();
    }

    /**
     * Compute the object's position in world coordinates
     * 
     * @return a point in world coordinates in [x,y] form
     */
    public double[] getGlobalPosition() {
        double[] p = {modelViewTransform[0][2], modelViewTransform[1][2], 1};
        return p;
    }

    /**
     * Compute the object's rotation in the global coordinate frame
     * 
     * @return the global rotation of the object (in degrees) 
     */
    public double getGlobalRotation() {
        return Math.toDegrees(Math.atan2(modelViewTransform[1][0], modelViewTransform[0][0]));
    }

    /**
     * Compute the object's scale in global terms
     * 
     * @return the global scale of the object 
     */
    public double getGlobalScale() {
        return Math.sqrt(Math.pow(modelViewTransform[0][0],2) + Math.pow(modelViewTransform[1][0], 2)+ Math.pow(modelViewTransform[2][0], 2));
    }

    /**
     * Change the parent of a game object.
     * 
     * @param parent
     */
    public void setParent(GameObject parent) {
        
        // store the current global rotation and scale
        double rotation = this.getGlobalRotation();
        double scale = this.getGlobalScale();
        
        // change parents
        myParent.myChildren.remove(this);
        myParent = parent;
        myParent.myChildren.add(this);
       
        // calculate the new origin
        double[] newPosition = MathUtil.multiply(MathUtil.invert(myParent.modelViewTransform), this.getGlobalPosition());
        
        // set the local transforms to suit
        this.setPosition(newPosition[0], newPosition[1]);
        this.setRotation(rotation - myParent.getGlobalRotation());
        this.setScale(scale/myParent.getGlobalScale());
    }

    public boolean containsPoint(double[] p) {
        return false;
    }
}