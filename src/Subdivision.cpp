#include "Subdivision.h"
#include "Tools.h"
#include "Set.h"
#include "cinder/Rand.h"

#define NUM_SUBDIVISION_SIDE 60
#define STRENGTH_RANGE 13

using namespace std;
using namespace ci;

Subdivision *Subdivision::_Subdivisionsingleton = NULL;

/*
 * Creation of the singleton, return the instance.
 */
Subdivision * Subdivision::getInstance ()
{
    if (NULL == _Subdivisionsingleton)
    {
        printf("creating File finder singleton.\n");
        _Subdivisionsingleton =  new Subdivision;
    }
    
    return _Subdivisionsingleton;
}

Subdivision::Subdivision()
{
    for(int i = 0; i < NUM_SUBDIVISION_SIDE * NUM_SUBDIVISION_SIDE; i++)
    {
        mContent.push_back(new Content());
    }
}

void Subdivision::Update(float aTimeInterval)
{
    for(int i = 0; i < mContent.size(); i++)
    {
        mContent[i]->Update(aTimeInterval);
    }
}

Content * Subdivision::GetContent(float ax, float ay)
{
    int lIndex = GetIndex(ax, ay);
    return GetContent(lIndex);
}

Content * Subdivision::GetContent(int ax, int ay)
{
    int lIndex = GetIndex(ax, ay);
    return GetContent(lIndex);
}

Content * Subdivision::GetContent(int aIndex)
{
    if(aIndex >= 0)
    {
        return mContent[aIndex];
    }
    else
    {
        return NULL;
    }
}

Anchor * Subdivision::GetClosestContentRandom(float ax, float ay, int &aDistance)
{
    std::vector< Anchor * > * lClosest = GetClosestContent(ax, ay, aDistance);
    
    if(lClosest)
    {
        if(lClosest->size() > 0)
        {
            int index = math<int>::clamp(floor(Rand::randInt(lClosest->size())), 0, lClosest->size() - 1);
            return lClosest->at(index);
        }
    }
    
    return NULL;
}

vector< Anchor * > * Subdivision::GetClosestContent(float ax, float ay, int &aDistance)
{
    bool lIsSearching = true;;
    int ly, lx;
    Content * lContent;
    Vec2i lPositionGrid = GetCoordinates(math<float>::clamp(ax, 0.f, 1.f), math<float>::clamp(ay, 0.f, 1.f));
    int lMaxPart = 73;
    int lMinPartIn = 100;
    vector< Anchor * > * lMinPartInContent = NULL;
    while(lIsSearching)
    {
        // top
        ly = aDistance;
        for(int x = -aDistance; x <= aDistance; x++)
        {
            lContent = GetContent(lPositionGrid.x + x, lPositionGrid.y + ly);
//            cout << "try " << lPositionGrid.x + x << " " << lPositionGrid.y + ly << endl;
            if(lContent != NULL)
            {
                if(lContent->anchors->size() > 0 && lContent->particles1->size() < lMaxPart )
                {
                    lMinPartInContent = lContent->anchors;
                    lMinPartIn = lContent->particles1->size();
                }
            }
        }
        // bottom
        ly = -aDistance;
        for(int x = -aDistance; x <= aDistance; x++)
        {
            lContent = GetContent(lPositionGrid.x + x, lPositionGrid.y + ly);
            if(lContent != NULL)
            {
                if(lContent->anchors->size() > 0 && lContent->particles1->size() < lMaxPart )
                {
                    if(lMinPartIn > lContent->particles1->size())
                    {
                        lMinPartInContent = lContent->anchors;
                        lMinPartIn = lContent->particles1->size();
                    }
                }
            }
        }
        // left
        lx = -aDistance;
        for(int y = -aDistance + 1; y <= aDistance - 1; y++)
        {
            lContent = GetContent(lPositionGrid.x + lx, lPositionGrid.y + y);
            if(lContent != NULL)
            {
                if(lContent->anchors->size() > 0 && lContent->particles1->size() < lMaxPart )
                {
                    if(lMinPartIn > lContent->particles1->size())
                    {
                        lMinPartInContent = lContent->anchors;
                        lMinPartIn = lContent->particles1->size();
                    }
                }
            }
        }
        // right
        lx = aDistance;
        for(int y = -aDistance + 1; y <= aDistance - 1; y++)
        {
            lContent = GetContent(lPositionGrid.x + lx, lPositionGrid.y + y);
            if(lContent != NULL)
            {
                if(lContent->anchors->size() > 0 && lContent->particles1->size() < lMaxPart )
                {
                    if(lMinPartIn > lContent->particles1->size())
                    {
                        lMinPartInContent = lContent->anchors;
                        lMinPartIn = lContent->particles1->size();
                    }
                }
            }
        }
        if(lMinPartInContent != NULL)
        {
            return lMinPartInContent;
        }
        aDistance++;
        if(aDistance >= NUM_SUBDIVISION_SIDE * 0.3)
        {
            return NULL;
        }
    }
    return NULL;
}

Vec2i Subdivision::GetCoordinates(float ax, float ay)
{
    Vec2i lPositionGrid;
    lPositionGrid.x = floor(ax * (float)NUM_SUBDIVISION_SIDE);
    lPositionGrid.y = floor(ay * (float)NUM_SUBDIVISION_SIDE);

    return lPositionGrid;
}

int Subdivision::GetIndex(float ax, float ay)
{
    Vec2i lPositionGrid = GetCoordinates(ax, ay);
    return GetIndex(lPositionGrid.x, lPositionGrid.y);
}
       
int Subdivision::GetIndex(int aX, int aY)
{
    int rIndex = aX + NUM_SUBDIVISION_SIDE * aY;
    if((rIndex < 0) || (rIndex >= mContent.size()))
    {
        rIndex = -1;
    }
    return rIndex;
}

int Subdivision::GetNumPart(float aX, float aY)
{
//    Content * lContent;// = GetContent(aX, aY);
    Vec2i lCenter = GetCoordinates(aX, aY);
    int partSize = 0;
    int range = 5;
    Content * lContent;
    for(int x = -range; x < range; x++)
    {
        for(int y = -range; y < range; y++)
        {
            lContent = GetContent(lCenter.x + x, lCenter.y + y);
            if(lContent != NULL)
            {
                partSize += lContent->particles1->size();
            }
        }
    }
    
    return partSize;
}

void Subdivision::Clear()
{
    for(int i = 0; i < mContent.size(); i++)
    {
        mContent[i]->anchors->clear();
        mContent[i]->particles1->clear();
    }
}

void Subdivision::AddAnchor(Anchor * aAnchor)
{
    int lIndex = GetIndex(aAnchor->mPosition.x, aAnchor->mPosition.y);
    if(lIndex >= 0)
    {
        mContent[lIndex]->anchors->push_back(aAnchor);
    }
}

void Subdivision::Add(Particle * aPart)
{
    int lIndex = GetIndex(aPart->mPosition.x, aPart->mPosition.y);
    if(lIndex >= 0)
    {
        mContent[lIndex]->particles1->push_back(aPart);
    }
}

void Subdivision::drawDebug()
{
    for(int x = 0; x < NUM_SUBDIVISION_SIDE; x++)
    {
        for(int y = 0; y < NUM_SUBDIVISION_SIDE; y++)
        {
            Vec3f strength = GetContent(x, y)->strength;
        }
    }
}

void Subdivision::AddStrength(Vec3f aStrength, Vec3f aPosition1, Vec3f aPosition2)
{
    Vec2i lCenter1 = GetCoordinates(aPosition1.x, aPosition1.y);
    Vec2i lCenter2 = GetCoordinates(aPosition2.x, aPosition2.y);
    Vec2i lCenter;
    
    int stepCount = max(abs(lCenter1.x - lCenter2.x), abs(lCenter1.y - lCenter2.y));
    float lStepCoeff;
    for(int i = 0; i < stepCount; i++)
    {
        lStepCoeff = (float)i / (float)stepCount;
        lCenter.x = lCenter2.x + lStepCoeff * (lCenter1.x - lCenter2.x);
        lCenter.y = lCenter2.y + lStepCoeff * (lCenter1.y - lCenter2.y);
        int range = STRENGTH_RANGE * (abs(aStrength.x) + abs(aStrength.y)) * 30;
        range = math<float>::clamp(range, STRENGTH_RANGE * 0.8, STRENGTH_RANGE);
        Content * lContent;
        float lDistance;
        for(int x = -range; x < range; x++)
        {
            for(int y = -range; y < range; y++)
            {
                lContent = GetContent(lCenter.x + x, lCenter.y + y);
                if(lContent != NULL)
                {
                    lDistance = abs(x) + abs(y);
                    lDistance = 1.f - ((float)lDistance / (2. * (float)range));
                    lContent->AddStrength(aStrength * lDistance * lDistance);
                }
            }
        }
    }
}

Vec3f Subdivision::GetStrength(float ax, float ay)
{
    Content * lContent = GetContent(ax, ay);
    if(lContent != NULL)
        return lContent->strength;
    else
        return Vec3f::zero();
}

void Content::Update(float aTimeInterval)
{
    strength *= Set::frictionStr;
}

void Content::AddStrength(Vec3f aStrength)
{
    strength += aStrength;
}