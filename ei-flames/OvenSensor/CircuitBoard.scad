// PCB circuit board breadboard
pcb_14x20 = [60, 40, 1];

module circuit_board_mount(def, air=0.2) {
    r = 1+air;
    b = [r+2.54/2,r+2.54/2,1];
    color("Darkgreen")
    union() {
        translate(b)
        cylinder(h=4, r=r, center=true, $fn=24);
        translate([def[0]-b[0],b[1],1])
        cylinder(h=4, r=r, center=true, $fn=24);
        translate([b[0],def[1]-b[1],1])
        cylinder(h=4, r=r, center=true, $fn=24);
        translate([def[0]-b[0],def[1]-b[1],1])
        cylinder(h=4, r=r, center=true, $fn=24);
    }
}

module circuit_board(def) {
    color("Darkgreen")
    difference() {
        cube(def);
        circuit_board_mount(def);
    }
}

circuit_board(pcb_14x20);
