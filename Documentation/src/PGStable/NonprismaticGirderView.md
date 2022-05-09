Nonprismatic Girder {#pgstable_nonprismatic_girder_view}
=====================
A non-prismatic girder has section properties properties that vary along its length.

## Girder Properties
Define the girder as a sequence of discrete sections that vary linerally over a length.

Use the [+] and [-] buttons to define girder sections.

Section properties are defined at the start and end of each section. Properties are linerally interpolated along the section.

### Stress Points
Stress points define where girder stresses are computed.

Item | Description
-----|----------------
Compute from Properties | Computes top left, top right, bottom left, and bottom right using Height, Wtf, Wbf, and Ytop dimensions
Explicitly Define | Explicitly enter the coordinates of the top left, top right, bottom left, and bottom right corners of the girder. The origin is at the centroid of the section. This option is intended to be used for asymmetric girders.

### Girder Properties
Item | Description
-----|------------------
Height | Overall height
Wtf | Top width
Wbf | Bottom width
Area | Cross sectional area
Ix | Moment of inertia about the horizontal centroidal axis
Iy | Moment of inertia about the vertical centroidal axis
Ytop | Location of the centroid measured down from the top of the girder
Xleft | Location of the centroid measured from the extreme left edge of the girder
L | Length of girder section
Top Left (X,Y) | Top left corner of the girder section
Top Right (X,Y) | Top right corner of the girder section
Bottom Left (X,Y) | Bottom left corner of the girder section
Bottom Right (X,Y) | Bottom right corner of the girder section

Precamber is a camber that is constructed into the girder (this is not the natural camber caused be girder self-weight and prestressing)

## Materials & Dead Loads
The self weight of the girder is computed from the section properties and the unit weight with reinforcement. Additional dead loads can be modeled.

Item | Description
-----|----------------
Unit Weight | Unit weight of concrete used to compute modulus of elasticity
Unit Weight with Reinforcement | Unit weight of concrete and reinforcement. Used to compute dead load.
Normalweight/Lightweight Concrete | Indicate the type of concrete used.
K1 | Averaging factor used when computing the modulus of elasticity
K2 | Bounding factor used when computing the modulus of elasticity

### Additional Dead Loads
Additional dead loads are modeld as discrete point loads. These loads could represent the weight of cast in place diaphragms.

Use the [+] and [-] buttoms to add and delete additional dead loads.

Item | Description
-----|----------------
X | Location of the load measured from the left end of the girder
P | Magnitude of the load (positive values for downward/gravity loads)

## Wind Characteristics
Enter the girder drag coefficient. This coefficient is used for computing wind pressure loading per LRFD 3.8.3.1. The Florida Department of Transportation has undertaken a significant amount of research to establish the following guidance for horizontal drag:
* Cd = 2.2 for I-shaped girders with no deck forms in place
* Cd = 1.5 for U-shaped girders with no deck forms in place

## Prestress Arrangement
The prestressing may be defined with a Simplified or Detailed arrangement. A simplified arrangement consists of parameters that describe the location of the prestressing force as well as a prestress transfer length. The prestressing force is defined on the Lifting and Hauling views. In a detailed arrangement, the location and magnitude of the prestressing force is defined at discrete locations along the girder. This is accomplished in the Lifting and Hauling views.

Use the drop down list to select the Prestressing Arrangement

Item | Description
-----|----------------
Simplified | A simplified prestressing arrangement will be used
Detailed | A detailed prestressing arrangement will be used

### Simplifed Arrangement Parameters

Item | Description
-----|----------------
ex | Lateral eccentricty of prestressing force (typically zero)
Prestress Transfer Length | Length from the ends of the girder that the prestress force is increased from zero to its full value
Ys | Location of the straight strands measured from the Top/Bottom face of the girder
Xh1-4 | Location of the harp points measured from the left end of the girder as distance or a percentage of the girder length
Yh1-4 | Location of the harp points measured from the Top/Bottom face of the girder
Yts | Location of the temporary strands
