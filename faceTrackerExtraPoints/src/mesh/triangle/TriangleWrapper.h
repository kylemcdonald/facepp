#ifndef __OV_TRIANGLEWRAPPER_H__
#define __OV_TRIANGLEWRAPPER_H__

#include "ofMain.h"
// AP #include "Overture.h"
//#include "OvertureTypes.h"
//#include "wdhdefs.h" // for sPrintF, etc.
//#include "mathutil.h" // for min, max, etc.

class TriangleWrapperParameters
{

public:
  TriangleWrapperParameters( double minAng=-1, bool freeze=false, double maxA=-1, bool vor=true,
           bool neighbours=false, bool quiet=true, int verbose=0  ) : 
    minAngle(minAng), freezeSegments(freeze), maxArea(maxA), voronoi(vor), saveNeighbours(neighbours),
        quietMode(quiet), verboseMode(verbose) { }

  TriangleWrapperParameters( const TriangleWrapperParameters & triw ) : 
    minAngle(triw.minAngle), freezeSegments(triw.freezeSegments), maxArea(triw.maxArea) { }

  ~TriangleWrapperParameters() { }

  void setMinimumAngle(double angle) { minAngle = angle; }
  void toggleFreezeSegments()      { freezeSegments = !freezeSegments; }
  void setMaximumArea(double area)   { maxArea = area; }
  void toggleVoronoi()             { voronoi = !voronoi; }
  void saveVoronoi(bool trueOrFalse=true){ voronoi=trueOrFalse; }
  void saveNeighbourList(bool trueOrFalse=true){ saveNeighbours=trueOrFalse; }
  void setQuietMode(bool trueOrFalse=true){ quietMode=trueOrFalse; }
  // turn on verbose mode, level=1 is "V", level=2 is "VV" level=3 is "VVV"
  void setVerboseMode(int level){ verboseMode=level; }

  double getMinimumAngle() const   { return minAngle; }
  bool getFreezeSegments() const { return freezeSegments; }
  double getMaximumArea() const    { return maxArea; }
  bool getVoronoi() const        { return voronoi; }
  bool getSaveNeighbours()       { return saveNeighbours; }

  string generateArgumentString() const;

private:
  
  double minAngle;
  bool freezeSegments;
  double maxArea;
  bool voronoi;
  bool saveNeighbours;
  bool quietMode;
  int verboseMode;
  
};


class TriangleWrapper
{

public:

  TriangleWrapper();

  TriangleWrapper( vector  < int > & faces, vector  < double > & xyz ) 
  { initialize( faces, xyz ) ; }

  ~TriangleWrapper() { }

  void initialize(  vector  < int > & faces,  vector  < double > & xyz );
  void setHoles( const vector  < double > &holes );
  void generate();
  
  TriangleWrapperParameters & getParameters() { return parameters; }

  int getNumberOfEdges() const { return numberOfEdges; }  //
  int getNumberOfBoundaryEdges() const { return numberOfBoundaryEdges; }  //

  const vector  < int > & generateElementList () const { return triangles; }
  const vector  < double > & getPoints() const { return points; }
  const vector  < int > & getInitialFaces2TriangleMapping() const { return initialFaces2TriangleMapping; }
  const vector  < int > & getNeighbours() const { return neighbours; }
	TriangleWrapperParameters parameters;
//protected:
  

//private:
  vector  < int > initialFaces;
  vector  < double > initialPoints;
  vector  < double > holes;

  vector  < int > triangles;
  vector  < double > points;
  vector  < int > initialFaces2TriangleMapping;
  vector  < int > neighbours;
  
  int numberOfEdges, numberOfBoundaryEdges;
  
};

#endif
