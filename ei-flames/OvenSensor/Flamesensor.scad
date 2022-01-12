// the KY-026 Flame sensor module
pcb_KY026 = [1.4*25.4, 0.6*25.4, 1];

use <Connectors.scad>

module ir_led(h=8.6, d=5.0)
{
    color("Blue")
    union() {
    cylinder(h=1.0, d=d+1, center=true, $fn=24);
    translate([0, 0, (h-d/2)/2]) {
    cylinder(h=h-d/2, d=d, center=true, $fn=24);
    translate([0, 0, (h-d/2)/2]) sphere(d=d, $fn=24);
    } }
    color("white")
    union() {
    pin_l = 5;  // pin length
    translate([0,2.54/2,-pin_l])
    cube([1/2,1/2,pin_l]);
    translate([0,-2.54/2,-pin_l])
    cube([1/2,1/2,pin_l]);}
}

module flame_sensor_connectors(def=pcb_KY026,d=2.54,w=0.5) {
    // the connector
    left=[d+d/2,def[1]/2,0];
    translate(left+[-w/2,-w/2,-w+1.5])
    female_connectors_90();
}

module flame_sensor_holes(def=pcb_KY026,d=2.54,w=0.5,n=4) {
    left=[d+d/2,def[1]/2,0];
    translate(left)
    for (xl = [0: d : (n-1)*d]) {
        translate([-d-w/2, xl-(n-1)*d/2, -d])
        cylinder(h=2*d, r=w, $fn=24);
    }
}

module flame_sensor(def=pcb_KY026) {
    color("Red")
    difference() {
    cube(def);
    translate(pcb_KY026-[0.15*25.4,(0.4+0.1)*25.4,1.5])
    cube([0.2*25.4, 0.4*25.4, 2]);
    translate([8, def[1]/2, -1.5]) 
        cylinder(d=3,h=3, $fn=24);

    // the connector holes
    flame_sensor_holes(def=def);
    }
    // the connector
    flame_sensor_connectors(def=def);
    // the IR led
    translate([1.1*25.4,def[1]/2,1+5])
    ir_led();
}

flame_sensor();
