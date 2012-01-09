Element["" "SMT LED (pin 1 is anode)" "" "PLCC2" 0 0 22100 0 3 100 ""]
(
	ElementLine[-6690  -3900 -6690  5900 1000] # Left
	ElementLine[-6690    5900  6690  5900 1000] # Bottom
	ElementLine[ 6690   5900  6690 -5900 1000] # Right 
	ElementLine[ 6690  -5900  -4690 -5900 1000] # Top
	ElementLine[-6690  -3900  -4690 -5900 1000] # Top Left
	ElementArc[0 0 4000 4000 0 360 1000]

	Pad[-5860 -2118 
	    -5860  2118
	      6000 2000 6600 "2" "2" "square"]
	Pad[5860 -2118
	    5860  2118
	     6000 2000 6600 "1" "1" "square"]
)
