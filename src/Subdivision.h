#ifndef _SUBDIVISION_H
#define _SUBDIVISION_H

#include "Math.h"
#include "Anchor.h"
#include "Particle.h"

class Content
{
public:
    Content(){anchors = new std::vector< Anchor * >(); particles1 = new std::vector< Particle * >(); strength = cinder::Vec3f::zero();};
    void Update(float aTimeInterval);
    void AddStrength(ci::Vec3f aStrength);
    std::vector< Anchor * > * anchors;
    std::vector< Particle * > * particles1;
    ci::Vec3f strength;
    bool mIsInitialized;
};

/*
 *  Manage the subdivisions and the contents.
 *  Used to avoid to iterater over all the points.
 */
class Subdivision{

public:
    /*
     *  Constructor.
     */
    Subdivision();
    
    /*
     *  Update.
     */
    void Update(float aTimeInterval);
    
    /*
	 * Creation of the singleton, return the instance.
	 */
    static Subdivision * getInstance ();
    
    /*
     *  Get vector of content.
     */
    Content * GetContent(float x, float y);
    Content * GetContent(int ax, int ay);
    Content * GetContent(int aIndex);
    
    /*
     *  look for the closest content of the case.
     */
    std::vector< Anchor * > * GetClosestContent(float ax, float ay, int &aDistance);
    
    /*
     *  same as GetClosestContent + random throuht result.
     */
    Anchor * GetClosestContentRandom(float ax, float ay, int &aDistance);
    
    /*
     *  get strength at specific position.
     */
    cinder::Vec3f GetStrength(float ax, float ay);
    
    /*
     *  get num part.
     */
    int GetNumPart(float aX, float aY);
    
    /*
     * Clear content.
     */
    void Clear();
    
    /*
     * Add an Anchor in the proper content case.
     */
    void AddAnchor(Anchor * aAnchor);
    
    /*
     * Add a particle 1 in the proper content case.
     */
    void Add(Particle * aAnchor);
    
    /*
     * Add a strength at a position.
     */
    void AddStrength(cinder::Vec3f aStrength, cinder::Vec3f aPosition1, cinder::Vec3f aPosition2);
    
    /*
     * Add an Anchor in the proper content case.
     */
    void drawDebug();
    
private:
    /*
     *  Get index with coordinates in the grid (int) or relatively.
     */
    int GetIndex(float ax, float ay);
    int GetIndex(int aX, int aY);
    
    /*
     *  Get coordinates of the positions in the grid.
     */
    cinder::Vec2i GetCoordinates(float ax, float ay);
    
    std::vector< Content * > mContent;
    
    
    static Subdivision* _Subdivisionsingleton;
};

#endif
