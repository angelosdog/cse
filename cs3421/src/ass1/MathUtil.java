package ass1;

/**
 * A collection of useful math methods 
 *
 * @author malcolmr, vtra143
 */
public class MathUtil {

    /**
     * Normalise an angle to the range (-180, 180]
     * 
     * @param angle 
     * @return
     */
    static public double normaliseAngle(double angle) {
        return ((angle + 180.0) % 360.0 + 360.0) % 360.0 - 180.0;
    }

    /**
     * Clamp a value to the given range
     * 
     * @param value
     * @param min
     * @param max
     * @return
     */

    public static double clamp(double value, double min, double max) {
        return Math.max(min, Math.min(max, value));
    }

    /**
     * Multiply two matrices
     * 
     * @param p A 3x3 matrix
     * @param q A 3x3 matrix
     * @return
     */
    public static double[][] multiply(double[][] p, double[][] q) {

        double[][] m = new double[3][3];

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m[i][j] = 0;
                for (int k = 0; k < 3; k++) {
                    m[i][j] += p[i][k] * q[k][j]; 
                }
            }
        }

        return m;
    }

    /**
     * Multiply a vector by a matrix
     * 
     * @param m A 3x3 matrix
     * @param v A 3x1 vector
     * @return
     */
    public static double[] multiply(double[][] m, double[] v) {

        double[] u = new double[3];

        for (int i = 0; i < 3; i++) {
            u[i] = 0;
            for (int j = 0; j < 3; j++) {
                u[i] += m[i][j] * v[j];
            }
        }

        return u;
    }
    /**
     * Invert a 3x3 matrix using the general formula:
     *                  1                   [ei - fh][ch - bi][bf - ce]
     * ------------------------------------ [fg - di][ai - cg][cd - af]
     * a(ei - fh) - b(di - fg) - c(dh - eg) [dh - eg][bg - ah][ae - bd]
     *  
     * for the general matrix:
     * [a][b][c]
     * [d][e][f]
     * [g][h][i]
     * 
     * @param m A 3x3 matrix
     * @return
     */  
    public static double[][] invert(double[][] m) {
        // this just works, don't question it
        double a = m[0][0];
        double b = m[0][1];
        double c = m[0][2];
        double d = m[1][0];
        double e = m[1][1];
        double f = m[1][2];
        double g = m[2][0];
        double h = m[2][1];
        double i = m[2][2];
        double det = 1/(a*(e*i - f*h) - b*(d*i - f*g) - c*(d*h - e*g));
        double[][] inverse = {{e*i - f*h, c*h - b*i, b*f - c*e},
                              {f*g - d*i, a*i - c*g, c*d - a*f},
                              {d*h - e*g, b*g - a*h, a*e - b*d}};
        
        // used i as a variable name already...
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                inverse[j][k] *= det;
            }
        }
        return inverse;
    }
    /**
     * Print out a 3x3 matrix with tab separated values
     * 
     * @param m A 3x3 matrix
     * @return
     */  
    public static void printMatrix(double[][] m) {
        for(int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                System.out.print(m[i][j] + "\t");
            }
            System.out.print("\n");
        }
    }

    /**
     * @param pos
     * @return
     */
    public static double[][] translationMatrix(double[] v) {
        double[][] tm = {{1,0,v[0]},{0,1,v[1]},{0,0,1}};
        return tm;
    }

    /**
     * @param angle
     * @return
     */
    public static double[][] rotationMatrix(double angle) {
        angle = Math.toRadians(angle);
        double[][] rm = {{Math.cos(angle),-Math.sin(angle),0},{Math.sin(angle),Math.cos(angle),0},{0,0,1}}; 
        return rm;
    }

    /**
     * @param scale
     * @return
     */
    public static double[][] scaleMatrix(double scale) {
        double[][] sm = {{scale, 0, 0}, {0, scale,0}, {0,0,1}};
        return sm;
    }


}
