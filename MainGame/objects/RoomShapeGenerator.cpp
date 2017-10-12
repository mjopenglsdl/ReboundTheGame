#include "Room.hpp"

#include <vector>
#include <memory>
#include <utility>

#include <cppmunk/Space.h>
#include <cppmunk/Body.h>
#include <cppmunk/SegmentShape.h>
#include <cppmunk/CircleShape.h>
#include <cppmunk/PolyShape.h>
#include "data/RoomData.hpp"
#include "data/TileSet.hpp"
#include "defaults.hpp"

#if _WIN32
using ssize_t = intmax_t;
#endif

enum NodeType { TerrainBoundary, TerrainAnkle, TerrainCorner, LevelEdge };
    
struct Segment // Generic segments
{
    NodeType end1, end2;
    bool isOtherEnd;
    bool isSemiTerrain;
    size_t terrain, j, i1, i2;
};

template <bool Vertical>
inline void generateSegments(std::vector<Segment>& segments, const TileSet& tileSet, const util::grid<uint8_t>& layer)
{
// Replacement rule to help in the annoying things
#define TT TileSet::TileType
#define At TileSet::Attribute
    
    auto terrainCanMakeNewSegment = [](TileSet::TileType type)
    {
        return isContained(type, TT::TerrainUpperLeft, TT::TerrainUpperRight, TT::TerrainLowerLeft,
            TT::TerrainLowerRight, Vertical ? TT::TerrainLeft : TT::TerrainUp,
            Vertical ? TT::TerrainRight : TT::TerrainDown);
    };
    
    auto isCornerTerrain = [](TileSet::TileType type)
    {
        return isContained(type, TT::TerrainUpperLeft, Vertical ? TT::TerrainUpperRight : TT::TerrainLowerLeft);
    };

    auto isTerrainOtherEnd = [](TileSet::TileType type)
    {
        return isContained(type, TT::TerrainLowerRight, Vertical ? TT::TerrainUpperRight : TT::TerrainLowerLeft,
            Vertical ? TT::TerrainRight : TT::TerrainDown);
    };
    
    auto isTerrainSegmentCorner = [](TileSet::TileType type)
    {
        return isContained(type, TT::TerrainLowerRight, Vertical ? TT::TerrainLowerLeft : TT::TerrainUpperRight);
    };
    
    auto getViableContinuation = [](bool semiterrain, bool otherEnd)
    {
        if (semiterrain) return TT::SemiTerrain2;
        else if (otherEnd) return Vertical ? TT::TerrainRight : TT::TerrainDown;
        else return Vertical ? TT::TerrainLeft : TT::TerrainUp;
    };
    
    auto jSize = Vertical ? layer.width() : layer.height();
    auto iSize = Vertical ? layer.height() : layer.width();

    Segment curSegment;
    bool newSegment = false;
    for (size_t j = 0; j < jSize; j++)
    {
        const TileSet::TileIdentity* lastIdentity = nullptr;
        for (size_t i = 0; i < iSize; i++)
        {
            auto curTile = Vertical ? layer(j, i) : layer(i, j);
            const auto &identity = tileSet.tileIdentities[curTile];

            bool isViableSemiTerrain = TileSet::isSemiTerrain(identity.type) &&
                (Vertical ? TileSet::isIdHorizontalSemiTerrain(identity.id) :
                            TileSet::isIdVerticalSemiTerrain(identity.id));
            TileSet::Attribute terrainAttr = tileSet.getTileAttribute(curTile);
            
            if (TileSet::isTerrain(identity.type) || isViableSemiTerrain)
            {
                if (newSegment && !TileSet::refersToSame(*lastIdentity, identity))
                {
                    curSegment.i2 = i-1;
                    curSegment.end2 = TerrainBoundary;
                    segments.push_back(curSegment);
                    newSegment = false;
                }

            repeatNewSegmentPhase: // People will spank me, but...
                if (!newSegment && (isViableSemiTerrain || terrainCanMakeNewSegment(identity.type)))
                {
                    bool isTerrainCorner = isViableSemiTerrain ? identity.type == TT::SemiTerrain1 :
                        isCornerTerrain(identity.type);
                    bool isOtherEnd = isViableSemiTerrain ?
                        (Vertical ? isContained(terrainAttr, At::RightSolid, At::RightNoWalljump) :
                        terrainAttr == At::DownSolid) : isTerrainOtherEnd(identity.type);

                    newSegment = true;
                    curSegment.j = j;
                    curSegment.i1 = i;
                    curSegment.end1 = isTerrainCorner ? TerrainCorner : i == 0 ? LevelEdge :
                        TileSet::refersToSame(*lastIdentity, identity) ? TerrainAnkle : TerrainBoundary;
                    curSegment.isOtherEnd = isOtherEnd;

                    curSegment.isSemiTerrain = isViableSemiTerrain;
                    curSegment.terrain = identity.id;

                    if (isTerrainCorner) goto skipSegmentTermination;
                }
                
                if (newSegment && identity.type != getViableContinuation(isViableSemiTerrain, curSegment.isOtherEnd))
                {
                    bool isTerrainCorner = isViableSemiTerrain ? identity.type == TT::SemiTerrain3 :
                        isTerrainSegmentCorner(identity.type);
                    
                    curSegment.i2 = isTerrainCorner ? i : i-1;
                    curSegment.end2 = isTerrainCorner ? TerrainCorner : TerrainAnkle;
                    segments.push_back(curSegment);
                    newSegment = false;

                    bool isNewTerrainCorner = isViableSemiTerrain ? identity.type == TT::SemiTerrain1 :
                        isCornerTerrain(identity.type);
                    // In order to avoid code duplication
                    if (isNewTerrainCorner) goto repeatNewSegmentPhase;
                }
#undef At
#undef TT
            }
            else if (newSegment)
            {
                curSegment.i2 = i-1;
                curSegment.end2 = TerrainBoundary;
                segments.push_back(curSegment);
                newSegment = false;
            }

        skipSegmentTermination:
            lastIdentity = &identity;
        }

        if (newSegment)
        {
            curSegment.i2 = iSize-1;
            curSegment.end2 = LevelEdge;
            segments.push_back(curSegment);
            newSegment = false;
        }
    }
}

template <bool Vertical>
void convertShapes(std::vector<std::shared_ptr<cp::Shape>> &shapes, std::shared_ptr<cp::Body> body,
    const TileSet& tileSet, const std::vector<Segment>& segments, TileSet::Attribute* attributes)
{
    auto pi = Vertical ? &cpVect::y : &cpVect::x;
    auto pj = Vertical ? &cpVect::x : &cpVect::y;

    size_t i = 0;
    for (const Segment& segment : segments)
    {
        auto physicalParams = segment.isSemiTerrain ? TileSet::PhysicalParameters() :
            tileSet.terrains[segment.terrain].physicalParameters;

        auto perpnOffset = Vertical ? physicalParams.upperOffset : physicalParams.leftOffset,
             perppOffset = Vertical ? physicalParams.lowerOffset : physicalParams.rightOffset,
             parnOffset  = Vertical ? physicalParams.leftOffset  : physicalParams.upperOffset,
             parpOffset  = Vertical ? physicalParams.rightOffset : physicalParams.lowerOffset,
             cornerRadius = physicalParams.cornerRadius;

        cpVect endPoint1, endPoint2;
        endPoint1.*pj = endPoint2.*pj = DefaultTileSize * segment.j +
            (segment.isOtherEnd ? DefaultTileSize + perpnOffset - cornerRadius
                                : -perppOffset + cornerRadius);

        endPoint1.*pi = DefaultTileSize * segment.i1;
        switch (segment.end1)
        {
            case LevelEdge: endPoint1.*pi -= 4.0 * DefaultTileSize; break;
            case TerrainAnkle: endPoint1.*pi += parpOffset - cornerRadius; break;
            case TerrainCorner: endPoint1.*pi -= parnOffset - cornerRadius; break;
            default: break;
        } 
        
        endPoint2.*pi = DefaultTileSize * (segment.i2+1);
        switch (segment.end2)
        {
            case LevelEdge: endPoint2.*pi += 4.0 * DefaultTileSize; break;
            case TerrainAnkle: endPoint2.*pi -= parnOffset - cornerRadius; break;
            case TerrainCorner: endPoint2.*pi += parpOffset - cornerRadius; break;
            default: break;
        }

        attributes[i] = segment.isSemiTerrain ? TileSet::getSemiTerrainAttribute(segment.terrain) :
            tileSet.terrains[segment.terrain].terrainAttribute;
        auto shape = std::make_shared<cp::SegmentShape>(body, endPoint1, endPoint2, cornerRadius);
        shape->setUserData(attributes+i);
        shapes.push_back(shape);
        i++;
    }
}

inline void collectSingleObjects(std::vector<std::pair<size_t,size_t>>& singleObjectLocations,
    const TileSet& tileSet, const util::grid<uint8_t>& layer)
{
    auto height = layer.height();
    auto width = layer.width();

    for (size_t j = 0; j < height; j++)
        for (size_t i = 0; i < width; i++)
            if (TileSet::isSingleObject(tileSet.tileIdentities[layer(i, j)].type))
                singleObjectLocations.emplace_back(i, j);
}

inline void convertSingleObjects(std::vector<std::shared_ptr<cp::Shape>> &shapes, std::shared_ptr<cp::Body> body,
    const TileSet& tileSet, const util::grid<uint8_t>& layer,
    const std::vector<std::pair<size_t,size_t>>& singleObjectLocations, TileSet::Attribute* attributes)
{
    size_t i = 0;
    for (const auto& p : singleObjectLocations)
    {
        size_t x = p.first, y = p.second;
        auto curTile = layer(x, y);
        const auto& identity = tileSet.tileIdentities[curTile];
        if (!TileSet::isSingleObject(identity.type)) continue;

        std::shared_ptr<cp::Shape> shape;
        cpVect dp = { (cpFloat)DefaultTileSize * x, (cpFloat)DefaultTileSize * y };
        const auto& pts = tileSet.singleObjects[identity.id].points;
        auto radius = tileSet.singleObjects[identity.id].radius;
        switch (tileSet.singleObjects[identity.id].objectMode)
        {
            case TileSet::SingleObject::ObjectMode::TileMultiple:
            {
                cpFloat width = DefaultTileSize * pts[0].x - radius;
                cpFloat height = DefaultTileSize * pts[0].y - radius;
                shape = std::make_shared<cp::PolyShape>(body, std::vector<cpVect>
                    { cpVect{dp.x-width/2, dp.y-height/2}, cpVect{dp.x+width/2, dp.y-height/2},
                      cpVect{dp.x+width/2, dp.y+height/2}, cpVect{dp.x-width/2, dp.y+height/2} }, radius);
            } break;
            case TileSet::SingleObject::ObjectMode::Circle:
                shape = std::make_shared<cp::CircleShape>(body, radius, dp);
            break;
            case TileSet::SingleObject::ObjectMode::Segment:
            {
                cpVect pt1 = { dp.x+pts[0].x, dp.y+pts[0].y };
                cpVect pt2 = { dp.x+pts[1].x, dp.y+pts[1].y };
                shape = std::make_shared<cp::SegmentShape>(body, pt1, pt2, radius);
            } break;
            case TileSet::SingleObject::ObjectMode::Polygon:
            {
                std::vector<cpVect> cpPoints;
                cpPoints.reserve(pts.size());
                for (auto p : pts) cpPoints.push_back({ dp.x+p.x, dp.y+p.y });
                shape = std::make_shared<cp::PolyShape>(body, cpPoints, radius);
            } break;
        }

        attributes[i] = tileSet.singleObjects[identity.id].objectAttribute;
        shape->setUserData(attributes+i);
        shapes.push_back(shape);
        i++;
    }
}

std::vector<std::shared_ptr<cp::Shape>>
    generateShapesForTilemap(const RoomData& data, const TileSet& tileSet, std::shared_ptr<cp::Body> body,
    ShapeGeneratorDataOpaque& shapeGeneratorData)
{
    const auto& layer = data.mainLayer;

    std::vector<Segment> horizontalSegments, verticalSegments;
    generateSegments<false>(horizontalSegments, tileSet, layer);
    generateSegments<true>(verticalSegments, tileSet, layer);

    std::vector<std::pair<size_t,size_t>> singleObjectLocations;
    collectSingleObjects(singleObjectLocations, tileSet, layer); 

    auto totalSize = horizontalSegments.size() + verticalSegments.size() + singleObjectLocations.size();
    TileSet::Attribute* attrs = new TileSet::Attribute[totalSize];

    std::vector<std::shared_ptr<cp::Shape>> shapes;
    convertShapes<false>(shapes, body, tileSet, horizontalSegments, attrs);
    convertShapes<true>(shapes, body, tileSet, verticalSegments, attrs + horizontalSegments.size());
    convertSingleObjects(shapes, body, tileSet, layer, singleObjectLocations,
        attrs + (horizontalSegments.size() + verticalSegments.size()));

    shapeGeneratorData = ShapeGeneratorDataOpaque(static_cast<void*>(attrs),
        [](void* ptr) { delete[] static_cast<TileSet::Attribute*>(ptr); });

    return shapes;
}
