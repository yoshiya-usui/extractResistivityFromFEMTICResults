# extractResistivityFromFEMTICResults
This is a tool to extract resistivity from the output files of FEMTIC.

You need to execute it with the following arguments.

[mesh type (2)] [iteration number] [name of points file]

This version only supports the nonconforming hexahedral mesh (mesh type = 2).

In the name of points files, the x-, y-, and z-coordinates (km) should be listed for each point at which you want to obtain the resistivity.

If the program works appropriately, the resistivity (Ohm-m) of each point is listed with the x-, y-, and z-coordinates in the output file (resistivity.txt).


