// Oven Sensor

use <Sensor.scad>

pcb_14x20 = [60, 40, 1];
wall = 1.2;
air = 0.2;
thick=(wall+air);
pcb_offset = [thick, thick, thick+2.54];
case_body = [60+1,40+1,49];
case_front = [thick,case_body[1],case_body[2]];
case_back  = [thick,case_body[1],case_body[2]];

module body(sensor=false) {
    color("Silver", 1.0)
    difference() {
      // the box
      cube(case_body+[2*thick, 2*thick, 2*thick]);
      translate([-thick,thick,thick])
        cube(case_body+[4*thick, 0, 0]);
      // the pcb holes
      translate([thick, thick, 0])
        oven_sensor_mount(pcb_14x20);   
    }
    if (sensor==true) {
      translate(pcb_offset)
        oven_sensor(pcb_14x20);
    }
}

module dht11_sensor_hole(cube=[16, 12, 8]) {
  cube(cube+[4*air,4*air,4*air]);
}

module flame_sensor_hole() {
  cylinder(d=5+4*air, h=4*thick, $fn=24);
}

module front() {
  translate([-thick, 0,0]) 
    difference() {
      union(){
        cube(case_front+[0, 2*thick, 2*thick]);
        translate([thick-air, thick,thick])
        difference() {
          cube(case_front+[air, 0, 0]);
          translate([0, thick,thick])
            cube(case_front+[thick, -2*thick, -2*thick]);
        }
      }
      translate([+2*thick,33,38.25+2.54]) 
      rotate([0,-90,0])
      flame_sensor_hole();
      //
      translate([+4*thick,3.25+air,19.5-2*air+2.54]) 
      rotate([0,-90,0])
      dht11_sensor_hole();
    }
}

// size of an usb b connector (hole is 12x7)
module usb_b_connector_hole(size = [17, 12, 7]) {
        cube(size+[4*air,4*air,4*air]);
}

module back() {
  translate([60+2*thick, 0,0]) {
     difference() { 
      union(){
        cube(case_back+[0, 2*thick, 2*thick]);
        translate([-(thick-air), thick,thick])
        difference() {
          cube(case_back+[air, 0, 0]);
            translate([0, thick,thick])
          cube(case_back+[thick, -2*thick, -2*thick]);
        }
      }
      // usb connector hole
      translate([-case_back[0], case_back[1]/2-5.5-air+wall,wall+9+1+2.54])
      usb_b_connector_hole();
  }
}
}

// show all we have
body(sensor=true);
front();
back();
