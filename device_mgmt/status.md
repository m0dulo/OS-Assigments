#initial status

```dot
digraph G {
   node [fontname = "Consolas"]
   CPU[shape = "hexagon",label="cpu",fontsize = 30]

   CH1[label="CH1",fontsize = 25]
   CH2[label="CH2",fontsize = 25]

   CO1[label="CO1",fontsize = 25]
   CO2[label="CO2",fontsize = 25]
   CO3[label="CO3",fontsize = 25]

   DC1[label="KB(I)",fontsize = 25]
   DC2[label="MOUSE(I)",fontsize = 25]
   DC3[label="PTR(O)",fontsize = 25]
   DC4[label="MOR(O)",fontsize = 25]

   CH1P[shape = "record", label="{PROCESS|}"]
   CH2P[shape = "record", label="{PROCESS|}"]
   CO1P[shape = "record", label="{PROCESS|}"]
   CO2P[shape = "record", label="{PROCESS|}"]
   CO3P[shape = "record", label="{PROCESS|}"]
   DC1P[shape = "record", label="{PROCESS|}"]
   DC2P[shape = "record", label="{PROCESS|}"]
   DC3P[shape = "record", label="{PROCESS|}"]
   DC4P[shape = "record", label="{PROCESS|}"]

    CH1B[shape = "record", label = "{WAITING||---}"]
    CH2B[shape = "record", label = "{WAITING||---}"]
    CO1B[shape = "record", label = "{WAITING||---}"]
    CO2B[shape = "record", label = "{WAITING||---}"]
    CO3B[shape = "record", label = "{WAITING||---}"]
    DC1B[shape = "record", label = "{WAITING||---}"]
    DC2B[shape = "record", label = "{WAITING||---}"]
    DC3B[shape = "record", label = "{WAITING||---}"]
    DC4B[shape = "record", label = "{WAITING||---}"]

   DC1 -> CO1
   CO1 -> DC1

   DC2 -> CO1
   CO1 -> DC2

   DC3 -> CO2
   CO2 -> DC3

   DC4 -> CO3
   CO3 -> DC4

   CO1 -> CH1
   CH1 -> CO1

   CO2 -> CH2
   CH2 -> CO2

   CO3 -> CH2
   CH2 -> CO3

   CH1 -> CPU [style=dashed,dir=none]
   CH2 -> CPU [style=dashed,dir=none]

    
    CH1 -> CH1P
    CH2 -> CH2P
    CO1->CO1P
    CO2 -> CO2P
    CO3 -> CO3P
    DC1->DC1P
    DC2->DC2P
    DC3->DC3P
    DC4->DC4P

    CH1 -> CH1B
    CH2 -> CH2B
    CO1 -> CO1B
    CO2 -> CO2B
    CO3 -> CO3B
    DC1 -> DC1B
    DC2 -> DC2B
    DC3 -> DC3B
    DC4 -> DC4B



}
```