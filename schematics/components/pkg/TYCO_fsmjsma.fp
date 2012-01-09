# $Id: TYCO_fsmjsma.fp,v 1.1 2008/09/08 09:22:13 alexander_kurz Exp $
Element[0x0 "Tyco Alcoswitch FSMJSMA" "SWx" "fsmjsma" 26575 15000 -5000 -22000 0 100 ""]
(
	# signal pads
	Pad[ 8900 -16550 8900 -19350 5500 1000 4543 "1"  "1" "square"]
	Pad[ 8900 16550 8900 19350 5500 1000 4543 "2"  "2" "square"]
	Pad[ -8900 -16550 -8900 -19350 5500 1000 4543 "3"  "3" "square"]
	Pad[ -8900 16550 -8900 19350 5500 1000 4543 "4"  "4" "square"]
	# the element box
	ElementLine [-11800 -11800 -11800 11800 1000]
	ElementLine [11800 -11800 11800 11800 1000]
	ElementLine [-11800 -11800 11800 -11800 1000]
	ElementLine [-11800 11800 11800 11800 1000]
	# the stylus
	ElementArc[0 0 6900 6900 0 0 1000]
	ElementArc[0 0 6900 6900 0 180 1000]
	# two connected pins
	ElementLine [-8850 -10325 -8850 10325 1000]
	ElementLine [8850 -10325 8850 10325 1000]
	ElementLine [8850 -8850 2950 -8850 1000]
	ElementLine [-8850 -8850 -2950 -8850 1000]
	ElementLine [2950 -10325 -2950 -8850 1000]

	#
	Attribute("author" "Alexander Kurz")
	Attribute("copyright" "2008 Alexander Kurz")
	Attribute("use-license" "Unlimited")
	Attribute("dist-license" "CC-BY-SA-2.5")
	Attribute("height" "3.8mm")
	Attribute("ref" "tycoelectronics.com document number 1308390_0904_E3_E10")
)
