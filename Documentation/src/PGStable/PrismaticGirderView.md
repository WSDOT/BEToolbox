Prismatic Girder {#pgstable_prismatic_girder_view}
=====================
A prismatic girder has constant section properties.

## Girder Properties
Define the girder section properties. Use the drop down list to select a girder or select "Defined girder properties" to enter the girder properties.

Item | Description
-----|------------------
Height | Overall height
Wtf | Top width
Wbf | Bottom width
Area | Cross sectional area
Ix | Moment of inertia about the horizontal centroidal axis
Iy | Moment of inertia about the vertical centroidal axis
Yt | Location of the centroid measured down from the top of the girder
L | Girder length

## Dead Loads
The self weight of the girder is computed from the section properties and the unit weight with reinforcement. Additional dead loads can be modeled.

Item | Description
-----|----------------
Unit Weight | Unit weight of concrete used to compute modulus of elasticity
Unit Weight with Reinforcement | Unit weight of concrete and reinforcement. Used to compute dead load.

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
Prestress Transfer Length | Length from the ends of the girder that the prestress force is increased from zero to its full value
Ys (E) | Location of the straight strands
Yh1 (Fo) | Location of the harped strands at the end of the girder
Yh2 (Fcl) | Location of the harped strands at the harp point
Harp Point | Location of the harped point as a fraction of the girder length. Enter a value less than or equal to 0.5
Yts | Location of the temporary strands

> NOTE: E, Fo, and Fcl are dimensions found on WSDOT standard girder sheets.
