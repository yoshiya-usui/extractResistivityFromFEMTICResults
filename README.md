# extractResistivityFromFEMTICResults
This is a tool to extract resistivity from the output files of FEMTIC.
You need to execute it with the following arguments.
[mesh type(2)] [iteration number] [name of points file]
This version only supports the nonconforming hexahedral mesh (mesh type = 2).
In the name of points files, the x-, y-, and z-coordinates (km) should be listed for each point.
If the program works appropriately, the x-, y-, and z-coordinates and resistivity (Ohm-m) are listed in the output file (resistivity.txt).


