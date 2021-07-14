Hauling View {#pgstable_hauling_view}
=====================
The Hauling View provides the parameters for hauling analysis as well as the hauling analysis report. Use the input fields on the left side of the view to define the specific parameters of a hauling analysis. The report on the right side of the view will automatically update as the analysis parameters change.

Analysis is performed for two cases; Normal Crown Slope and Maximum Superelevation.

## Effective Prestress Force at Lifting (Fpe)
Enter the effective prestress force at the time of hauling. If the Prestress Definition in the Girder View is set to Simplified, enter the effective prestress force in the straight, harped, and temporary strands. If the effective Prestress Defintiion in the Girder View is set to Detailed, press [Edit Fpe] and enter the prestress force and its location relative to the top or bottom of the girder.

## Material
Enter the girder material at the time of hauling.

Item | Description
-----|---------------
f'c | Concrete strength
Ec  | Modulus of elasticity. If this box is checked, enter a value, otherwise the modulus of elasticity will be computed
fr   | Modulus of rupture. Enter the coefficient for computing the modulus of rupture
K1, K2 | Averaging and bounding factor used when computing the modulus of elasticity

## Analysis Parameters
Enter the analysis parameters

Item | Description
-----|-----------------
Trailing Bunk | Distance to the bunk point at the trailing end of the haul configuration
Leading Bunk | Distance to the bunk point at the leading end of the haul configuration
Impact Up/Down | Dynamic load allowance (impact). Select how the impact loading is to be applied. It can be applied to the normal crown slope case, the maximum super elevation case, or both.
Normal Crown Slope | Enter the normal crown slope
Max. Superelevation | Enter the maximum expectied superelevation to be encountered during the haul
Wind Loading | Select the wind loading type and enter the loading parameters. For Wind Speed, enter the wind velocity and wind pressure will be comptued according to LRFD 3.8.1.2. For Wind Pressure, enter a pressure loading applied to the vertical surface of the girder.
Overhang Bracket | Deck form overhang brackets may be attached to the girder prior to transport to facilitate on site construction. Enter an equivalent weight per unit length and eccentricity of the resultant overhang bracket weight from the centerline of the girder. Positive value for weight is in the direction of gravity (downwards). Positive values for eccentricity is in the direction of sweep.
Centrifugal Force | Select the centrifugal force type; Favorable or Adverse. Favorable forces act away from the center of the turning movement while Adverse forces act inwards.
Speed | Enter the turning speed
Turning Radius | Enter the turning radius
Vertical Camber | Enter the camber and a camber multiplier, if applicable.
Lateral Camber | Asymmetric girders can have lateral deflections due to the geometry of the section. Prestressing that is not symmetric with respect to the vertical axis of the girder can also have lateral deflection. Enter the lateral camber if applicable.
Haul Truck | Select a haul truck from the drop down list or select "Define haul truck" to input the haul truck parameters
Height of girder bottom above roadway | Enter the location of the bottom of the girder from the roadway surface directly above the roll center
Height of roll axis above roadway | Enter the location of the roll axis above the roadway surface
Truck Roll Stiffness | Enter the roll stiffness of the haul truck
Truck Width | Enter the center to center width of the truck wheels
Sweep Tolerance | Enter the girder lateral sweep tolerance. Lateral sweep is added to lateral camber.
Support Placement Tolerance | Enter the lateral tolerance for placement of the bunking devices.

> NOTE: Centrifugal forces are only applied to the Max. Superelevation case

## Criteria
Define the analysis criteria

Item | Description
-----|----------
Factors of Safety | Enter the factors of safety against cracking and failure
Evaluate stresses | Select the method for computing and evaluating stresses in the girder. Stresses computed for a plumb girder do not account for the tilt of the girder into its equilibrium position. Stresses computed for a tilted girder account for the girder tilting into its equilibrium position.
Compression | Enter the coefficients for computing allowable compression. The general compression limit is used for compression stress in the general bending condition (girder weight plus prestress). The lateral bending compression limit is used for peak stresses at the section extremities when lateral bending due to girder tilt, wind, and centrifugal force are considered.
Tension | Enter the coefficients for computing allowable tension with and without sufficient bonded reinforcement. There are allowable tension limits for the Normal Crown Slope and Maximum Superelevation cases
Max. distance between bunk points | Enter the maximum permissible distance between bunk points
Max. leading overhang | Enter the maximum leading overhanging
Max. girder weight | Enter the maximum girder weight that can be hauled.